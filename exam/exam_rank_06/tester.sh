#!/usr/bin/env bash

set -euo pipefail

ROOT_DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
SRC="$ROOT_DIR/mini_serv.c"
TMP_DIR=$(mktemp -d)
BIN="$TMP_DIR/mini_serv"
SLOW_SRC="$TMP_DIR/slow_client.c"
SLOW_BIN="$TMP_DIR/slow_client"
PORT=$((30000 + RANDOM % 10000))

declare -A CLIENT_PID
declare -A CLIENT_FD
declare -A CLIENT_OUT
declare -A CLIENT_OFF

fail() {
	echo "[FAIL] $*" >&2
	exit 1
}

cleanup() {
	set +e
	for name in "${!CLIENT_PID[@]}"; do
		if [[ -n ${CLIENT_FD[$name]:-} ]]; then
			eval "exec ${CLIENT_FD[$name]}>&-" 2>/dev/null
		fi
		kill "${CLIENT_PID[$name]}" 2>/dev/null
		wait "${CLIENT_PID[$name]}" 2>/dev/null
	done
	if [[ -n ${SERVER_PID:-} ]]; then
		kill "$SERVER_PID" 2>/dev/null
		wait "$SERVER_PID" 2>/dev/null
	fi
	rm -rf "$TMP_DIR"
}

trap cleanup EXIT INT TERM

need_cmd() {
	command -v "$1" >/dev/null 2>&1 || fail "missing required command: $1"
}

assert_eq() {
	local expected=$1
	local actual=$2
	local message=$3
	if [[ "$actual" != "$expected" ]]; then
		printf '[FAIL] %s\nexpected: %q\nactual:   %q\n' "$message" "$expected" "$actual" >&2
		exit 1
	fi
}

wait_for_port() {
	local tries=0
	while (( tries < 50 )); do
		if nc -z 127.0.0.1 "$PORT" >/dev/null 2>&1; then
			return 0
		fi
		sleep 0.05
		tries=$((tries + 1))
	done
	fail "server did not start on port $PORT"
}

start_server() {
	SERVER_STDOUT="$TMP_DIR/server.out"
	SERVER_STDERR="$TMP_DIR/server.err"
	: > "$SERVER_STDOUT"
	: > "$SERVER_STDERR"
	"$BIN" "$PORT" >"$SERVER_STDOUT" 2>"$SERVER_STDERR" &
	SERVER_PID=$!
	wait_for_port
}

start_nc_client() {
	local name=$1
	local in_fifo="$TMP_DIR/${name}.in"
	local out_file="$TMP_DIR/${name}.out"
	: > "$out_file"
	mkfifo "$in_fifo"
	exec {fd}>"$in_fifo"
	nc 127.0.0.1 "$PORT" <"$in_fifo" >"$out_file" &
	CLIENT_PID[$name]=$!
	CLIENT_FD[$name]=$fd
	CLIENT_OUT[$name]=$out_file
	CLIENT_OFF[$name]=0
	sleep 0.1
}

send_client() {
	local name=$1
	local data=$2
	printf '%s' "$data" >&"${CLIENT_FD[$name]}"
}

close_client() {
	local name=$1
	eval "exec ${CLIENT_FD[$name]}>&-"
	local pid=${CLIENT_PID[$name]}
	local start=$SECONDS
	while kill -0 "$pid" 2>/dev/null; do
		if (( SECONDS - start >= 5 )); then
			kill "$pid" 2>/dev/null || true
			break
		fi
		sleep 0.05
	done
	wait "$pid" 2>/dev/null || true
}

read_new_output() {
	local name=$1
	local file=${CLIENT_OUT[$name]}
	local off=${CLIENT_OFF[$name]}
	if [[ ! -s $file ]]; then
		echo -n ""
		return 0
	fi
	if (( off == 0 )); then
		cat "$file"
	else
		tail -c +$((off + 1)) "$file"
	fi
}

update_offset() {
	local name=$1
	CLIENT_OFF[$name]=$(stat -c %s "${CLIENT_OUT[$name]}")
}

wait_for_output() {
	local name=$1
	local expected=$2
	local timeout=${3:-5}
	local start=$SECONDS
	while (( SECONDS - start < timeout )); do
		local got
		got=$(read_new_output "$name")
		if [[ "$got" == "$expected" ]]; then
			update_offset "$name"
			return 0
		fi
		sleep 0.05
	done
	local got
	got=$(read_new_output "$name")
	printf '[FAIL] unexpected output for %s\nexpected new data: %q\nactual new data:   %q\n' "$name" "$expected" "$got" >&2
	exit 1
}

build_slow_client() {
	cat > "$SLOW_SRC" <<'EOF'
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

static void die(const char *msg)
{
	perror(msg);
	exit(1);
}

int main(int ac, char **av)
{
	struct sockaddr_in addr;
	int fd;
	int flags;
	char buffer[4096];
	ssize_t bytes;

	if (ac != 2)
		return (2);
	fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd < 0)
		die("socket");
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(atoi(av[1]));
	addr.sin_addr.s_addr = htonl(2130706433);
	if (connect(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
		die("connect");
	flags = fcntl(fd, F_GETFL, 0);
	if (flags < 0)
		die("fcntl");
	if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) < 0)
		die("fcntl");
	sleep(3);
	while ((bytes = recv(fd, buffer, sizeof(buffer), 0)) > 0)
		write(1, buffer, bytes);
	if (bytes == 0)
		return (3);
	if (errno != EAGAIN && errno != EWOULDBLOCK)
		die("recv");
	return (0);
}
EOF
	gcc -Wall -Wextra -Werror "$SLOW_SRC" -o "$SLOW_BIN"
}

test_no_arguments() {
	local out="$TMP_DIR/noargs.out"
	local err="$TMP_DIR/noargs.err"
	: > "$out"
	: > "$err"
	if "$BIN" >"$out" 2>"$err"; then
		fail "server should fail with no arguments"
	fi
	assert_eq "" "$(cat "$out")" "unexpected stdout for no-argument run"
	assert_eq "Wrong number of arguments" "$(cat "$err")" "unexpected stderr for no-argument run"
}

test_bind_conflict() {
	local out="$TMP_DIR/conflict.out"
	local err="$TMP_DIR/conflict.err"
	: > "$out"
	: > "$err"
	if "$BIN" "$PORT" >"$out" 2>"$err"; then
		fail "second server should fail when the port is already in use"
	fi
	assert_eq "" "$(cat "$out")" "unexpected stdout for bind conflict"
	assert_eq "Fatal error" "$(cat "$err")" "unexpected stderr for bind conflict"
}

test_basic_chat() {
	start_nc_client c1
	wait_for_output c1 ""

	start_nc_client c2
	wait_for_output c1 $'server: client 1 just arrived\n'
	wait_for_output c2 ""

	send_client c1 $'hello\nworld\n'
	wait_for_output c2 $'client 0: hello\nclient 0: world\n'
	wait_for_output c1 ""

	send_client c2 $'bye\n'
	wait_for_output c1 $'client 1: bye\n'
	wait_for_output c2 ""

	close_client c2
	wait_for_output c1 $'server: client 1 just left\n'

	close_client c1
}

test_fragmented_message() {
	start_nc_client c3
	wait_for_output c3 ""

	start_nc_client c4
	wait_for_output c3 $'server: client 3 just arrived\n'
	wait_for_output c4 ""

	send_client c3 'par'
	sleep 0.1
	send_client c3 $'tial\nnext\n'
	wait_for_output c4 $'client 2: partial\nclient 2: next\n'
	wait_for_output c3 ""

	send_client c3 $'split\n'
	wait_for_output c4 $'client 2: split\n'
	wait_for_output c3 ""

	close_client c4
	wait_for_output c3 $'server: client 3 just left\n'
	close_client c3
}

test_slow_reader() {
	build_slow_client
	"$SLOW_BIN" "$PORT" >"$TMP_DIR/slow.out" 2>"$TMP_DIR/slow.err" &
	local slow_pid=$!
	sleep 0.4
	local spam_payload
	spam_payload=$(head -c 900 </dev/zero | tr '\0' 'X')
	{
		for _ in $(seq 1 2500); do
			printf '%s\n' "$spam_payload"
		done
	} | nc 127.0.0.1 "$PORT" >/dev/null
	local start=$SECONDS
	while kill -0 "$slow_pid" 2>/dev/null; do
		if (( SECONDS - start >= 8 )); then
			kill "$slow_pid" 2>/dev/null || true
			wait "$slow_pid" 2>/dev/null || true
			fail "slow-reader scenario timed out"
		fi
		sleep 0.05
	done
	wait "$slow_pid"
	local status=$?
	if [[ $status -eq 3 ]]; then
		fail "slow nonblocking client was disconnected"
	fi
	if [[ $status -ne 0 ]]; then
		fail "slow nonblocking client helper failed"
	fi
	assert_eq "" "$(cat "$TMP_DIR/slow.err")" "slow client should not print errors"
}

need_cmd gcc
need_cmd nc
need_cmd head
need_cmd tr
need_cmd cat
need_cmd tail
need_cmd stat

gcc -Wall -Wextra -Werror "$SRC" -o "$BIN"

test_no_arguments

start_server
test_bind_conflict
test_basic_chat
test_fragmented_message
test_slow_reader

echo "All tester.sh scenarios passed."

#!/bin/bash

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

ref_path="maps-intermediate/mouli_maps_solved"
maps_path="maps-intermediate/mouli_maps"

echo -e "${BLUE}Compiling student implementation...${NC}"
gcc -Wall -Wextra -Werror main.c -o bsq

for map in $maps_path/*; do
	echo -e "${BLUE}Testing with map: $map${NC}"
	./bsq $map > my_output.log
	if [ $? -ne 0 ]; then
		echo "❌ failed to run for $map!"
		break
	fi
	ref_map="$ref_path/$(basename $map)"
	cat $ref_map > ref_output.log
	diff my_output.log ref_output.log
	if [ $? -ne 0 ]; then
		echo "❌ failed for $map!"
	else
		echo "✅ passed for $map!"
	fi
done

echo -e "${YELLOW}Testing with valgrind...${NC}"
for map in $maps_path/*; do
	echo -e "${BLUE}Testing with map: $map${NC}"
	valgrind --leak-check=full --show-leak-kinds=all ./bsq $map > my_output.log 2> valgrind_output.log
	if [ $? -ne 0 ]; then
		echo "❌ failed to run for $map!"
		break
	fi
	# Wait for the program to finish writing before checking:
	wait
	grep "definitely lost" valgrind_output.log | grep -v "All heap blocks were freed -- no leaks are possible" > /dev/null
	if [ $? -ne 0 ]; then
		echo "❌ failed for $map!"
	else
		echo "✅ passed for $map!"
	fi
done

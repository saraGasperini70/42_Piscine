#!/bin/bash

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

echo -e "${BLUE}Compiling reference implementation...${NC}"
gcc -Wall -Wextra -Werror -std=c99 -o ref ../42_examshell/.resources/rank05/level2/life/life.c
echo -e "${BLUE}Compiling student implementation...${NC}"
gcc -Wall -Wextra -Werror -std=c99 main.c

#Test 1:
echo -e "${BLUE}Test 1 - Basic functionality:${NC}"
echo 'sdxddssaaww' | ./a.out 5 5 0 > my_output.log | cat -e && echo 'sdxddssaaww' | ./ref 5 5 0 > ref_output.log | cat -e
diff my_output.log ref_output.log
if [ $? -ne 0 ]; then
	echo "1. ❌ failed!"
else
	echo "1. ✅ passed!"
fi

echo -e "${BLUE}Test 2 - Larger board with more iterations:${NC}"
echo 'sdxssdswdxddddsxaadwxwdxwaa' | ./a.out 10 6 0 >> my_output.log && echo 'sdxssdswdxddddsxaadwxwdxwaa' | ./ref 10 6 0 >> ref_output.log
diff my_output.log ref_output.log
if [ $? -ne 0 ]; then
	echo "2. ❌ failed!"
else
	echo "2. ✅ passed!"
fi

echo -e "${BLUE}Test 3 - Small board with no iterations:${NC}"
echo 'dxss' | ./a.out 3 3 0 >> my_output.log && echo 'dxss' | ./ref 3 3 0 >> ref_output.log
diff my_output.log ref_output.log
if [ $? -ne 0 ]; then
	echo "3. ❌ failed!"
else
	echo "3. ✅ passed!"
fi

echo -e "${BLUE}Test 4 - Small board with one iteration:${NC}"
echo 'dxss' | ./a.out 3 3 1 >> my_output.log && echo 'dxss' | ./ref 3 3 1 >> ref_output.log
diff my_output.log ref_output.log
if [ $? -ne 0 ]; then
	echo "4. ❌ failed!"
else
	echo "4. ✅ passed!"
fi

echo -e "${BLUE}Test 5 - Small board with two iterations:${NC}"
echo 'dxss' | ./a.out 3 3 2 >> my_output.log && echo 'dxss' | ./ref 3 3 2 >> ref_output.log
diff my_output.log ref_output.log
if [ $? -ne 0 ]; then
	echo "5. ❌ failed!"
else
	echo "5. ✅ passed!"
fi

# Test 6: Block pattern (2x2 square) - should be stable across all iterations
echo -e "${BLUE}Test 6 - Block pattern (stable):${NC}"
echo -n 'dxdxsxsx' | ./a.out 4 4 5 > my_output.log && echo -n 'dxdxsxsx' | ./ref 4 4 5 > ref_output.log
diff my_output.log ref_output.log
if [ $? -ne 0 ]; then
	echo "6. ❌ failed!"
else
	echo "6. ✅ passed!"
fi

# Test 7: Empty board - should remain empty
echo -e "${BLUE}Test 7 - Empty board:${NC}"
echo -n '' | ./a.out 5 5 10 >> my_output.log && echo -n '' | ./ref 5 5 10 >> ref_output.log
diff my_output.log ref_output.log
if [ $? -ne 0 ]; then
	echo "7. ❌ failed!"
else
	echo "7. ✅ passed!"
fi

# Test 8: Single cell - should die immediately
echo -e "${BLUE}Test 8 - Single cell (dies):${NC}"
echo -n 'x' | ./a.out 3 3 5 >> my_output.log && echo -n 'x' | ./ref 3 3 5 >> ref_output.log
diff my_output.log ref_output.log
if [ $? -ne 0 ]; then
	echo "8. ❌ failed!"
else
	echo "8. ✅ passed!"
fi

# Test 9: Blinker pattern (oscillates every 2 iterations)
echo -e "${BLUE}Test 9 - Blinker (period 2):${NC}"
echo -n 'dxdxdx' | ./a.out 3 5 1 >> my_output.log && echo -n 'dxdxdx' | ./ref 3 5 1 >> ref_output.log
diff my_output.log ref_output.log
if [ $? -ne 0 ]; then
	echo "9. ❌ failed!"
else
	echo "9. ✅ passed!"
fi

# Test 10: Blinker after 2 iterations (should be back to original)
echo -e "${BLUE}Test 10 - Blinker after 2 iterations (returns to original):${NC}"
echo -n 'dxdxdx' | ./a.out 3 5 2 >> my_output.log && echo -n 'dxdxdx' | ./ref 3 5 2 >> ref_output.log
diff my_output.log ref_output.log
if [ $? -ne 0 ]; then
	echo "10. ❌ failed!"
else
	echo "10. ✅ passed!"
fi

# Test 11: Glider-like pattern with multiple iterations (larger board)
echo -e "${BLUE}Test 11 - Complex pattern on larger board (10x10, 5 iterations):${NC}"
echo 'ddxddxddxsxsxsxaa' | ./a.out 10 10 5 >> my_output.log && echo 'ddxddxddxsxsxsxaa' | ./ref 10 10 5 >> ref_output.log
diff my_output.log ref_output.log
if [ $? -ne 0 ]; then
	echo "11. ❌ failed!"
else
	echo "11. ✅ passed!"
fi

# Test 12: High iteration count
echo -e "${BLUE}Test 12 - High iteration count (stable pattern):${NC}"
echo -n 'dxdxsxsx' | ./a.out 4 4 100 >> my_output.log && echo -n 'dxdxsxsx' | ./ref 4 4 100 >> ref_output.log
diff my_output.log ref_output.log
if [ $? -ne 0 ]; then
	echo "12. ❌ failed!"
else
	echo "12. ✅ passed!"
fi

# Test 13: Rectangular board (non-square)
echo -e "${BLUE}Test 13 - Rectangular board (1x7):${NC}"
echo -n 'dxdxdx' | ./a.out 7 1 2 >> my_output.log && echo -n 'dxdxdx' | ./ref 7 1 2 >> ref_output.log
diff my_output.log ref_output.log
if [ $? -ne 0 ]; then
	echo "13. ❌ failed!"
else
	echo "13. ✅ passed!"
fi

# Test 14: All cells alive in small board
echo -e "${BLUE}Test 14 - All cells alive (dies):${NC}"
echo -n 'dxdxsxdxsxsx' | ./a.out 3 3 1 >> my_output.log && echo -n 'dxdxsxdxsxsx' | ./ref 3 3 1 >> ref_output.log
diff my_output.log ref_output.log
if [ $? -ne 0 ]; then
	echo "14. ❌ failed!"
else
	echo "14. ✅ passed!"
fi

# Summary
echo -e "${YELLOW}================================${NC}"
echo -e "${YELLOW}Checking diff results...${NC}"
echo -e "${YELLOW}================================${NC}"
diff my_output.log ref_output.log > /dev/null
if [ $? -eq 0 ]; then
	echo -e "${GREEN}All tests passed! ✨${NC}"
else
	echo -e "${RED}Some tests failed. Check output above.${NC}"
fi

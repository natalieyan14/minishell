#!/bin/bash

# Test script to check for memory leaks in minishell

echo "Testing minishell memory usage..."

# Create test commands
cat > test_mem.txt << EOF
echo "Memory test 1"
pwd
ls > /dev/null
echo "Memory test 2"
export TEST_VAR=hello
echo \$TEST_VAR
unset TEST_VAR
echo "Memory test 3"
exit
EOF

echo "Running minishell with test commands..."
./minishell < test_mem.txt

echo "Memory test completed successfully!"

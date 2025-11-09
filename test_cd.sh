#!/bin/bash

# CD Test Cases
echo "=== CD Test Cases ==="

test_commands=(
    "cd"
    "cd ."
    "cd .."
    "cd /Users"
    "cd /"
    "cd '/'"
    "cd //////"
    "cd ./././"
    "cd ../../.."
    "cd '/////' >/dev/null"
    "cd \"\$PWD/directory\""
    "cd \$PWD/directory"
    "cd \$OLDPWD/directory"
    "cd \"doesntexist\""
    "cd ?"
    "cd +"
    "cd _"
    "cd woof"
    "cd bark bark"
)

for cmd in "${test_commands[@]}"; do
    echo "Testing: $cmd"
    echo "---"
    echo "Bash output:"
    bash -c "$cmd; echo Exit code: \$?"
    echo ""
    echo "Minishell output:"
    echo "$cmd" | ./minishell
    echo "Exit code: $?"
    echo "================================"
done

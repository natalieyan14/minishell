#!/bin/bash

# ERROR Test Cases (try with echo $?)
echo "=== ERROR Test Cases ==="

test_commands=(
    "test"
    "echo > <"
    "<"
    "hello world"
    "cat wouaf wouaf"
    ">"
    "> > > >"
    ">> >> >> >>"
    "<<"
    "EechoE"
    ".echo."
    ">echo>"
    "<echo<"
    ">>echo>>"
    "rm -f something"
    "cat something"
    "your minishell is perfect"
    "cat < ls"
    "cat < ls > ls"
)

for cmd in "${test_commands[@]}"; do
    echo "Testing: $cmd"
    echo "---"
    echo "Bash output:"
    bash -c "$cmd; echo Exit code: \$?" 2>&1
    echo ""
    echo "Minishell output:"
    echo "$cmd" | ./minishell 2>&1
    echo "Exit code: $?"
    echo "================================"
done

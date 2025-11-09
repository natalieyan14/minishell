#!/bin/bash

# OTHER Test Cases
echo "=== OTHER Test Cases ==="

test_commands=(
    "export a=\"nenie'iri\""
    "echo \$a"
    "cat /dev/urandom | head -10"
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

echo ""
echo "Manual tests to perform:"
echo "1. Check leaks when interrupt from heredoc (CTRL-C)"
echo "2. Check SHLVL incrementation"

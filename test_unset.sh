#!/bin/bash

# UNSET Test Cases
echo "=== UNSET Test Cases ==="

test_commands=(
    "unset"
    "unset doesntexist"
    "unset PWD"
    "unset PWD"
    "unset OLDPWD"
    "unset PATH"
    "unset PATH"
    "unset PATH"
    "unset TES.T"
    "unset TES+T"
    "unset TES=T"
    "unset TES}T"
    "unset TES-T"
    "unset _TEST"
    "unset TES_T"
    "unset TEST_"
    "unset TE*ST"
    "unset TES#T"
    "unset TES@T"
    "unset TES\$?T"
    "unset ============"
    "unset +++++++"
    "unset ________"
    "unset export"
    "unset echo"
    "unset unset"
    "unset TES^T"
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

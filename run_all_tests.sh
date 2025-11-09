#!/bin/bash

# Master Test Runner
echo "=========================================="
echo "       MINISHELL COMPREHENSIVE TESTS"
echo "=========================================="

# Make sure minishell is compiled
if [ ! -f "./minishell" ]; then
    echo "Error: minishell executable not found. Compiling..."
    make
    if [ ! -f "./minishell" ]; then
        echo "Failed to compile minishell. Exiting."
        exit 1
    fi
fi

# Make all test scripts executable
chmod +x test_*.sh

echo "Starting comprehensive minishell tests..."
echo ""

# Run each test category
echo "Running CD tests..."
./test_cd.sh > results_cd.txt 2>&1

echo "Running ECHO tests..."
./test_echo.sh > results_echo.txt 2>&1

echo "Running EXIT tests..."
./test_exit.sh > results_exit.txt 2>&1

echo "Running EXPORT tests..."
./test_export.sh > results_export.txt 2>&1

echo "Running UNSET tests..."
./test_unset.sh > results_unset.txt 2>&1

echo "Running ERROR tests..."
./test_errors.sh > results_errors.txt 2>&1

echo "Running PIPES tests..."
./test_pipes.sh > results_pipes.txt 2>&1

echo "Running OTHER tests..."
./test_other.sh > results_other.txt 2>&1

echo ""
echo "=========================================="
echo "All tests completed!"
echo "Results saved in results_*.txt files"
echo "=========================================="
echo ""
echo "Quick summary of test files created:"
echo "- test_cd.sh: Change directory tests"
echo "- test_echo.sh: Echo command tests"  
echo "- test_exit.sh: Exit command tests"
echo "- test_export.sh: Export command tests"
echo "- test_unset.sh: Unset command tests"
echo "- test_errors.sh: Error handling tests"
echo "- test_pipes.sh: Pipe functionality tests"
echo "- test_other.sh: Miscellaneous tests"
echo ""
echo "To run individual tests:"
echo "./test_cd.sh"
echo "./test_echo.sh"
echo "etc..."

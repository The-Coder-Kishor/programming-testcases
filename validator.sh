#!/bin/bash

# Function to compile C files
compile_c_files() {
    gcc main.c matrix.c -o main
    if [ $? -eq 0 ]; then
        echo "Code compilation successful."
    else
        echo "Code compilation failed."
        exit 1
    fi
}

# Function to compare output to expected output
compare_output() {
    ./main < input0.txt > output0.txt
    if [ $? -eq 0 ]; then
        echo "Code execution successful."
    else
        echo "Code execution failed."
        exit 1
    fi
    actual_output=$(cat "./output0.txt")
    expected_output=$(cat "./expected_output0.txt")
    if [ "$actual_output" = "$expected_output" ]; then
        echo "Task 0 passed"
    else
        echo "Task 0 failed."
        exit 1
    fi
    rm output0.txt

    ./main < input1.txt > output1.txt
    if [ $? -eq 0 ]; then
        echo "Code execution successful."
    else
        echo "Code execution failed."
        exit 1
    fi
    actual_output=$(cat "./output1.txt")
    expected_output=$(cat "./expected_output1.txt")
    if [ "$actual_output" = "$expected_output" ]; then
        echo "Task 1 passed"
    else
        echo "Task 1 failed."
        exit 1
    fi
    rm output1.txt

    ./main < input2.txt > output2.txt
    if [ $? -eq 0 ]; then
        echo "Code execution successful."
    else
        echo "Code execution failed."
        exit 1
    fi
    actual_output=$(cat "./output2.txt")
    expected_output=$(cat "./expected_output2.txt")
    if [ "$actual_output" = "$expected_output" ]; then
        echo "Task 2 passed"
    else
        echo "Task 2 failed."
        exit 1
    fi
    rm output2.txt

    ./main < input3.txt > output3.txt
    if [ $? -eq 0 ]; then
        echo "Code execution successful."
    else
        echo "Code execution failed."
        exit 1
    fi
    actual_output=$(cat "./output3.txt")
    expected_output=$(cat "./expected_output3.txt")
    if [ "$actual_output" = "$expected_output" ]; then
        echo "Task 3 passed"
    else
        echo "Task 3 failed."
        exit 1
    fi
    rm output3.txt

    actual_output=$(cat "./mx_history")
    expected_output=$(cat "expected_mx_history")
    if [ "$actual_output" = "$expected_output" ]; then
        echo "mx_history passed"
    else
        echo "mx_history failed."
        exit 1
    fi
    rm mx_history

    actual_output=$(cat "./outputadd11.txt")
    expected_output=$(cat "./expected_outputadd11.txt")
    if [ "$actual_output" = "$expected_output" ]; then
        echo "Output Add 11 passed"
    else
        echo "Output Add 11 failed."
        exit 1
    fi
    rm outputadd11.txt

    actual_output=$(cat "./outputmult11.txt")
    expected_output=$(cat "./expected_outputmult11.txt")
    if [ "$actual_output" = "$expected_output" ]; then
        echo "Output Mult 11 passed"
    else
        echo "Output Mult 11 failed."
        exit 1
    fi
    rm outputmult11.txt

    actual_output=$(cat "./outputscalar1.txt")
    expected_output=$(cat "./expected_outputscalar1.txt")
    if [ "$actual_output" = "$expected_output" ]; then
        echo "Output Scalar 1 passed"
    else
        echo "Output Scalar 1 failed."
        exit 1
    fi
    rm outputscalar1.txt

    actual_output=$(cat "./outputscalar2.txt")
    expected_output=$(cat "./expected_outputscalar2.txt")
    if [ "$actual_output" = "$expected_output" ]; then
        echo "Output Scalar 2 passed"
    else
        echo "Output Scalar 2 failed."
        exit 1
    fi
    rm outputscalar2.txt

    actual_output=$(cat "./outputtranspose1.txt")
    expected_output=$(cat "./expected_outputtranspose1.txt")
    if [ "$actual_output" = "$expected_output" ]; then
        echo "Output Transpose 1 passed"
    else
        echo "Output Transpose 1 failed."
        exit 1
    fi
    rm outputtranspose1.txt
    
    actual_output=$(cat "./outputtranspose2.txt")
    expected_output=$(cat "./expected_outputtranspose2.txt")
    if [ "$actual_output" = "$expected_output" ]; then
        echo "Output Transpose 2 passed"
    else
        echo "Output Transpose 2 failed."
        exit 1
    fi
    rm outputtranspose2.txt
}

# Main script
compile_c_files
compare_output
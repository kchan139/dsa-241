#!/bin/bash

# Check if the executable 'main' already exists and remove it
if [ -f main ]; then
    echo "Removing existing executable 'main'."
    rm main
fi


# g++ -fsanitize=address -fsanitize=undefined -std=c++17 \
#     -o main -Iinclude -Itest -Isrc \
#     main.cpp \
#     test/unit_test/hash/unit_test.cpp test/unit_test/heap/unit_test.cpp \
#     -DTEST_HASH

# HASH
g++ -fsanitize=address -std=c++17 -I "./test"  -I "./include" -I "./include/tensor" -I "./include/sformat" -I "./include/ann" -I "./demo" $(find ./src/ann/ -type f -iregex ".*\.cpp") ./src/tensor/*\.cpp main.cpp test/unit_test/hash/unit_test.cpp -o main -DTEST_HASH

# ReLu
# g++ -fsanitize=address -std=c++17 -I "./test"  -I "./include" -I "./include/tensor" -I "./include/sformat" -I "./include/ann" -I "./demo" $(find ./src/ann/ -type f -iregex ".*\.cpp") ./src/tensor/*\.cpp main.cpp test/unit_test/layer/unit_test_relu.cpp -o main -DTEST_LAYER

#   Sigmoid
# g++ -fsanitize=address -std=c++17 -I "./test"  -I "./include" -I "./include/tensor" -I "./include/sformat" -I "./include/ann" -I "./demo" $(find ./src/ann/ -type f -iregex ".*\.cpp") ./src/tensor/*\.cpp main.cpp test/unit_test/layer/unit_test_sigmoid.cpp -o main -DTEST_LAYER

#   Softmax
# g++ -fsanitize=address -std=c++17 -I "./test"  -I "./include" -I "./include/tensor" -I "./include/sformat" -I "./include/ann" -I "./demo" $(find ./src/ann/ -type f -iregex ".*.cpp") ./src/tensor/*.cpp main.cpp test/unit_test/layer/unit_test_softmax.cpp -o main -DTEST_LAYER

#   Tanh
# g++ -fsanitize=address -std=c++17 -I "./test"  -I "./include" -I "./include/tensor" -I "./include/sformat" -I "./include/ann" -I "./demo" $(find ./src/ann/ -type f -iregex ".*.cpp") ./src/tensor/*.cpp main.cpp test/unit_test/layer/unit_test_tanh.cpp -o main -DTEST_LAYER

#   Loss
# g++ -fsanitize=address -std=c++17 -I "./test"  -I "./include" -I "./include/tensor" -I "./include/sformat" -I "./include/ann" -I "./demo" $(find ./src/ann/ -type f -iregex ".*.cpp") ./src/tensor/*.cpp main.cpp test/unit_test/loss/unit_test_CrossEntropy.cpp -o main -DTEST_LOSS
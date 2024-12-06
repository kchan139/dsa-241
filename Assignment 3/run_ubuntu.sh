clear
g++ -std=c++17 -o main -Iinclude -Itest -Isrc main.cpp test/unit_test/graph/unit_test.cpp -DTEST_GRAPH

if [ $? -eq 0 ]; then
    clear
    valgrind ./main test_unit graph01
fi
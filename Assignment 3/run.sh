clear
g++ -fsanitize=address -fsanitize=undefined -std=c++17 -o main -Iinclude -Itest -Isrc main.cpp test/unit_test/graph/unit_test.cpp -DTEST_GRAPH

if [ $? -eq 0 ]; then
    clear
    ./main test_unit
fi
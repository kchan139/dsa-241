clear
g++ -std=c++17 -o main -Iinclude -Itest -Isrc main.cpp test/unit_test/graph/unit_test.cpp -DTEST_GRAPH

if [ $? -eq 0 ]; then
    clear
    valgrind --leak-check=full ./main test_unit graph09
    # valgrind ./main test_unit graph10
    # valgrind ./main test_unit graph11
    # valgrind ./main test_unit graph12
    # valgrind ./main test_unit graph13
fi
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include "list/listheader.h"
using namespace std;

int main(int argc, char** argv) {
    cout << "Assignment-1" << endl;

    try
    {
        xlistDemo1();
        xlistDemo2();
        xlistDemo3();
        xlistDemo4();
        dlistDemo1();
        dlistDemo2();
        dlistDemo3();
        dlistDemo4();
        dlistDemo5();
        dlistDemo6();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    cout << "Testing XArrayList<int>..." << endl;
    test_XArrayList_int();
    cout << endl;

    cout << "Testing XArrayList<Point>..." << endl;
    test_XArrayList_Point();
    cout << endl;

    cout << "Testing XArrayList<Point*>..." << endl;
    test_XArrayList_PointPointer();
    cout << endl;
    
    cout << "Testing DLinkedList<int>..." << endl;
    test_DLinkedList_int();
    cout << endl;

    cout << "Testing DLinkedList<Point>..." << endl;
    test_DLinkedList_Point();
    cout << endl;

    cout << "Testing DLinkedList<Point*>..." << endl;
    test_DLinkedList_PointPointer();
    cout << endl;

    return 0;
}


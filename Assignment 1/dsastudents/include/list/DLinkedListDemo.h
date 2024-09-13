/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   DLinkedListDemo.h
 * Author: LTSACH
 *
 * Created on 19 August 2020, 21:34
 */

#ifndef DLINKEDLISTDEMO_H
#define DLINKEDLISTDEMO_H

#include <iostream>
#include <iomanip>
#include "list/DLinkedList.h"
#include "util/Point.h"
using namespace std;

void dlistDemo1(){
    List<int> dlist;
    for(int i = 0; i < 10 ; i++)
        dlist.add(i, i*i);
    dlist.println();
    
    for(List<int>::Iterator it=dlist.begin(); it != dlist.end(); it++ )
        cout << *it << " ";
    cout << endl;
}
void dlistDemo2(){
    DLinkedList<Point*> list1(&DLinkedList<Point*>::free, &Point::pointEQ);
    list1.add(new Point(23.2f, 25.4f));
    list1.add(new Point(24.6f, 23.1f));  
    list1.add(new Point(12.5f, 22.3f)); 
    
    for(DLinkedList<Point*>::Iterator it = list1.begin(); it != list1.end(); it++)
        cout << **it << endl;
    
    Point* p1 = new Point(24.6f, 23.1f); //found in list
    Point* p2 = new Point(124.6f, 23.1f); //not found
    cout << *p1 << "=> " << (list1.contains(p1)? "found; " : "not found; ")
                << " indexOf returns: " << list1.indexOf(p1) << endl;
    cout << *p2 << "=> " << (list1.contains(p2)? "found; " : "not found; ")
                << " indexOf returns: " << list1.indexOf(p2) << endl;
    
    ///Different results if not pass &Point::equals
    cout << endl << endl;
    DLinkedList<Point*> list2(&DLinkedList<Point*>::free);
    list2.add(new Point(23.2f, 25.4f));
    list2.add(new Point(24.6f, 23.1f));  
    list2.add(new Point(12.5f, 22.3f)); 
    
    for(DLinkedList<Point*>::Iterator it = list2.begin(); it != list2.end(); it++)
        cout << **it << endl;
    
    cout << *p1 << "=> " << (list2.contains(p1)? "found; " : "not found; ")
                << " indexOf returns: " << list2.indexOf(p1) << endl;
    cout << *p2 << "=> " << (list2.contains(p2)? "found; " : "not found; ")
                << " indexOf returns: " << list2.indexOf(p2) << endl;
    
    delete p1; delete p2;
}

void dlistDemo3(){
    DLinkedList<Point> dList;
    dList.add(Point(1.5, 3.5));
    dList.add(Point(2.5, 4.5));
    dList.add(Point(1.6, 3.1));
    
    cout << "test for indexOf: " << endl;
    Point p(1.6, 3.1);
    cout << p << " at: " << dList.indexOf(p);
}
bool pointComparator(Point*& p1, Point*& p2){
    return (p1->getX() == p2->getX()) && (p1->getY() == p2->getY());
}
string LpointPtr2Str(Point*& ptr){
    stringstream os;
    os << "("   << ptr->getX() << ", " 
                << ptr->getY()
       << ")";
    return os.str();
}
void dlistDemo4(){
    DLinkedList<Point*> dList(&DLinkedList<Point*>::free, &pointComparator);
    dList.add(new Point(1.5, 3.5));
    dList.add(new Point(2.5, 4.5));
    dList.add(new Point(1.6, 3.1));
    dList.println(&LpointPtr2Str);
    
    cout << "test for indexOf: " << endl;
    Point* p = new Point(1.6, 3.1);
    cout << *p << " at: " << dList.indexOf(p) << endl;
    delete p;
}
void dlistDemo5(){
    DLinkedList<float> dList;
    dList.add(3.2);
    dList.add(5.5);
    dList.println();
    cout << "index of 5.5: " << dList.indexOf(5.5) << endl;
    cout << "index of 15.5: " << dList.indexOf(15.5) << endl;
}

void dlistDemo6(){
    List<int> list;
    for(int i = 0; i< 10 ; i++)
        list.add(i, i*i);
    
    cout << setw(25) << left << "Original list: ";
    list.println();
    
    //
    int& item = list.get(5);
    item = 999;
    cout << setw(25) << left << "After changing an item: ";
    list.println();
}

// Unit tests for DLinkedList<int>
void test_DLinkedList_int() {
    DLinkedList<int> list;

    // Test add method
    list.add(1);
    list.add(2);
    list.add(3);
    printTestResult("Add elements", list.size() == 3 && list.get(0) == 1 && list.get(1) == 2 && list.get(2) == 3);

    // Test add at index method
    list.add(1, 99);
    printTestResult("Add element at index", list.get(1) == 99 && list.size() == 4);

    // Test removeAt method
    int removedItem = list.removeAt(1);
    printTestResult("Remove at index", removedItem == 99 && list.size() == 3 && list.get(1) == 2);

    // Test removeItem method
    bool removed = list.removeItem(2);
    printTestResult("Remove item", removed && list.size() == 2 && list.indexOf(2) == -1);

    // Test empty method
    DLinkedList<int> emptyList;
    printTestResult("Empty list", emptyList.empty() && !list.empty());

    // Test size method
    printTestResult("Size method", list.size() == 2);

    // Test clear method
    list.clear();
    printTestResult("Clear method", list.size() == 0 && list.empty());

    // Test indexOf and contains methods
    list.add(5);
    list.add(10);
    printTestResult("Index of method", list.indexOf(5) == 0 && list.indexOf(10) == 1);
    printTestResult("Contains method", list.contains(5) && !list.contains(99));

    // Test exception on invalid get index
    try {
        list.get(5);
        printTestResult("Exception on invalid index", false); // Should not reach this
    } catch (const std::out_of_range &) {
        printTestResult("Exception on invalid index", true);
    }

    // Test iterator
    list.clear();
    for (int i = 0; i < 3; ++i) list.add(i);
    int index = 0;
    bool iteratorTestPassed = true;
    for (DLinkedList<int>::Iterator it = list.begin(); it != list.end(); ++it) {
        if (*it != index++) iteratorTestPassed = false;
    }
    printTestResult("Iterator test", iteratorTestPassed);

    // Test assignment operator
    DLinkedList<int> assignedList;
    assignedList = list;
    printTestResult("Assignment operator", assignedList.size() == list.size() && assignedList.get(0) == list.get(0));
}

// Unit tests for DLinkedList<Point>
void test_DLinkedList_Point() {
    DLinkedList<Point> list;

    // Test add method
    list.add(Point(1.0f, 2.0f));
    list.add(Point(3.0f, 4.0f));
    printTestResult("Add Point elements", list.size() == 2 && list.get(0) == Point(1.0f, 2.0f));

    // Test indexOf and contains
    printTestResult("IndexOf Point", list.indexOf(Point(3.0f, 4.0f)) == 1);
    printTestResult("Contains Point", list.contains(Point(1.0f, 2.0f)));

    // Test iterator
    DLinkedList<Point>::Iterator it = list.begin();
    bool iteratorTestPassed = (*it == Point(1.0f, 2.0f));
    ++it;
    iteratorTestPassed = iteratorTestPassed && (*it == Point(3.0f, 4.0f));
    printTestResult("Iterator test (Point)", iteratorTestPassed);

    // Test clear
    list.clear();
    printTestResult("Clear method (Point)", list.size() == 0);
}

// Unit tests for DLinkedList<Point*>
void test_DLinkedList_PointPointer() {
    DLinkedList<Point*> list(&DLinkedList<Point*>::free, &Point::pointEQ);

    Point *p1 = new Point(1.0f, 2.0f);
    Point *p2 = new Point(3.0f, 4.0f);

    // Test add method
    list.add(p1);
    list.add(p2);
    printTestResult("Add Point* elements", list.size() == 2 && *(list.get(0)) == *p1);

    // Test contains
    printTestResult("Contains Point*", list.contains(p1) && !list.contains(new Point(5.0f, 6.0f)));

    // Test indexOf
    printTestResult("IndexOf Point*", list.indexOf(p2) == 1);

    // Test clear with custom delete
    list.clear();
    printTestResult("Clear method (Point*)", list.size() == 0);
}

#endif /* DLINKEDLISTDEMO_H */


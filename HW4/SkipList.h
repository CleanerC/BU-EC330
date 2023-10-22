//
//  SkipList.h
//  Skip_List
//
//  Copyright © Tali Moreshet. All rights reserved.
//
//
//  This is an implementation of a Skip List class, consisting of Linked Lists, which are made up of Nodes.
//  All classes are templated, such that their types may be provided at creation.
//  Nodes contain data (which doubles as key), and pointers to all directions.

#ifndef SkipList_h
#define SkipList_h

#include <iostream>
#include <cstdlib>
#include <stdlib.h>
#include <vector>

/* added */
#define MAXVAL 2147483647
#define MINVAL -2147483648

using namespace std;

template<class T>
class Node {
public:
    Node(const T& data);                            // constructor
    T data;                                         // node data, doubles as key
    Node<T> *next;                                  // node next pointer
    Node<T> *prev;                                  // node previous pointer
    Node<T> *up;                                    // node above pointer
    Node<T> *down;                                  // node below pointer
    void printData();                               // prints data value
    void print();                                   // prints entire node object content
};

template <class T>
class LinkedList {
public:
    LinkedList(T minVal, T maxVal);                 // constructor: accepts -infinity, +infinity values,
                                                    //  creates linked list with two corresponding nodes
    ~LinkedList();                                  // destructor
    Node<T>* search(Node<T>* location, T data);     // search for data starting at location, return node <= data
    Node<T>* insert(Node<T>* location, T data);     // insert node with data at new node to be placed directly 
                                                    //  after node with povided location, return new node
    void printData();                               // prints linked list data
    void print();                                   // prints linked list with all nodes pointers
    Node<T>* head;                                  // head of list

    /* added */
    Node<T>* tail;
};

template <class T>
class SkipList {
public:
    SkipList(T minVal, T maxVal);                   // constructor: accepts -infinity, +infinity values,
                                                    //  creates skip list with top-level only
    ~SkipList();                                    // destructor
    Node<T>* search(T data);                        // search for data, return node <= data (existing node, or location
                                                    //  where new node should be inserted in the bottom-most list)
    Node<T>* insert(T data);                            // insert node with data, return pointer if inserted, NULL if error
    void printData();                               // prints skip list data
    void print();                                   // prints skip list with all nodes pointers
    LinkedList<T> *topList;                         // pointer to the top-most list
    int randSeed = 330;                             // to be used as seed for the getRand() function

    /* added */
    vector<LinkedList<T>*> levels;
};

//returns 0 or 1 with a 50% chance 
//When it returns 1, insert the node to next level of the skiplist
int getRand(){
    return rand()%2;
}

/********************** From here down is the content of the LinkedList.cpp file: ***********************/

/****** Implementation of Node ******/

// Constructor
template<class T>
Node<T>::Node(const T& data)
{
    this->data = data;
    this->next = nullptr;
    this->prev = nullptr;
    this->up = nullptr;
    this->down = nullptr;
}

// Print node's data value
template <class T>
void Node<T>::printData()
{
    cout << data << " ";
}

// Print entire node object content (data value, object's address, and all pointers)
template <class T>
void Node<T>::print()
{
    cout << " * " << data << ": [addr: " << this << " next: " << this->next << " prev: " << this->prev << " up: " << this->up << " down: " << this->down << "]  ";
}


/****** Implementation of linked list ******/

template <class T>
LinkedList<T>::LinkedList(T minVal, T maxVal){
    head = new Node<T>(minVal);

    head->next = new Node<T>(maxVal);
    head->next->prev = head;

    tail = head->next;
}

//make a iterater, and a temp pointer. while the iterator is point to something. make the next one point to the iter->next, and free iterator, then make the iterator point to next
template<class T>
LinkedList<T>::~LinkedList(){
    Node<T> *iter = head;
    Node<T> *next;
    while(iter != nullptr){
        next = iter->next;
        delete iter;
        iter = next;
    }

}

//create a iterater, if will loop through the whole loop.
template<class T>
Node<T>* LinkedList<T>::search(Node<T> *location, T data){
    Node<T> *iter = location;
    while(iter != nullptr){
        if(iter->data == data || (iter->data < data && iter->next->data > data)){
            return iter;
        }
        iter = iter->next;
    }  
    return nullptr;
}

template<class T>
Node<T>* LinkedList<T>::insert(Node<T> *location, T data) {
    Node<T> *iter = search(location, data);
    if(iter == nullptr){
        return nullptr;
    }else {
        Node<T> *newNode = new Node<T>(data);
        newNode->next = iter->next;
        newNode->prev = iter;
        iter->next->prev = newNode;
        iter->next = newNode;
        return newNode;
    }
}


//create a iterator, point to every element in the LinkedList, can call printData on every node.
template<class T>
void LinkedList<T>::printData(){
    Node<T> *iter = head;
    while(iter != nullptr){
        iter->printData();
        iter = iter->next;
    }
    cout<<endl;
}

//create a iterator, point to every element in the LinkedList, can call pinrt on every Node
template<class T>
void LinkedList<T>::print(){
    Node<T> *iter = head;
    while(iter->next != nullptr){
        iter->print();
        iter = iter->next;
    }
    cout<<endl;
}




/****** Skip List Implementation ******/

   /*** TO BE COMPLETED ***/

template <class T>
SkipList<T>::SkipList(T minVal, T maxVal){
    topList = new LinkedList<T>(minVal, maxVal);
    levels.push_back(topList);
}

template <class T>
SkipList<T>::~SkipList(){
    /* to be implemneted */
}

template <class T>
Node<T>* SkipList<T>::search(T data){
    Node<T> *iter = topList->head;
    while(iter != nullptr){
        if(data < iter->next->data){
            iter = iter->down;
        } else {
            iter = iter->next;
        }
    }
    return iter;
}

template <class T>
Node<T>* SkipList<T>::insert(T data) {
    Node<T> *iter = new Node(data);
    Node<T> *temp = search(data);
    iter->next = temp->next;
    iter->prev = temp;
    temp->next->prev = iter;
    temp->next = iter;
    srand(randSeed);
    while(getRand() == 1){
        static int cnt = 2;
        if(cnt > level){
            LinkedList* newTop = new LinkedList<T>(MINVAL, MAXVAL);
            newTop->head = topList->head;
            newTop->tail = topList->tail;
            newTop->insert(newTop->head, data);
            Node<T> temp = newTop->search(newTop->head, data);
            temp->down = iter;
            iter->up = temp;
            cnt++;
        } else {
            
        }
    }
}


#endif /* SkipList_h */

// list.h 
//	Data structures to manage LISP-like lists.  
//
//      As in LISP, a list can contain any type of data structure
//	as an item on the list: thread control blocks, 
//	pending interrupts, etc.  That is why each item is a "void *",
//	or in other words, a "pointers to anything".
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#ifndef LIST_H
#define LIST_H

#include "copyright.h"
#include "utility.h"

// The following class defines a "list element" -- which is
// used to keep track of one item on a list.  It is equivalent to a
// LISP cell, with a "car" ("next") pointing to the next element on the list,
// and a "cdr" ("item") pointing to the item on the list.
//
// Internal data structures kept public so that List operations can
// access them directly.

class ListElement {
   public:
     ListElement(void *itemPtr, int sortKey);	// initialize a list element

     ListElement *next;		// next element on list, 
				// NULL if this is the last
     int key;		    	// priority, for a sorted list
     void *item; 	    	// pointer to item on the list
};

// The following class defines a "list" -- a singly linked list of
// list elements, each of which points to a single item on the list.
//
// By using the "Sorted" functions, the list can be kept in sorted
// in increasing order by "key" in ListElement.

class List {
  public:
    List();		// 初始化 List
    ~List();    // 析构函数 

    void Prepend(void *item); 	// 将 item 放到 List 首
    void Append(void *item); 	// 将 item 放到 List 尾
    void *Remove(); 	 // 将 item 从 List 首移除
    void Mapcar(VoidFunctionPtr func);	// 对 List 中每个元素应用 "func"
    bool IsEmpty();		// List 是否为空
    void RemoveItem(ListElement *tmp);  // 移除 List 中一个元素
    
    // Routines to put/get items on/off list in order (sorted by key)
    void SortedInsert(void *item, int sortKey);	// Put item into list
    void *SortedRemove(int *keyPtr); 	  	   // Remove first item from list
    ListElement *listFirst() { return first; }

  private:
    ListElement *first;  	// List 首，NULL 则为空
    ListElement *last;		// List 尾
};

#endif // LIST_H

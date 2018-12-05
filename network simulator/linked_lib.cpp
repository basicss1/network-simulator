#include <iostream>

#include "linked_lib.h"

using namespace std;

ListNode::ListNode()
{
    this->data = nullptr;
}

ListNode::~ListNode()
{
    if (this->data != nullptr) {
        //delete the allocated Datagram
        //   delete this;
        //delete  this->data;
        this->data = nullptr;
    }
}

void push_back(List& front, Datagram* data)
{
    // creates a new ListNode containing data
    // insert the new node to the end of the linked-list pointed by front
    //if front is nullptr, make front pt to new node
    ListNode ptr2 {};
    ptr2.data = data;
    ptr2.next = nullptr;
    List my_node = make_shared<ListNode>(ptr2);
    if (front == NULL) {
        front = my_node;
    }
    else {
        List current = front;
        while(current->next != nullptr) {
            current = current->next;
        }
        current->next = my_node;
    }

}

List pop_front(List& front)
{
    // removes the first node from linked-list front
    //returns the original first node
    //set the next of the original first node to nullptr
    List first_node = front;
    if(first_node == nullptr) {
        throw std::runtime_error("front cannot be a null pointer");
    }
    else if (first_node != nullptr) {
        front = front->next;
        first_node->next = nullptr;
    }
    return first_node;
}

void list_format(ostream& os, const List& front)
{
    if (front == nullptr) {
        os << "[]";
        return;
    }
    os << "[\n";
    List curr = front;
    while (curr != nullptr) {
        os << "      " << *curr->data;
        curr = curr->next;
        if (curr != nullptr)
            os << ",\n";
    }
    os << "\n    ]";
}

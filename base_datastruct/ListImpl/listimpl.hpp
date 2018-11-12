#ifndef __LISTIMPL_H__
#define __LISTIMPL_H__
#include <stdint.h>
#include <utility>
/*
 *  container used for multitask, must have push_back/pop_front and as far as faster
 *
 * */

template<class T> struct ListNode {
    T data;
    ListNode *next = nullptr;
    ListNode() = default;
    ~ListNode() = default;
    //constructed by args
    template<class ...Args> ListNode(Args &&... args): data(std::forward<Args>(args)...){}
    //copy-construct
    ListNode(const T &other): data(other) {}
    //move-construct
    ListNode(T &&other): data(std::forward<T>(other)) {}
};

template<class T> class List {
    ListNode<T> *head = nullptr, *tail = nullptr;
    uint64_t len = 0;

public:
    List() = default;
    ~List() {
        clear();
    }
    
    uint64_t size() const { return len; }
    //equal to emplace_back, struct 'ListNode' is unknown to user
    template<class ...Args> void push_back(Args &&... args) {
        ListNode<T> *newnode = new ListNode<T>(std::forward<Args>(args)...);
        if (!tail) {
            tail = newnode;
            head = newnode;
            len = 1;
        } else {
            tail->next = newnode;
            tail = newnode;
            ++len;
        }
    }
    template<class ...Args> void push_front(Args &&... args) {
        ListNode<T> *newnode = new ListNode<T>(std::forward<Args>(args)...);
        if (!head) {
            head = newnode;
            tail = newnode;
            len = 1;
        } else {
            newnode->next = head;
            head = newnode;
            ++len;
        }
    }
    void pop_front();
    T &front() const {
        return head->data;
    }
    void swap(List &other);
    void clear();
};

template<class T> void List<T>::pop_front () {
    if (head) {
        ListNode<T> *delnode = head;
        head = head->next;
        delete delnode;
        if (!head) {
            tail = nullptr;
        }
        --len;
    }
}

template<class T> void List<T>::swap (List<T> &other) {
    ListNode<T> *tmp = head;
    head = other.head;
    other.head = tmp;

    tmp = tail;
    tail = other.tail;
    other.tail = tmp;

    uint64_t tmpsize = len;
    len = other.len;
    other.len = tmpsize;
}

template<class T> void List<T>::clear () {
    ListNode<T> *cur = head;
    while (cur) {
        ListNode<T> *next = cur->next;
        delete cur;
        cur = next;
    }

    head = nullptr;
    tail = nullptr;
    len = 0;
}

#endif

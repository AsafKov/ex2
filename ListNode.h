#ifndef EX1_NODE_H
#define EX1_NODE_H

#include <cmath>

template <typename Data>
class ListNode {
private:
    Data data;
    ListNode<Data>* next = nullptr;
    ListNode<Data>* prev = nullptr;

public:
    explicit ListNode<Data>(Data const &val) : data(val), next(nullptr),
                                               prev(nullptr) {};
    ~ListNode() = default;
    Data &getData() { return this->data; }
    ListNode *getNext() const { return this->next; }
    ListNode *getPrev() const { return this->prev; }
    void setData(Data &nodeData){
        this->data=nodeData;
    }
    void setNext(ListNode* const nextNode){
        this->next = nextNode;
        if(nextNode != nullptr){
            nextNode->setPrev(this);
        }
    }

    void setPrev(ListNode* const prevNode){
        this->prev = prevNode;
        if(prevNode != nullptr){
            prevNode->setNext(this);
        }
    }

    void clearNode(){
        this->next = nullptr;
        this->prev = nullptr;
    }
};


#endif //EX1_NODE_H

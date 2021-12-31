#ifndef EX1_NODE_H
#define EX1_NODE_H

#include <cmath>

template <typename Data>
class Node {
private:
    Data data;
    Node<Data>* next = nullptr;
    Node<Data>* prev = nullptr;

public:
    explicit Node<Data>(Data const &val) : data(val), next(nullptr),
                                        prev(nullptr) {};
    ~Node() = default;
    Data &getData() { return this->data; }
    Node *getNext() const { return this->next; }
    Node *getPrev() const { return this->prev; }
    void setData(Data &nodeData){
        this->data=nodeData;
    }
    void setNext(Node* const nextNode){
        this->next = nextNode;
        if(nextNode != nullptr){
            nextNode->setPrev(this);
        }
    }

    void setPrev(Node* const prevNode){
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

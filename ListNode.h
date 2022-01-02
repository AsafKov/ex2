#ifndef EX1_NODE_H
#define EX1_NODE_H

#include <cmath>

template <typename Key, typename Data>
class ListNode {
private:
    Key key;
    Data data;
    ListNode<Key, Data>* next = nullptr;
    ListNode<Key, Data>* prev = nullptr;

public:
    explicit ListNode<Key, Data>(Key const &key, Data const &val) : key(key), data(val), next(nullptr),
                                               prev(nullptr) {};
    ~ListNode() = default;
    Data &getData() { return this->data; }
    Key &getKey() { return this->key; }
    ListNode *getNext() const { return this->next; }
    ListNode *getPrev() const { return this->prev; }
    void setData(Data &nodeData){
        this->data=nodeData;
    }
    void setNext(ListNode* const nextNode){
        this->next = nextNode;
    }

    void setPrev(ListNode* const prevNode){
        this->prev = prevNode;
    }

    void clearNode(){
        this->next = nullptr;
        this->prev = nullptr;
    }
};


#endif //EX1_NODE_H

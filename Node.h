#ifndef EX1_NODE_H
#define EX1_NODE_H

#include <cmath>

template <typename Key, typename Data>
class Node {
private:
    const Key key;
    Data data;
    Node<Key, Data>* left = nullptr;
    Node<Key, Data>* right = nullptr;
    Node<Key, Data>* father = nullptr;

public:
    int height, balancingParameter;
    explicit Node<Key, Data>(Key const &k, Data const &val) : key(k), data(val), left(nullptr),
                                        right(nullptr), father(nullptr), height(0), balancingParameter(0) {};
    ~Node() = default;
    Data &getData() { return this->data; }
    const Key &getKey() const { return this->key; }
    Node *getLeft() const { return this->left; }
    Node *getRight() const { return this->right; }
    void setData(Data &nodeData){
        this->data=nodeData;
    }
    void setLeft(Node* const leftNode){
        this->left = leftNode;
        if(leftNode != nullptr){
            leftNode->setFather(this);
        }
        this->calculateHeightAndBalance();
    }
    void setRight(Node* const rightNode){
        this->right = rightNode;
        if(rightNode != nullptr){
            rightNode->setFather(this);
        }
        this->calculateHeightAndBalance();
    }

    void clearNode(){
        this->left = nullptr;
        this->right = nullptr;
        this->father = nullptr;
    }

    void setFather(Node<Key, Data>* const prev){ this->father = prev;}
    void calculateHeightAndBalance(){
        if(this->getLeft() == nullptr && this->getRight() == nullptr) {
            this->balancingParameter = 0;
            this->height = 0;
            return;
        }
        int heightLeft = this->getLeft() == nullptr? 0:this->getLeft()->height;
        int heightRight = this->getRight() == nullptr? 0:this->getRight()->height;
        this->height = fmax((double) heightRight, (double) heightLeft)+1;
        this->balancingParameter = heightLeft - heightRight;
    }
    Node<Key, Data> *getFather() const { return this->father; }
};


#endif //EX1_NODE_H

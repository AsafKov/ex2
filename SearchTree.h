#ifndef EX1_SEARCHTREE_H
#define EX1_SEARCHTREE_H

#include <iostream>
#include "Node.h"
#include "cmath"

template<typename Key, typename Data>
class SearchTree {
private:
    Node<Key, Data> *root;
    int size;

    void findUnbalance(Node<Key, Data> *currentNode) {
        bool balancingPnt = false;
        while (!balancingPnt) {
            currentNode->calculateHeightAndBalance();
            if (abs(currentNode->balancingParameter) == 2) {
                balancingPnt = true;
            } else {
                if (currentNode != this->root) {
                    currentNode = currentNode->getFather();
                } else {
                    break;
                }
            }
        }
        balanceTree(currentNode);
    }

    void balanceTree(Node<Key, Data> *balancingPnt) {
        if (balancingPnt == nullptr) return;
        switch (balancingPnt->balancingParameter) {
            case 2: {
                if (balancingPnt->getLeft()->balancingParameter >= 0) {
                    roll_LL(balancingPnt);
                } else {
                    roll_LR(balancingPnt);
                }
            }
                break;

            case -2: {
                if (balancingPnt->getRight()->balancingParameter <= 0) {
                    roll_RR(balancingPnt);
                } else {
                    roll_RL(balancingPnt);
                }
            }
        }
    }

    void clearPostOrder(Node<Key, Data> *currentRoot) {
        if (currentRoot == nullptr) {
            return;
        }

        clearPostOrder(currentRoot->getRight());
        clearPostOrder(currentRoot->getLeft());

        currentRoot->clearNode();
    }

    int scanInOrder(Node<Key, Data> *node, Node<Key, Data> ***sortedArr, int index) {
        if (index == this->size || node == nullptr) {
            return index;
        }
        index = scanInOrder(node->getLeft(), sortedArr, index);
        (*sortedArr)[index] = node;
        index++;
        index = scanInOrder(node->getRight(), sortedArr, index);
        return index;
    }

    int scanInOrderLimited(Node<Key, Data> *node, Node<Key, Data> ***sortedArr, int index, int stop) {
        if (index == stop || node == nullptr) {
            return index;
        }
        index = scanInOrderLimited(node->getLeft(), sortedArr, index, stop);
        if(index == stop){
            return index;
        }
        (*sortedArr)[index] = node;
        index++;
        index = scanInOrderLimited(node->getRight(), sortedArr,index, stop);
        return index;
    }

    void roll_LL(Node<Key, Data> *balancingPnt) {
        rightRotate(balancingPnt);
    }

    void roll_RR(Node<Key, Data> *balancingPnt) {
        leftRotate(balancingPnt);
    }

    void roll_LR(Node<Key, Data> *balancingPnt) {
        leftRotate(balancingPnt->getLeft());
        rightRotate(balancingPnt);
    }

    void roll_RL(Node<Key, Data> *balancingPnt) {
        rightRotate(balancingPnt->getRight());
        leftRotate(balancingPnt);
    }

    void leftRotate(Node<Key, Data> *node) {
        Node<Key, Data> *father = node->getFather();
        Node<Key, Data> *rightSon = node->getRight();
        Node<Key, Data> *leftOfRight = rightSon->getLeft();

        if (father == nullptr) {
            setRoot(rightSon);
        } else {
            if (father->getRight() == node) {
                father->setRight(rightSon);
            } else {
                father->setLeft(rightSon);
            }
            father->calculateHeightAndBalance();
        }


        rightSon->setLeft(node);
        node->setRight(leftOfRight);

        node->calculateHeightAndBalance();
        rightSon->calculateHeightAndBalance();
    }

    void rightRotate(Node<Key, Data> *node) {
        Node<Key, Data> *father = node->getFather();
        Node<Key, Data> *leftSon = node->getLeft();
        Node<Key, Data> *rightOfLeft = leftSon->getRight();

        if (father == nullptr) {
            setRoot(leftSon);
        } else {
            if (father->getRight() == node) {
                father->setRight(leftSon);
            } else {
                father->setLeft(leftSon);
            }
            father->calculateHeightAndBalance();
        }

        leftSon->setRight(node);
        node->setLeft(rightOfLeft);

        node->calculateHeightAndBalance();
        leftSon->calculateHeightAndBalance();
    }

    Node<Key, Data> *findMin(Node<Key, Data> *node) {
        auto *temp = node->getLeft();
        while (temp != nullptr) {
            node = node->getLeft();
            temp = temp->getLeft();
        }
        return node;
    }

    void removeNoChildren(Node<Key, Data> *node, Node<Key, Data> *father);

    Node<Key, Data> *removeTwoChildren(Node<Key, Data> *node, Node<Key, Data> *father);

    void removeOneChildRight(Node<Key, Data> *node, Node<Key, Data> *father);

    void deleteTree(Node<Key, Data> *node, Node<Key, Data> *father);

    bool isBalanced(Node<Key, Data> *node, bool carry);

    void setRoot(Node<Key, Data> *newRoot) {
        this->root = newRoot;
        if (newRoot != nullptr) {
            newRoot->setFather(nullptr);
        }
    }

    Node<Key, Data> *buildFromSortedArray(Node<Key, Data> **array, int indexLeft, int indexRight);

public:
    SearchTree &operator=(const SearchTree &tree) = delete;

    SearchTree(const SearchTree &tree) = delete;

    SearchTree() : root(nullptr), size(0) {};

    ~SearchTree() {
        deleteTree(this->root, nullptr);
        this->root = nullptr;
    }

    Node<Key, Data> *find(Key const &key);

    void remove(Key const &key);

    void insert(Node<Key, Data> *newNode);

    void insert(Key &key, Data &data); //For Testing
    void scanInOrder(Node<Key, Data> ***sortedArr);
    void scanInOrderLimited(Node<Key, Data> ***sortedArr, int stop);

    Node<Key, Data> *findLeftmost(Node<Key, Data> *node);

    Node<Key, Data> *findRightmost(Node<Key, Data> *node);

    int getSize() const { return this->size; }

    void removeOneChildLeft(Node<Key, Data> *node, Node<Key, Data> *father);

    bool isBalanced();

    void clearTree();

    void mergeWith(Node<Key, Data> **toMergeNodes, int toMergeSize);
    Node<Key, Data>* getRoot(){
        return this->root;
    }

};

template<typename Key, typename Data>
void SearchTree<Key, Data>::insert(Node<Key, Data> *newNode) {
    auto *temp = root;
    bool leafAdded = false;

    if (temp == nullptr) {
        this->root = newNode;
        this->size++;
        return;
    }
    while (!leafAdded) {
        if (temp == newNode) {
            return;
        }
        if (newNode->getKey() > temp->getKey()) {
            if (temp->getRight() == nullptr) {
                temp->setRight(newNode);
                leafAdded = true;
            } else {
                temp = temp->getRight();
            }
        } else {
            if (temp->getLeft() == nullptr) {
                temp->setLeft(newNode);
                leafAdded = true;
            } else {
                temp = temp->getLeft();
            }
        }
    }
    this->size++;
    findUnbalance(temp);
}

template<typename Key, typename Data>
void SearchTree<Key, Data>::scanInOrder(Node<Key, Data> ***sortedArr) {
    scanInOrder(this->root, sortedArr, 0);
}

template<typename Key, typename Data>
void SearchTree<Key, Data>::scanInOrderLimited(Node<Key, Data> ***sortedArr, int stop) {
    scanInOrderLimited(this->root, sortedArr, 0, stop);
}

template<typename Key, typename Data>
Node<Key, Data> *SearchTree<Key, Data>::find(Key const &key) {
    auto *node = this->root;
    while (node != nullptr) {
        if (node->getKey() == key) {
            return node;
        }
        if (key < node->getKey()) {
            node = node->getLeft();
        } else {
            node = node->getRight();
        }
    }
    return nullptr;
}



template<typename Key, typename Data>
void SearchTree<Key, Data>::remove(Key const &key) {
    Node<Key, Data> *node = find(key);
    if (node == nullptr) {
        return;
    }
    auto *father = node->getFather();

    //no children
    if ((node->getLeft() == nullptr) && (node->getRight() == nullptr)) {
        removeNoChildren(node, father);
    }

    //one child on left
    if ((node->getLeft() != nullptr) && (node->getRight() == nullptr)) {
        removeOneChildLeft(node, father);
    }

    //one child on right
    if ((node->getLeft() == nullptr) && (node->getRight() != nullptr)) {
        removeOneChildRight(node, father);
    }

    //two children
    if ((node->getLeft() != nullptr) && (node->getRight() != nullptr)) {
        father = removeTwoChildren(node, father);
    }

    // Re-balance
    if (father == nullptr) {
        father = this->root;
        if (father == nullptr) {
            this->size = 0;
            delete node;
            return;
        }
        balanceTree(father->getRight());
        balanceTree(father->getLeft());
    }
    while (father != nullptr) {
        father->calculateHeightAndBalance();
        balanceTree(father);
        father = father->getFather();
    }
    delete node;
    this->size -= 1;
}



template<typename Key, typename Data>
void SearchTree<Key, Data>::removeNoChildren(Node<Key, Data> *node, Node<Key, Data> *father) {
    if (father == nullptr) {
        this->root = nullptr;
        return;
    }
    if (father->getLeft() == node) {
        father->setLeft(nullptr);
    } else {
        father->setRight(nullptr);
    }
    father->calculateHeightAndBalance();
    node->setFather(nullptr);
    node->setRight(nullptr);
}

template<typename Key, typename Data>
void SearchTree<Key, Data>::removeOneChildLeft(Node<Key, Data> *node, Node<Key, Data> *father) {
    Node<Key, Data> *leftSon = node->getLeft();
    if (father == nullptr) {
        setRoot(leftSon);
        return;
    } else {
        if (father->getLeft() == node) {
            father->setLeft(leftSon);
        } else {
            father->setRight(leftSon);
        }
        father->calculateHeightAndBalance();
    }

    node->setFather(nullptr);
    node->setLeft(nullptr);
}

template<typename Key, typename Data>
void SearchTree<Key, Data>::removeOneChildRight(Node<Key, Data> *node, Node<Key, Data> *father) {
    Node<Key, Data> *rightSon = node->getRight();
    if (father == nullptr) {
        setRoot(rightSon);
    } else {
        if (father->getLeft() == node) {
            father->setLeft(rightSon);
        } else {
            father->setRight(rightSon);
        }
    }

    node->setFather(nullptr);
    node->setRight(nullptr);
}

template<typename Key, typename Data>
Node<Key, Data> *SearchTree<Key, Data>::removeTwoChildren(Node<Key, Data> *node, Node<Key, Data> *father) {
    Node<Key, Data> *nextInOrder = findMin(node->getRight());
    Node<Key, Data> *fatherNextInOrder = nextInOrder->getFather();
    Node<Key, Data> *tempLeft = node->getLeft();
    Node<Key, Data> *tempRight = node->getRight();

    if (father == nullptr) {
        setRoot(nextInOrder);
    } else {
        if (father->getLeft() == node) {
            father->setLeft(nextInOrder);
        } else {
            father->setRight(nextInOrder);
        }
        father->calculateHeightAndBalance();
    }

    if (fatherNextInOrder->getRight() == nextInOrder) {
        nextInOrder->setLeft(node);
        node->setRight(nullptr);
        removeOneChildLeft(node, nextInOrder);
        return nextInOrder;
    } else {
        fatherNextInOrder->setLeft(node);
    }

    node->setRight(nextInOrder->getRight());
    node->setLeft(nullptr);
    nextInOrder->setLeft(tempLeft);
    nextInOrder->setRight(tempRight);

    if (node->getRight() != nullptr) {
        removeOneChildRight(node, node->getFather());
    } else {
        removeNoChildren(node, node->getFather());
    }

    return fatherNextInOrder;
}

template<typename Key, typename Data>
void SearchTree<Key, Data>::deleteTree(Node<Key, Data> *node, Node<Key, Data> *father) {
    if (node == nullptr) {
        return;
    }
    deleteTree(node->getLeft(), node);
    deleteTree(node->getRight(), node);

    if (father != nullptr) {
        if (father->getRight() == node) {
            father->setRight(nullptr);
        } else {
            father->setLeft(nullptr);
        }
    }

    delete node;
}

template<typename Key, typename Data>
bool SearchTree<Key, Data>::isBalanced(Node<Key, Data> *node, bool carry) {
    if (node == nullptr) return true;

    carry = isBalanced(node->getLeft(), carry) && carry;
    carry = isBalanced(node->getRight(), carry) && carry;

    node->calculateHeightAndBalance();

    return carry && abs(node->balancingParameter) < 2;
}

template<typename Key, typename Data>
bool SearchTree<Key, Data>::isBalanced() {
    return isBalanced(this->root, true);
}

template<typename Key, typename Data>
void SearchTree<Key, Data>::insert(Key &key, Data &data) {
    Node<Key, Data> *newNode = new Node<Key, Data>(key, data);
    insert(newNode);
}

template<typename Key, typename Data>
void SearchTree<Key, Data>::mergeWith(Node<Key, Data> **toMergeNodes, int toMergeSize) {
    auto **ownNodes = new Node<Key, Data>*[size];
    scanInOrder(&ownNodes);
    int mergedSize = this->size + toMergeSize;
    auto **sortedArr = new Node<Key, Data> *[mergedSize];
    int indexToMerge = 0, indexOwn = 0;
    int currentIndex = 0;
    //int minSize = (int) fmin(toMergeSize, this->getSize());
    while (indexToMerge < toMergeSize && indexOwn < this->size) {
        if (toMergeNodes[indexToMerge]->getKey() < ownNodes[indexOwn]->getKey()) {
            sortedArr[currentIndex] = toMergeNodes[indexToMerge];
            indexToMerge++;
        } else {
            sortedArr[currentIndex] = ownNodes[indexOwn];
            indexOwn++;
        }
        currentIndex++;
    }

    while (indexToMerge < toMergeSize) {
        sortedArr[currentIndex] = toMergeNodes[indexToMerge];
        indexToMerge++;
        currentIndex++;
    }

    while (indexOwn < this->getSize()) {
        sortedArr[currentIndex] = ownNodes[indexOwn];
        indexOwn++;
        currentIndex++;
    }

    for(int i=0; i<mergedSize; i++){
        sortedArr[i]->setLeft(nullptr);
        sortedArr[i]->setRight(nullptr);
        sortedArr[i]->setFather(nullptr);
    }

    setRoot(buildFromSortedArray(sortedArr, 0, mergedSize - 1));
    this->size = mergedSize;
    delete[] ownNodes;
    delete[] sortedArr;
}

template<typename Key, typename Data>
Node<Key, Data> *SearchTree<Key, Data>::buildFromSortedArray(Node<Key, Data> **array, int indexLeft, int indexRight) {
    if (indexLeft > indexRight) {
        return nullptr;
    }

    int mid = (indexLeft + indexRight) / 2;
    Node<Key, Data> *currentRoot = array[mid];

    currentRoot->setLeft(buildFromSortedArray(array, indexLeft, mid - 1));

    currentRoot->setRight(buildFromSortedArray(array, mid + 1, indexRight));

    return currentRoot;
}

template<typename Key, typename Data>
Node<Key, Data> *SearchTree<Key, Data>::findLeftmost(Node<Key, Data> *node){
    if(node == nullptr) return node;
    while(node->getLeft() != nullptr){
        node = node->getLeft();
    }

    return node;
}

template<typename Key, typename Data>
Node<Key, Data> *SearchTree<Key, Data>::findRightmost(Node<Key, Data> *node){
    if(node == nullptr) return node;
    while(node->getRight() != nullptr){
        node = node->getRight();
    }

    return node;
}

template<typename Key, typename Data>
void SearchTree<Key, Data>::clearTree() {
    clearPostOrder(root);
    root = nullptr;
}

#endif //EX1_SEARCHTREE_H

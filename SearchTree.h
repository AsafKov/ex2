#ifndef EX1_SEARCHTREE_H
#define EX1_SEARCHTREE_H
#include "Node.h"

template<typename Key>
class SearchTree {
private:
    Node<Key> *root;
    int size;

    void findUnbalance(Node<Key> *currentNode) {
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

    void HistScorePostInsert(Node<Key> *currentNode)
    {
        int scr = currentNode->getPlayer()->getScore();
        int sumLevels = currentNode->getSumLevel();
        currentNode->increaseScore(scr);
        currentNode->addHist(currentNode->getLeft()->getScoreHist());
        currentNode->addHist(currentNode->getRight()->getScoreHist());
        currentNode->increaseSumLevel(currentNode->getLeft()->getSumLevel());
        currentNode->increaseSumLevel(currentNode->getRight()->getSumLevel());
        auto father=currentNode->getFather();
        while (father!= nullptr)
        {
            scr = currentNode->getPlayer()->getScore();
            sumLevels = currentNode->getSumLevel();
            father->increaseScore(scr);
            father->increaseSumLevel(sumLevels);
            currentNode=father;
            father=father->getFather();
        }
    }

    void HistScorePostRemove(Node<Key> *currentNode, int score, int sumLevel)
    {
        currentNode->decreaseScore(score);
        currentNode->decreaseSumLevel(sumLevel);
        auto father=currentNode->getFather();
        while (father!= nullptr)
        {
            score = currentNode->getPlayer()->getScore();
            sumLevel = currentNode->getSumLevel();
            father->decreaseScore(score);
            father->decreaseSumLevel(sumLevel);
            currentNode=father;
            father=father->getFather();
        }
    }

    void histPreLL(Node<Key> *balancingPnt)
    {
        balancingPnt->subtractHist(balancingPnt->getLeft()->getScoreHist());
        balancingPnt->decreaseSumLevel(balancingPnt->getLeft()->getSumLevel());
        balancingPnt->addHist(balancingPnt->getLeft()->getRight()->getScoreHist());
        balancingPnt->increaseSumLevel(balancingPnt->getLeft()->getRight()->getSumLevel());

        if (balancingPnt->getRight()!=nullptr)
        {
            balancingPnt->getLeft()->addHist(balancingPnt->getRight()->getScoreHist());
            balancingPnt->getLeft()->increaseSumLevel(balancingPnt->getRight()->getSumLevel());

        }
        balancingPnt->getLeft()->increaseScore(balancingPnt->getPlayer()->getScore());
        balancingPnt->getLeft()->increaseSumLevel(balancingPnt->getPlayer()->getSumLevel());
    }

    void histPreRR(Node<Key> *balancingPnt)
    {
        balancingPnt->subtractHist(balancingPnt->getRight()->getScoreHist());
        balancingPnt->decreaseSumLevel(balancingPnt->getRight()->getSumLevel());
        balancingPnt->addHist(balancingPnt->getRight()->getLeft()->getScoreHist());
        balancingPnt->increaseSumLevel(balancingPnt->getRight()->getLeft()->getSumLevel());

        if (balancingPnt->getLeft()!=nullptr)
        {
            balancingPnt->getRight()->addHist(balancingPnt->getLeft()->getScoreHist());
            balancingPnt->getRight()->increaseSumLevel(balancingPnt->getLeft()->getSumLevel());
        }
        balancingPnt->getRight()->increaseScore(balancingPnt->getPlayer()->getScore());
        balancingPnt->getRight()->increaseSumLevel(balancingPnt->getPlayer()->getSumLevel());
    }

    void balanceTree(Node<Key> *balancingPnt) {
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

    void clearPostOrder(Node<Key> *currentRoot) {
        if (currentRoot == nullptr) {
            return;
        }

        clearPostOrder(currentRoot->getRight());
        clearPostOrder(currentRoot->getLeft());

        currentRoot->clearNode();
    }

    int setHistInOrder(Node<Key> *node, int index) {
        if (index == this->size || node == nullptr) {
            return index;
        }
        index = setHistInOrder(node->getLeft(), index);
        node->increaseScore(node->getPlayer()->getScore());
        node->increaseSumLevel(node->getPlayer()->getSumLevel());
        if (node->getLeft()!=nullptr)
        {
            node->addHist(node->getLeft()->getScoreHist());
            node->increaseSumLevel(node->getLeft()->getSumLevel());
        }
        if (node->getRight()!=nullptr)
        {
            node->addHist(node->getRight()->getScoreHist());
            node->increaseSumLevel(node->getRight()->getSumLevel());
        }
        index++;
        index = setHistInOrder(node->getRight(), index);
        return index;
    }

    int scanInOrder(Node<Key> *node, Node<Key> ***sortedArr, int index) {
        if (index == this->size || node == nullptr) {
            return index;
        }
        index = scanInOrder(node->getLeft(), sortedArr, index);
        (*sortedArr)[index] = node;
        index++;
        index = scanInOrder(node->getRight(), sortedArr, index);
        return index;
    }

    int scanInOrderLimited(Node<Key> *node, Node<Key> ***sortedArr, int index, int stop) {
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

    void roll_LL(Node<Key> *balancingPnt) {
        histPreLL(balancingPnt);
        rightRotate(balancingPnt);
    }

    void roll_RR(Node<Key> *balancingPnt) {
        histPreRR(balancingPnt);
        leftRotate(balancingPnt);
    }

    void roll_LR(Node<Key> *balancingPnt) {
        histPreRR(balancingPnt);
        leftRotate(balancingPnt->getLeft());
        histPreLL(balancingPnt);
        rightRotate(balancingPnt);
    }

    void roll_RL(Node<Key> *balancingPnt) {
        histPreLL(balancingPnt);
        rightRotate(balancingPnt->getRight());
        histPreRR(balancingPnt);
        leftRotate(balancingPnt);
    }

    void leftRotate(Node<Key> *node) {
        Node<Key> *father = node->getFather();
        Node<Key> *rightSon = node->getRight();
        Node<Key> *leftOfRight = rightSon->getLeft();

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

    void rightRotate(Node<Key> *node) {
        Node<Key> *father = node->getFather();
        Node<Key> *leftSon = node->getLeft();
        Node<Key> *rightOfLeft = leftSon->getRight();

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

    Node<Key> *findMin(Node<Key> *node) {
        auto *temp = node->getLeft();
        while (temp != nullptr) {
            node = node->getLeft();
            temp = temp->getLeft();
        }
        return node;
    }

    void removeNoChildren(Node<Key> *node, Node<Key> *father);

    Node<Key> *removeTwoChildren(Node<Key> *node, Node<Key> *father);

    void removeOneChildRight(Node<Key> *node, Node<Key> *father);

    void deleteTree(Node<Key> *node, Node<Key> *father);

    bool isBalanced(Node<Key> *node, bool carry);

    void setRoot(Node<Key> *newRoot) {
        this->root = newRoot;
        if (newRoot != nullptr) {
            newRoot->setFather(nullptr);
        }
    }

    Node<Key> *buildFromSortedArray(Node<Key> **array, int indexLeft, int indexRight);

public:
    SearchTree &operator=(const SearchTree &tree) = delete;

    SearchTree(const SearchTree &tree) = delete;

    SearchTree() : root(nullptr), size(0) {};

    ~SearchTree() {
        deleteTree(this->root, nullptr);
        this->root = nullptr;
    }

    Node<Key> *find(Key const &key);

    void remove(Key const &key);

    void insert(Node<Key> *newNode);

    void insert(Key &key, Player &player); //For Testing
    void scanInOrder(Node<Key> ***sortedArr);
    void scanInOrderLimited(Node<Key> ***sortedArr, int stop);

    Node<Key> *findLeftmost(Node<Key> *node);

    Node<Key> *findRightmost(Node<Key> *node);

    int getSize() const { return this->size; }

    void removeOneChildLeft(Node<Key> *node, Node<Key> *father);

    bool isBalanced();

    void clearTree();

    void mergeWith(Node<Key> **toMergeNodes, int toMergeSize);
    Node<Key>* getRoot(){
        return this->root;
    }

    Node<Key>* getHistScore(Key const &key);

    double SearchTree<Key>::findM();
};

template<typename Key>
void SearchTree<Key>::insert(Node<Key> *newNode) {
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
    auto cur=temp;
    HistScorePostInsert(cur);
    findUnbalance(temp);
}

template<typename Key>
void SearchTree<Key>::scanInOrder(Node<Key> ***sortedArr) {
    scanInOrder(this->root, sortedArr, 0);
}

template<typename Key>
void SearchTree<Key>::scanInOrderLimited(Node<Key> ***sortedArr, int stop) {
    scanInOrderLimited(this->root, sortedArr, 0, stop);
}

template<typename Key>
Node<Key> *SearchTree<Key>::getHistScore(Key const &key) {
    auto *node = this->root;
    while (node != nullptr) {
        if (node->getKey() == key) {
            if (node->getRight()!=nullptr)
            {
                node->subtractHist(node->getRight()->getScoreHist());
            }
            return node->getScoreHist();
        }
        if (key < node->getKey()) {
            auto temp=node->getLeft();
            if (temp!=nullptr)
            {
                if (node->getRight!= nullptr)
                {
                    temp->subtractHist(node->getRight()->getScoreHist());
                }
                temp->decreaseScore(node->getPlayer()->getScore());
            }
            node =temp;
        } else {
            node = node->getRight();
        }
    }
    return nullptr;
}

template<typename Key>
Node<Key> *SearchTree<Key>::getSumOfLevels(Key const &key) {
    auto *node = this->root;
    while (node != nullptr) {
        if (node->getKey() == key) {
            if (node->getRight()!=nullptr)
            {
                node->decreaseSumLevel(node->getRight()->getSumLevel());
            }
            return node->getScoreHist();
        }
        if (key < node->getKey()) {
            auto temp=node->getLeft();
            if (temp!=nullptr)
            {
                if (node->getRight!= nullptr)
                {
                    temp->subtractHist(node->getRight()->getScoreHist());
                }
                temp->decreaseScore(node->getPlayer()->getScore());
            }
            node =temp;
        } else {
            node = node->getRight();
        }
    }
    return nullptr;
}

template<typename Key>
Node<Key> *SearchTree<Key>::find(Key const &key) {
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



template<typename Key>
void SearchTree<Key>::remove(Key const &key) {
    Node<Key> *node = find(key);
    if (node == nullptr) {
        return;
    }
    int* removedHist = node->getScoreHist();
    if (node->getLeft()!= nullptr)
    {
        node->subtractHist(node->getLeft()->getScoreHist());
    }
    int scoreDecreased = 0;
    for (int i=0;i<200;i++){
        if (node->getScoreHist()[i] != 0)
        {
            scoreDecreased = node->getScoreHist()[i];
            break;
        }
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

    HistScorePostRemove(father, node->getPlayer()->getScore(), node->getSumLevel());

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



template<typename Key>
void SearchTree<Key>::removeNoChildren(Node<Key> *node, Node<Key> *father) {
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

template<typename Key>
void SearchTree<Key>::removeOneChildLeft(Node<Key> *node, Node<Key> *father) {
    Node<Key> *leftSon = node->getLeft();
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

template<typename Key>
void SearchTree<Key>::removeOneChildRight(Node<Key> *node, Node<Key> *father) {
    Node<Key> *rightSon = node->getRight();
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

template<typename Key>
Node<Key> *SearchTree<Key>::removeTwoChildren(Node<Key> *node, Node<Key> *father) {
    Node<Key> *nextInOrder = findMin(node->getRight());
    Node<Key> *fatherNextInOrder = nextInOrder->getFather();
    Node<Key> *tempLeft = node->getLeft();
    Node<Key> *tempRight = node->getRight();

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

template<typename Key>
void SearchTree<Key>::deleteTree(Node<Key> *node, Node<Key> *father) {
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

template<typename Key>
bool SearchTree<Key>::isBalanced(Node<Key> *node, bool carry) {
    if (node == nullptr) return true;

    carry = isBalanced(node->getLeft(), carry) && carry;
    carry = isBalanced(node->getRight(), carry) && carry;

    node->calculateHeightAndBalance();

    return carry && abs(node->balancingParameter) < 2;
}

template<typename Key>
bool SearchTree<Key>::isBalanced() {
    return isBalanced(this->root, true);
}

template<typename Key>
void SearchTree<Key>::insert(Key &key, Player &player) {
    Node<Key> *newNode = new Node<Key>(key, player);
    insert(newNode);
}

template<typename Key>
void SearchTree<Key>::mergeWith(Node<Key> **toMergeNodes, int toMergeSize) {
    //TODO: Insert inorder updating of histogram after merging
    auto **ownNodes = new Node<Key>*[size];
    scanInOrder(&ownNodes);
    int mergedSize = this->size + toMergeSize;
    auto **sortedArr = new Node<Key> *[mergedSize];
    int indexToMerge = 0, indexOwn = 0;
    int currentIndex = 0;
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
    setHistInOrder(sortedArr[0],0);
    delete[] ownNodes;
    delete[] sortedArr;
}

template<typename Key>
Node<Key> *SearchTree<Key>::buildFromSortedArray(Node<Key> **array, int indexLeft, int indexRight) {
    if (indexLeft > indexRight) {
        return nullptr;
    }

    int mid = (indexLeft + indexRight) / 2;
    Node<Key> *currentRoot = array[mid];

    currentRoot->setLeft(buildFromSortedArray(array, indexLeft, mid - 1));

    currentRoot->setRight(buildFromSortedArray(array, mid + 1, indexRight));


    return currentRoot;
}

template<typename Key>
Node<Key> *SearchTree<Key>::findLeftmost(Node<Key> *node){
    if(node == nullptr) return node;
    while(node->getLeft() != nullptr){
        node = node->getLeft();
    }

    return node;
}

template<typename Key>
Node<Key> *SearchTree<Key>::findRightmost(Node<Key>* node){
    if(node == nullptr) return node;
    while(node->getRight() != nullptr){
        node = node->getRight();
    }

    return node;
}

template<typename Key>
void SearchTree<Key>::clearTree() {
    clearPostOrder(root);
    root = nullptr;
}

template<typename Key>
double SearchTree<Key>::findM(Node<Key>* node)
{
    if (node== nullptr) reurn 0;
    if (node->sumHist(node->getScoreHist()))

}


#endif //EX1_SEARCHTREE_H

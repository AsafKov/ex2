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

    void HistScorePostInsert(Node<Key> *currentNode, int score, int level)
    {
        currentNode->increaseSumLevel(level);
        currentNode->increaseScore(score);

        Node<Key> *father = currentNode->getFather();
        while (father != nullptr)
        {
            father->increaseScore(score);
            father->increaseSumLevel(level);
            father = father->getFather();
        }
    }

    void HistScorePostRemove(Node<Key> *currentNode, int score, int sumLevel)
    {
        currentNode->decreaseScore(score);
        currentNode->decreaseSumLevel(sumLevel);

        auto father=currentNode->getFather();
        while (father != nullptr)
        {
            father->decreaseScore(score);
            father->decreaseSumLevel(sumLevel);
            father = father->getFather();
        }
    }

    void histPreLL(Node<Key> *balancingPnt)
    {
        Node<PlayerKey> *left_son = balancingPnt->getLeft();
        Node<PlayerKey> *right_son = balancingPnt->getRight();

        if(left_son != nullptr){
            Node<PlayerKey> *right_of_left = balancingPnt->getLeft()->getRight();
            balancingPnt->subtractHist(left_son->getScoreHist());
            balancingPnt->decreaseSumLevel(left_son->getSumLevel());

            if(right_of_left != nullptr){
                balancingPnt->addHist(right_of_left->getScoreHist());
                balancingPnt->increaseSumLevel(right_of_left->getSumLevel());
            }

            if (right_son != nullptr){
                left_son->addHist(right_son->getScoreHist());
                left_son->increaseSumLevel(right_son->getSumLevel());
            }

            left_son->increaseScore(balancingPnt->getPlayer()->getScore());
            left_son->increaseSumLevel(balancingPnt->getPlayer()->getLevel());
        }
    }

    void histPreRR(Node<Key> *balancingPnt)
    {
        Node<PlayerKey> *left_son = balancingPnt->getLeft();
        Node<PlayerKey> *right_son = balancingPnt->getRight();

        if(right_son != nullptr){
            Node<PlayerKey> *left_of_right = balancingPnt->getRight()->getLeft();
            balancingPnt->subtractHist(right_son->getScoreHist());
            balancingPnt->decreaseSumLevel(right_son->getSumLevel());

            if(left_of_right != nullptr){
                balancingPnt->addHist(left_of_right->getScoreHist());
                balancingPnt->increaseSumLevel(left_of_right->getSumLevel());
            }

            if (left_son != nullptr){
                right_son->addHist(left_son->getScoreHist());
                right_son->increaseSumLevel(left_son->getSumLevel());
            }

            right_son->increaseScore(balancingPnt->getPlayer()->getScore());
            right_son->increaseSumLevel(balancingPnt->getPlayer()->getLevel());
        }
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
        if (node->getLeft() != nullptr)
        {
            node->addHist(node->getLeft()->getScoreHist());
            node->increaseSumLevel(node->getLeft()->getSumLevel());
        }
        index = setHistInOrder(node->getRight(), index);
        if (node->getRight() != nullptr)
        {
            node->addHist(node->getRight()->getScoreHist());
            node->increaseSumLevel(node->getRight()->getSumLevel());
        }
        index++;
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
        histPreRR(balancingPnt->getLeft());
        leftRotate(balancingPnt->getLeft());
        histPreLL(balancingPnt);
        rightRotate(balancingPnt);
    }

    void roll_RL(Node<Key> *balancingPnt) {
        histPreLL(balancingPnt->getRight());
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

    double findM(Node<Key>* node, int m, int sum);
    int getSumOfLevels(Key const &key);

    bool isBalanced();

    void clearTree();

    void mergeWith(Node<Key> **toMergeNodes, int toMergeSize);
    Node<Key>* getRoot(){
        return this->root;
    }

    int countPlayersWithScore(Key const &key, int score);

    int countPlayersBeforeKey(Key const &key);

    void getPercentOfPlayersWithScoreInBounds(int lowerLimit, int upperLimit, int score, int *count_in_range, int *count_in_range_with_score, int scale) {
        const int LARGEST = -1, SMALLEST = -2;
        if(size == 0){
            return;
        }

        if(score < 0 || score > scale){
            score = 0;
        }

        PlayerKey dummyKeyL(LARGEST, lowerLimit-1);
        PlayerKey dummyKeyU(SMALLEST, upperLimit+1);

        auto *lower_limit_node = new Node<PlayerKey>(dummyKeyL, new Player(LARGEST, score+1, 0));
        lower_limit_node->getPlayer()->setLevel(lowerLimit-1);
        lower_limit_node->increaseSumLevel(lowerLimit-1);
        insert(lower_limit_node);

        auto upper_limit_node = new Node<PlayerKey>(dummyKeyU, new Player(SMALLEST, score+1, 0));
        upper_limit_node->getPlayer()->setLevel(upperLimit+1);
        upper_limit_node->increaseSumLevel(upperLimit+1);
        insert(upper_limit_node);


        if(score < 0 || score > scale){
            *count_in_range_with_score = 0;
        } else {
            *count_in_range_with_score = countPlayersWithScore(dummyKeyU, score) - countPlayersWithScore(dummyKeyL, score);
        }
        *count_in_range = countPlayersBeforeKey(dummyKeyU) - countPlayersBeforeKey(dummyKeyL) - 1;

        remove(dummyKeyL);
        remove(dummyKeyU);
    }
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
    auto update_hist = temp;
    HistScorePostInsert(update_hist, newNode->getPlayer()->getScore(), newNode->getPlayer()->getLevel());
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
int SearchTree<Key>::countPlayersWithScore(Key const &key, int score) {
    auto *node = this->root;
    int count = 0;
    while(node != nullptr) {
        if (node->getKey() == key) {
            if (node->getRight() != nullptr)
            {
                count -= node->getRight()->getScoreHist()[score];
            }
            return count + node->getScoreCount(score);
        }
        if (node->getKey() > key) {
            node = node->getLeft();
        } else {
            if(node->getLeft() != nullptr){
                count += node->getLeft()->getScoreCount(score);
            }
            if(node->getPlayer()->getScore() == score){
                count++;
            }
            node = node->getRight();
        }
    }

    return count;
}

template<typename Key>
int SearchTree<Key>::countPlayersBeforeKey(const Key &key) {
    auto *node = this->root;
    int count = 0;
    while(node != nullptr) {
        if (node->getKey() == key) {
            if (node->getLeft() != nullptr)
            {
                count = node->getLeft()->getTreeSize();
            }
            return count;
        }
        if (node->getKey() > key) {
            node = node->getLeft();
        } else {
            if(node->getLeft() != nullptr){
                count += node->getLeft()->getTreeSize();
            }
            count++;
            node = node->getRight();
        }
    }

    return count;
}

template<typename Key>
int SearchTree<Key>::getSumOfLevels(Key const &key) {
    auto *node = this->root;
    int sum = 0;
    while (node != nullptr) {
        if (node->getKey() == key) {
            if (node->getRight() != nullptr) {
                sum -= node->getRight()->getSumLevel();
            }
            sum -= node->getPlayer()->getLevel();
        }
        return sum + node->getSumLevel();
        if (key < node->getKey()) {
            auto temp = node->getLeft();
            if (temp != nullptr) {
                continue;
            } else {
                return 0;
            }
        } else {
            if (node->getLeft() != nullptr) {
                sum += node->getLeft()->getSumLevel();
            }
            sum += node->getPlayer()->getLevel();
            node = node->getRight();
        }
    }
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

    auto *father = node->getFather();
    auto *org_father = node->getFather();

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

    if(org_father != nullptr){
        HistScorePostRemove(org_father, node->getPlayer()->getScore(), node->getPlayer()->getLevel());
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
        nextInOrder->increaseSumLevel(tempLeft->getSumLevel());
        nextInOrder->addHist(tempLeft->getScoreHist());
        removeOneChildLeft(node, nextInOrder);
        return nextInOrder;
    } else {
        fatherNextInOrder->setLeft(node);
    }

    node->setRight(nextInOrder->getRight());
    node->setLeft(nullptr);
    nextInOrder->setLeft(tempLeft);
    nextInOrder->setRight(tempRight);

    fatherNextInOrder->subtractHist(nextInOrder->getScoreHist());
    fatherNextInOrder->decreaseSumLevel(nextInOrder->getSumLevel()); // TODO: Validate

    if(tempRight != nullptr){
        nextInOrder->increaseSumLevel(tempRight->getSumLevel());
        nextInOrder->addHist(tempRight->getScoreHist());
    }

    if(tempLeft != nullptr){
        nextInOrder->increaseSumLevel(tempLeft->getSumLevel());
        nextInOrder->addHist(tempLeft->getScoreHist());
    }

    if (node->getRight() != nullptr) {
        fatherNextInOrder->increaseSumLevel(node->getRight()->getSumLevel());
        fatherNextInOrder->addHist(node->getRight()->getScoreHist());
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
    auto *newNode = new Node<Key>(key, player);
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
        sortedArr[i]->clearRanks();
    }
    setRoot(buildFromSortedArray(sortedArr, 0, mergedSize - 1));
    this->size = mergedSize;
    setHistInOrder(root,0);
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
double SearchTree<Key>::findM(Node<Key>* node, int m, int sum){
    if(node == nullptr || m == 0) return sum;
    if(node->getRight() != nullptr) {
        if(node->getRight()->getTreeSize() == m-1){
            return sum + node->getPlayer()->getLevel() + node->getRight()->getSumLevel();
        }

        if(node->getRight()->getTreeSize() > m-1){
            return findM(node->getRight(), m, sum);
        }

        if(node->getRight()->getTreeSize() < m-1){
            sum += node->getPlayer()->getLevel() + node->getRight()->getSumLevel();
            return findM(node->getLeft(), m-1-node->getRight()->getTreeSize(), sum);
        }
    } else {
        if(node->getTreeSize() <= m){
            return sum + node->getSumLevel();
        } else {
            sum += node->getPlayer()->getLevel();
            return findM(node->getLeft(), m-1, sum);
        }
    }

    return sum;
}


#endif //EX1_SEARCHTREE_H

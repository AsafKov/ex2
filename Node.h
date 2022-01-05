#ifndef EX2_NODE_H
#define EX2_NODE_H
#include "Player.h"
#include <cmath>

template <typename Key>
class Node {
private:
    const Key key;
    Player* player;
    int *scores_hist;
    int sumLevels;
    Node<Key>* left = nullptr;
    Node<Key>* right = nullptr;
    Node<Key>* father = nullptr;


public:
    int height, balancingParameter;
    explicit Node<Key>(Key const &k, Player* val) : key(k), player(val), left(nullptr),
                                                          right(nullptr), father(nullptr), height(0), balancingParameter(0)
    {
        scores_hist = new int[200];
        for (int i=0; i<200; i++){
            scores_hist[i] = 0;
        }
        scores_hist[player->getScore()]++;
        sumLevels = player->getLevel();
    };
    ~Node() = default;
    Player *getPlayer() { return this->player; }
    const Key &getKey() const { return this->key; }
    Node *getLeft() const { return this->left; }
    Node *getRight() const { return this->right; }
    int getScoreCount(int score) const {return this->scores_hist[score]; }
    int *getScoreHist() const {return this->scores_hist;}
    int getSumLevel() const {return this->sumLevels;}
    void increaseSumLevel(int setVal)  {this->sumLevels+=setVal;}
    void decreaseSumLevel(int setVal)  {this->sumLevels-=setVal;}

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

    void clearRanks(){
        this->sumLevels = player->getLevel();
        for(int i=0; i<200; i++){
            scores_hist[i] = 0;
        }

        scores_hist[player->getScore()] = 1;
    }

    int getTreeSize(){
        int size = 0;
        for(int i=0; i<200; i++){
            size += scores_hist[i];
        }

        return size;
    }

    void clearNode(){
        this->left = nullptr;
        this->right = nullptr;
        this->father = nullptr;
    }

    void decreaseScore(int k)
    {
        this->scores_hist[k]--;
    }

    void increaseScore(int k)
    {
        this->scores_hist[k]++;
    }
    
    int findDifferentScore(int* hist)
    {
        if (hist== nullptr) return 0;
        int *scores = new int[200];
        int scoreUpdated=0;
        for (int i=0; i<200; i++)
        {
            scores[i]=abs(this->scores_hist[i] - hist[i]);
            if (scores[i]>0) scoreUpdated=scores[i];
        }
        return scoreUpdated;
    }

    void subtractHist(const int* hist)
    {
        if (hist== nullptr) return;
        for (int i=0; i<200; i++)
        {
            this->scores_hist[i] -= hist[i];
        }
    }

    void addHist(const int* hist)
    {
        if (hist == nullptr) return;
        for (int i=0; i<200; i++)
        {
            this->scores_hist[i]+=hist[i];
        }
    }

    void setFather(Node<Key>* const prev){ this->father = prev;}
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
    Node<Key> *getFather() const { return this->father; }
};



#endif //EX2_NODE_H

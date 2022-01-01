#ifndef EX1_NODE_H
#define EX1_NODE_H

#include <cmath>
#include "Player.h"

template <typename Key>
class Node {
private:
    const Key key;
    Player player;
    int *score;
    Node<Key>* left = nullptr;
    Node<Key>* right = nullptr;
    Node<Key>* father = nullptr;


public:
    int height, balancingParameter;
    explicit Node<Key>(Key const &k, Player const &val) : key(k), player(val), left(nullptr),
                                                                right(nullptr), father(nullptr), height(0), balancingParameter(0)
                                        {
        score = new int[200];
        for (int i=0; i<200; i++){
            score[i]=0;
        }
    };
    ~Node() = default;
    Player &getPlayer() { return this->player; }
    const Key &getKey() const { return this->key; }
    Node *getLeft() const { return this->left; }
    Node *getRight() const { return this->right; }
    int *getScore() const {return this->score;}
    void setPlayer(Player &nodeData){
        this->player=nodeData;
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

    void setScore(int k)
    {
        this->score[k]++;
    }

    void decreaseScore(int k)
    {
        this->score[k]--;
    }

    void addScore(int k)
    {
        this->score[k]++;
    }



    int findDifferentScore(int* hist)
    {
        if (hist== nullptr) return 0;
        int scores[200];
        int scoreUpdated=0;
        for (int i=0; i<200; i++)
        {
            scores[i]=abs(this->score[i]-hist[i]);
            if (scores[i]>0) scoreUpdated=scores[i];
        }
        return scoreUpdated;
    }

    void decreaseHist(int* hist)
    {
        if (hist== nullptr) return;
        for (int i=0; i<200; i++)
        {
            this->score[i]-=hist[i];
        }
    }

    void updateHist(int* hist)
    {
        if (hist== nullptr) return;
        for (int i=0; i<200; i++)
        {
            this->score[i]+=hist[i];
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


#endif //EX1_NODE_H

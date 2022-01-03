#ifndef EX2_UNIONFIND_H
#define EX2_UNIONFIND_H
#include "DumbNode.h"

class UnionFind{
private:
    int size;
    const int scale;
    DumbNode *root;
    int *hist;

public:
    explicit UnionFind(int group_index, int scale): size(0), scale(scale), root(nullptr){
        hist = new int[scale](0);
    }
    int getSize() const{ return this->size; }
    DumbNode *getRoot(){ return this->root; }
    void setSize(int newSize){ this->size = newSize; }
    void insertNode(DumbNode *node, int score){
        node->setFather(root);
        size++;
        hist[score]++;
    }
    void mergeWith(UnionFind const &unionFind){
        DumbNode *prevRoot = unionFind.getRoot();
        prevRoot->setFather(this->root);
        size += unionFind.getSize();
        unionFind.mergeHist(&hist);
    }
    void mergeHist(int **mergeInto) const{
        for(int i = 0; i < scale; i++){
            (*mergeInto[i]) += this->hist[i];
        }
    }
    void getPlayersWithScore(int score){ return hist[score]; }
};

#endif //EX2_UNIONFIND_H

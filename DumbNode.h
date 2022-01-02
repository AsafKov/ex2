#ifndef EX2_DUMBNODE_H
#define EX2_DUMBNODE_H

class DumbNode{
private:
    DumbNode *father;
    DumbNode *son;
    int index;

public:
    DumbNode(int index):father(nullptr), son(nullptr), index(-1){};
    DumbNode *getFather(){ return this->father; }
    void setIndex(int index){ this->index = index; }
    void setFather(DumbNode *node){
        this->father = node;
        if(node != nullptr){
            node->setSon(this);
        }
    }
    DumbNode *getSon(DumbNode *node){ return this->son; }
    void setSon(DumbNode *node){ this->son = node; }
};

#endif //EX2_DUMBNODE_H

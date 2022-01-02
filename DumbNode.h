#ifndef EX2_DUMBNODE_H
#define EX2_DUMBNODE_H

class DumbNode{
private:
    DumbNode *father;

public:
    DumbNode():father(nullptr){};
    DumbNode *getFather(){ return this->father; }
    void setFather(DumbNode *node){ this->father = node; }
};

#endif //EX2_DUMBNODE_H

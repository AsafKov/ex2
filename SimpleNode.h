//
// Created by asafk on 02/01/2022.
//

#ifndef EX2_SIMPLENODE_H
#define EX2_SIMPLENODE_H

class SimpleNode{
private:
    SimpleNode *father;

public:
    SimpleNode():father(nullptr){};
    SimpleNode *getFather(){ return this->father; }
    void setFather(SimpleNode *node){ this->father = node; }
};

#endif //EX2_SIMPLENODE_H

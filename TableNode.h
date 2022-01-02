#ifndef EX1_NODE_H
#define EX1_NODE_H

#include <cmath>
#include "DumbNode.h"
#include "Player.h"

class TableNode {
private:
    DumbNode nodeInGroup;
    Player* player;

public:
    explicit TableNode(DumbNode nodeInGroup, Player &player) : nodeInGroup(nodeInGroup), player(player) {};
    ~TableNode() = default;
    void setNodeInGroup(DumbNode node)
    {
        this->nodeInGroup=node;
    }
};


#endif //EX1_NODE_H

#ifndef EX2_PLAYERMANAGER_H

#define EX2_PLAYERMANAGER_H
#include "HashTable.h"
#include <memory>

class PlayerManager{
private:
    typename std::shared_ptr<Player> PlayerOwner;
    int num_of_groups;
    int scale;
    Node<PlayerKey> **groups_ranked_trees;
    HashTable<int, PlayerOwner> players;
    DumbNode **groups_roots_array;
    Node<Player*> **players_tree;

public:
    PlayerManager(PlayerManager const &manager) = delete;
    PlayerManager &operator=(PlayerManager const &manager) = delete;
    PlayerManager(int number_of_groups, int scale): num_of_groups(number_of_groups), scale(scale){
        groups_ranked_trees = new Node<PlayerKey>*[number_of_groups];
        groups_roots_array = new DumbNode*[number_of_groups];
        players();
    }

};


#endif //EX2_PLAYERMANAGER_H

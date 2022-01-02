#ifndef EX2_PLAYERMANAGER_H

#define EX2_PLAYERMANAGER_H
#include "HashTable.h"
#include "DumbNode.h"
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


    AddPlayer(int playerId, int groupId, int score)
    {
        if (playerId<=0 || groupId<=0 || level<0){
            return INVALID_INPUT;
        }

        Player *newPlayer;
        try {
            newPlayer = new Player(playerId, level, groupId, score);
        } catch (std::bad_alloc &exception){
            return ALLOCATION_ERROR;
        }

        DumbNode *groupNode = groups_roots_array[GroupID];
        if(players_tree.find(playerId) != nullptr || groupNode == nullptr){
            return FAILURE;
        }

        PlayerOwner playerOwner(newPlayer);
        DumbNode *playerNode=newPlayer;
        playerNode->setFather(groupNode);
        playerNodeRankTree = new Node<Player*> player;
        this->players_tree.insert(playerNodeRankTree);

        PlayerKey playerKey = PlayerKey(playerId, level);
        groups_ranked_trees[groupId].insert(playerKey, newPlayer);

        this->players->insert(playerId, playerOwner);

        return SUCCESS;
    }


};


#endif //EX2_PLAYERMANAGER_H

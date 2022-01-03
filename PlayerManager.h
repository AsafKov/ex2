#ifndef EX2_PLAYERMANAGER_H

#define EX2_PLAYERMANAGER_H
#include "HashTable.h"
#include "UnionFind.h"
#include "TableNode.h"
#include <memory>

class PlayerManager{
private:
    typename std::shared_ptr<Player> PlayerOwner;
    int num_of_groups;
    int scale;
    SearchTree<PlayerKey> **groups_ranked_trees;
    HashTable<TableNode> players;
    UnionFind **groups_roots_array;
    SearchTree<PlayerKey> **players_tree;

    private insertToGroup(int group, ){

    }

public:
    PlayerManager(PlayerManager const &manager) = delete;
    PlayerManager &operator=(PlayerManager const &manager) = delete;
    PlayerManager(int number_of_groups, int scale): num_of_groups(number_of_groups), scale(scale){
        groups_ranked_trees = new Node<PlayerKey>*[number_of_groups];
        groups_roots_array = new UnionFind*[number_of_groups];
        players();
    }


    StatusType AddPlayer(int playerId, int groupId, int score)
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

        DumbNode *playerNode=newPlayer;
        TableNode playerTableNode = new TableNode(playerNode, newPlayer);
        this->players->insert(playerNode);

        DumbNode *groupNode = groups_roots_array[GroupID];
        if(players_tree.find(playerId) != nullptr || groupNode == nullptr){
            return FAILURE;
        }
        playerNode->setFather(groupNode);
        PlayerKey playerKey = PlayerKey(playerId, level);
        this->groups_ranked_trees[groupId]->insert(playerKey, newPlayer);
        this->players_tree->insert(playerKey, newPlayer);
        return SUCCESS;
    }




};


#endif //EX2_PLAYERMANAGER_H

#ifndef EX2_PLAYERMANAGER_H

#define EX2_PLAYERMANAGER_H

#include <memory>
#include "HashTable.h"
#include "UnionTree.h"
#include "library2.h"

class PlayerManager{
private:
    typedef std::shared_ptr<Player> PlayerOwner;

    int num_of_groups;
    int scale;

    HashTable<int, PlayerOwner> *players_table;
    UnionTree *group_trees;
    SearchTree<PlayerKey> *players_tree;
    int ***score_hist_level_0;

    void combine_hists(int group1, int group2){
        int **score_hist_group1 = (*score_hist_level_0)[group1];
        int **score_hist_group2 = (*score_hist_level_0)[group2];

        if(score_hist_group1 == score_hist_group2){
            return;
        }

        for(int i=0; i<scale; i++){
            (*score_hist_group1)[i] += (*score_hist_group2)[i];
        }

        (*score_hist_level_0)[group2] = (*score_hist_group1)[group1];
    }

    void increaseScoreCount(int score, int group_id){
        (*(*score_hist_level_0)[group_id])[score]++;
        (*(*score_hist_level_0)[0])[score]++;
    }

    void decreaseScoreCount(int score, int group_id){
        (*(*score_hist_level_0)[group_id])[score]--;
        (*(*score_hist_level_0)[0])[score]--;
    }

public:
    PlayerManager(PlayerManager const &manager) = delete;
    PlayerManager &operator=(PlayerManager const &manager) = delete;
    PlayerManager(int number_of_groups, int scale): num_of_groups(number_of_groups), scale(scale){
        players_table = new HashTable<int, PlayerOwner>();
        group_trees = new UnionTree(number_of_groups);
        score_hist_level_0 = new int**[number_of_groups+1](); // num_of_groups+1 for level_0 hist of the entire system (index 0)
        for(int i=0; i<number_of_groups+1; i++){
            (*score_hist_level_0)[i] = new int[scale]();
        }
        players_tree = new SearchTree<PlayerKey>();
    }

    StatusType mergeGroups(int group1, int group2){
        if(group1 == group2){
            return SUCCESS;
        }

        combine_hists(int group1, int group2);
        group_trees->mergeGroups(int group1, int group2);
    }

    StatusType addPlayer(int player_id, int group_id, int score){
        if(player_id <= 0 || score <= 0 || score > scale){
            return INVALID_INPUT;
        }

        Player *newPlayer;
        try {
            newPlayer = new Player(playerId, 0, groupId);
        } catch (std::bad_alloc &exception){
            return ALLOCATION_ERROR;
        }

        PlayerOwner playerOwner(newPlayer);
        PlayerKey key(player_id, level);
        players_table->insert(player_id, playerOwner);
        group_trees->insert(new Node<PlayerKey>(key, playerOwner.get()));
        players_tree->insert(new Node<PlayerKey>(key, playerOwner.get()));
        increaseScoreCount(score, group_id);
    }

    StatusType removePlayer(int player_id){
        PlayerOwner owner = players_table->get(player_id);
        if(owner == nullptr){
            return FAILURE;
        }
        Player *player = owner.get();
        PlayerKey key(player->getId(), player->getLevel());
        group_trees->remove(key, player->getGroupId());
        if(player->getLevel() == 0){
            decreaseScoreCount(player->getScore(), player->getGroupId());
        }
        players_table->remove(player_id);
    }

    StatusType increasePlayerLevel(int player_id, int levelIncrease){
        if(player_id <= 0 || levelIncrease <= 0){
            return INVALID_INPUT;
        }

        PlayerOwner owner = players_table->get(player_id);
        if(owner == nullptr){
            return FAILURE;
        }

        Player *player = owner.get();
        PlayerKey key(player_id, player->getLevel());
        if(player->getLevel() == 0){
            decreaseScoreCount(player->getScore(), player->getGroupId());
        }

        group_trees->remove(key);
        player->increaseLevel(levelIncrease);
        group_trees->insert(new Node<PlayerKey>(key, player), player->getGroupId());
    }

    StatusType changePlayerIDScore(int PlayerID, int NewScore)
    {
        if(player_id <= 0 || levelIncrease <= 0){
            return INVALID_INPUT;
        }

        PlayerOwner owner = players_table->get(player_id);
        if(owner == nullptr){
            return FAILURE;
        }

        Player *player = owner.get();
        PlayerKey key(player_id, player->getLevel());
        if(player->getLevel() == 0){
            decreaseScoreCount(player->getScore(), player->getGroupId());
            increaseScoreCount(NewScore, player->getGroupId());
        }

        group_trees->remove(key);
        player->increaseLevel(levelIncrease);
        group_trees->insert(new Node<PlayerKey>(key, player), player->getGroupId());
    }

    StatusType averageHighestPlayerLevelByGroup(int GroupID, int m, double * avgLevel)
    {
        if(GroupID>k || GroupID<0 || m<=0 || avgLevel== nullptr){
            return INVALID_INPUT;
        }

        UnionNode* cur= this->group_trees[GroupID]


    }


};

    StatusType getPercentOfPlayersWithScoreInBounds(int GroupID, int score, int lowerLevel, int higherLevel, double *players){
        if(players == nullptr || groupID < 0 || groupID >= num_of_groups){
            return INVALID_INPUT;
        }

        if(groupID == 0){

        }
    }

};

#endif //EX2_PLAYERMANAGER_H

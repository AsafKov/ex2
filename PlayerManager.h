#ifndef EX2_PLAYERMANAGER_H

#define EX2_PLAYERMANAGER_H

#include <memory>
#include "HashTable.h"
#include "UnionTree.h"
#include "library2.h"
#include "UnionHists.h"

class PlayerManager{
private:
    typedef std::shared_ptr<Player> PlayerOwner;

    int num_of_groups;
    int scale;

    HashTable<int, PlayerOwner> *players_table;
    UnionTree *group_trees;
    UnionHists *groups_level_0;
    int *score_hist_level_0;
    SearchTree<PlayerKey> *players_tree;

    void increaseScoreCount(int score, int group_id){
        groups_level_0->increaseScoreCount(group_id, score);
        score_hist_level_0[score]++;
    }

    void decreaseScoreCount(int score, int group_id){
        groups_level_0->decreaseScoreCount(group_id, score);
        score_hist_level_0[score]--;
    }

    int countLevel_0_total(){
        int count = 0;
        for(int i=0; i<scale+1; i++){
            count += score_hist_level_0[i];
        }

        return count;
    }

public:
    PlayerManager(PlayerManager const &manager) = delete;
    PlayerManager &operator=(PlayerManager const &manager) = delete;
    PlayerManager(int number_of_groups, int scale): num_of_groups(number_of_groups), scale(scale){
        players_table = new HashTable<int, PlayerOwner>();
        group_trees = new UnionTree(number_of_groups, scale);
        groups_level_0 = new UnionHists(number_of_groups, scale);
        score_hist_level_0 = new int[scale+1]();
        for(int i=0; i<scale+1; i++){
            score_hist_level_0[i] = 0;
        }
        players_tree = new SearchTree<PlayerKey>();
    }



    StatusType mergeGroups(int group1, int group2){
        if(group1 <= 0  || group2 <= 0 || group1 > num_of_groups || group2 > num_of_groups){
            return INVALID_INPUT;
        }
        if(group1 == group2){
            return SUCCESS;
        }

        groups_level_0->mergeGroups(group1, group2);
        group_trees->mergeGroups(group1, group2);
        return SUCCESS;
    }

    StatusType addPlayer(int player_id, int group_id, int score){
        if(player_id <= 0 || score <= 0 || score > scale || group_id <= 0 || group_id > num_of_groups){
            return INVALID_INPUT;
        }

        if(players_table->contains(player_id)){
            return FAILURE;
        }

        Player *newPlayer;
        try {
            newPlayer = new Player(player_id, score, group_id);
        } catch (std::bad_alloc &exception){
            return ALLOCATION_ERROR;
        }

        PlayerOwner playerOwner(newPlayer);
        players_table->insert(player_id, playerOwner);

        increaseScoreCount(score, group_id);
        return SUCCESS;
    }

    StatusType removePlayer(int player_id){
        if(player_id <= 0){
            return INVALID_INPUT;
        }

        if(!players_table->contains(player_id)){
            return FAILURE;
        }

        PlayerOwner owner = players_table->get(player_id);

        Player *player = owner.get();
        PlayerKey key(player->getId(), player->getLevel());
        group_trees->remove(key, player->getGroupId());
        players_tree->remove(key);
        if(player->getLevel() == 0){
            decreaseScoreCount(player->getScore(), player->getGroupId());
        }
        players_table->remove(player_id);
        return SUCCESS;
    }

    StatusType increasePlayerLevel(int player_id, int levelIncrease){
        if(player_id <= 0 || levelIncrease <= 0){
            return INVALID_INPUT;
        }

        if(!players_table->contains(player_id)){
            return FAILURE;
        }

        PlayerOwner owner = players_table->get(player_id);

        Player *player = owner.get();

        PlayerKey key(player_id, player->getLevel());
        if(player->getLevel() == 0){
            decreaseScoreCount(player->getScore(), player->getGroupId());
        }

        group_trees->remove(key, player->getGroupId());
        players_tree->remove(key);
        player->increaseLevel(levelIncrease);
        PlayerKey newKey(player_id, player->getLevel());
        group_trees->insert(new Node<PlayerKey>(newKey, player), player->getGroupId());
        players_tree->insert(new Node<PlayerKey>(newKey, player));
        return SUCCESS;
    }


    StatusType getPercentOfPlayersWithScoreInBounds(int group_id, int score, int lowerLevel, int higherLevel, double *players){
        if(players == nullptr || group_id < 0 || group_id > num_of_groups){
            return INVALID_INPUT;
        }

        if(lowerLevel > higherLevel){
            *players = 0;
            return FAILURE;
        }

        int count_in_range = 0, count_in_range_with_score = 0;

        if(group_id == 0){
            players_tree->getPercentOfPlayersWithScoreInBounds(lowerLevel, higherLevel, score, &count_in_range, &count_in_range_with_score, scale);
            if(lowerLevel == 0){
                count_in_range += countLevel_0_total();
                count_in_range_with_score += score_hist_level_0[score];
            }
        } else {
            group_trees->countPlayersWithScoreInBounds(lowerLevel, higherLevel, group_id, score, &count_in_range, &count_in_range_with_score);
            if(lowerLevel == 0){
                groups_level_0->countPlayersWithScore(group_id, score, &count_in_range, &count_in_range_with_score);
            }
        }

        if(count_in_range == 0) {
            *players = 0;
            return FAILURE;
        }

        *players = (double) count_in_range_with_score / count_in_range;
        *players *= 100;
        return SUCCESS;
    }

    StatusType changePlayerIDScore(int player_id, int new_score){
        if(player_id <= 0 || new_score <= 0 || new_score > scale){
            return INVALID_INPUT;
        }

        if(!players_table->contains(player_id)){
            return FAILURE;
        }

        PlayerOwner owner = players_table->get(player_id);

        Player *player = owner.get();

        PlayerKey key(player_id, player->getLevel());
        if(player->getLevel() == 0){
            decreaseScoreCount(player->getScore(), player->getGroupId());
            increaseScoreCount(new_score, player->getGroupId());
            player->setScore(new_score);
        } else {
            group_trees->remove(key, player->getGroupId());
            players_tree->remove(key);
            player->setScore(new_score);
            group_trees->insert(new Node<PlayerKey>(key, player), player->getGroupId());
            players_tree->insert(new Node<PlayerKey>(key, player));
        }
        return SUCCESS;
    }

    StatusType averageHighestPlayerLevelByGroup(int groupId, int m, double *avgLevel)
    {
        if(groupId > num_of_groups || groupId < 0 || m <= 0 || avgLevel == nullptr){
            return INVALID_INPUT;
        }

        if (groupId > 0) {
            if(group_trees->getGroupSize(groupId) + groups_level_0->getGroupSize(groupId) < m){
                return FAILURE;
            }
            *avgLevel = group_trees->averageHighestPlayerLevelByGroup(groupId, m) / m;
        } else {
            if (players_tree->getSize() + countLevel_0_total() < m){
                return FAILURE;
            }
            *avgLevel = players_tree->findM(players_tree->getRoot(), m, 0) / m;
        }

        return SUCCESS;

    }
};

#endif //EX2_PLAYERMANAGER_H

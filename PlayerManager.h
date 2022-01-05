#ifndef EX2_PLAYERMANAGER_H

#define EX2_PLAYERMANAGER_H

#include <memory>
#include "HashTable.h"
#include "UnionTree.h"
#include "library2.h"
#include <iostream>

class PlayerManager{
private:
    typedef std::shared_ptr<Player> PlayerOwner;

    int num_of_groups;
    int scale;

    HashTable<int, PlayerOwner> *players_table;
    UnionTree *group_trees;
    SearchTree<PlayerKey> *players_tree;
    int **score_hist_level_0;

    void combine_hists(int group1, int group2){
        int *score_hist_group1 = score_hist_level_0[group1];
        int *score_hist_group2 = score_hist_level_0[group2];

        if(score_hist_group1 == score_hist_group2){
            return;
        }

        int *newArr = new int[scale+1]();

        for(int i=0; i<scale + 1; i++){
            newArr[i] += score_hist_group1[i] +score_hist_group2[i];
        }

        score_hist_level_0[group1] = newArr;
        score_hist_level_0[group2] = newArr;
    }

    void increaseScoreCount(int score, int group_id){
        score_hist_level_0[group_id][score]++;
        score_hist_level_0[0][score]++;
    }

    void decreaseScoreCount(int score, int group_id){
        score_hist_level_0[group_id][score]--;
        score_hist_level_0[0][score]--;
    }

    int countLevel_0(int group){
        int count = 0;
        for(int i=0; i<scale+1; i++){
            count += score_hist_level_0[group][i];
        }

        return count;
    }

public:
    PlayerManager(PlayerManager const &manager) = delete;
    PlayerManager &operator=(PlayerManager const &manager) = delete;
    PlayerManager(int number_of_groups, int scale): num_of_groups(number_of_groups), scale(scale){
        players_table = new HashTable<int, PlayerOwner>();
        group_trees = new UnionTree(number_of_groups, scale);
        score_hist_level_0 = new int*[number_of_groups+1](); // num_of_groups+1 for level_0 hist of the entire system (index 0)
        for(int i=0; i<number_of_groups+1; i++){
            score_hist_level_0[i] = new int[scale+1]{0};
            for(int j=0; j<scale+1; j++){
                score_hist_level_0[i][j] = 0;
            }
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

        combine_hists(group1, group2);
        group_trees->mergeGroups(group1, group2);
        testMerge();
        return SUCCESS;
    }

    void testMerge(){
        int a[6] = {4, 16, 23, 25, 28, 40};
        for(int i=0; i<6; i++){
            std::cout<<"group " << a[i]<<": ";
            for(int j=0; j<scale+1; j++){
                std::cout<<score_hist_level_0[a[i]][j]<< " ";
            }
            std::cout<<"\n";
        }
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
        if(players == nullptr || group_id < 0 || group_id >= num_of_groups){
            return INVALID_INPUT;
        }

        if(lowerLevel > higherLevel){
            *players = 0;
            return FAILURE;
        }

        int count_in_range = 0, count_in_range_with_score = 0;

        if(group_id == 0){
            players_tree->getPercentOfPlayersWithScoreInBounds(lowerLevel, higherLevel, score, &count_in_range, &count_in_range_with_score, scale);
        } else {
            group_trees->countPlayersWithScoreInBounds(lowerLevel, higherLevel, group_id, score, &count_in_range, &count_in_range_with_score);
        }

        if(lowerLevel == 0){
            count_in_range += countLevel_0(group_id);
            if(score >= 0 && score <= scale){
                count_in_range_with_score += score_hist_level_0[group_id][score];
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
            player->setScore(new_score);
            group_trees->remove(key, player->getGroupId());
            group_trees->insert(new Node<PlayerKey>(key, player), player->getGroupId());
            players_tree->remove(key);
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
            if(group_trees->getGroupSize(groupId) + countLevel_0(groupId) < m){
                return FAILURE;
            }
            *avgLevel = group_trees->averageHighestPlayerLevelByGroup(groupId, m) / m;
        } else {
            if (players_tree->getSize() + countLevel_0(0) < m){
                return FAILURE;
            }
            *avgLevel = players_tree->findM(players_tree->getRoot(), m, 0) / m;
        }

        return SUCCESS;

    }
};

#endif //EX2_PLAYERMANAGER_H

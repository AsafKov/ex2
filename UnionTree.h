#ifndef EX2_UNIONTREE_H
#define EX2_UNIONTREE_H

#include <iostream>
#include "SearchTree.h"

class UnionTree {
private:
    class UnionNode{
    public:
        SearchTree<PlayerKey> *tree;
        UnionNode *next;
        UnionNode(): tree(new SearchTree<PlayerKey>()), next(nullptr){}
    };

    const int scale;
    const int num_of_groups;

    UnionNode **rank_trees;

    UnionNode *findTreeNode(int group){
        UnionNode *node = rank_trees[group];
        while(node->next != nullptr) {
                node = node->next;
        }

        // Updating path after finding the root
        UnionNode *updatePath = rank_trees[group];
        UnionNode *curr_next;
        while(updatePath->next != nullptr) {
            curr_next = updatePath->next;
            updatePath->next = node;
            updatePath = curr_next;
        }

        return node;
    }

public:
    UnionTree(UnionTree const &tree) = delete;
    UnionTree &operator=(UnionTree const &tree) = delete;
    explicit UnionTree(int groups, int scale) : num_of_groups(groups), scale(scale){
        rank_trees = new UnionNode*[num_of_groups]();
        for(int i = 0; i < num_of_groups; i++){
            rank_trees[i] = new UnionNode();
        }
    }
    double averageHighestPlayerLevelByGroup(int group_id, int m)
    {
        group_id--;
        UnionNode *groupNode = findTreeNode(group_id);
        return groupNode->tree->findM(groupNode->tree->getRoot(), m,0);
    }


    void insert(Node<PlayerKey> *playerNode, int group){
        group--;
        UnionNode *node = findTreeNode(group);
        node->tree->insert(playerNode);
    }

    void remove(PlayerKey &key, int group){
        group--;
        UnionNode *node = findTreeNode(group);
        node->tree->remove(key);
    }

    void mergeGroups(int group1, int group2){
        group1--;
        group2--;
        UnionNode *group1_node = findTreeNode(group1);
        UnionNode *group2_node = findTreeNode(group2);
        UnionNode *smaller, *bigger;
        if(group1_node != group2_node){
            if(group1_node->tree->getSize() > group2_node->tree->getSize()){
                bigger = group1_node;
                smaller = group2_node;
            } else {
                bigger = group2_node;
                smaller = group1_node;
            }

            auto **smaller_nodes = new Node<PlayerKey>*[smaller->tree->getSize()];
            smaller->tree->scanInOrder(&smaller_nodes);
            smaller->tree->clearTree();
            bigger->tree->mergeWith(smaller_nodes, smaller->tree->getSize());
            smaller->next = bigger;

            delete smaller->tree;
            smaller->tree = nullptr;
        }
    }

    void countPlayersWithScoreInBounds(int lowerLimit, int upperLimit, int group, int score, int *count_in_range, int *count_in_range_with_score){
        group--;
        UnionNode *node = findTreeNode(group);
        node->tree->getPercentOfPlayersWithScoreInBounds(lowerLimit, upperLimit, score, count_in_range, count_in_range_with_score, scale);
    }

    int getGroupSize(int group){
        group--;
        UnionNode *node = findTreeNode(group);
        return node->tree->getSize();
    }
};


#endif //EX2_UNIONTREE_H

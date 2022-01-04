#ifndef EX2_UNIONTREE_H
#define EX2_UNIONTREE_H

#include "SearchTree.h"

class UnionTree {
private:
    class UnionNode{
    public:
        SearchTree<PlayerKey> *tree;
        UnionNode *next;
        UnionNode(): tree(new SearchTree<PlayerKey>()), next(nullptr){}
    };

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
    explicit UnionTree(int groups) : num_of_groups(groups){
        rank_trees = new UnionNode*[num_of_groups];
        for(int i = 0; i < num_of_groups; i++){
            rank_trees[i] = new UnionNode();
        }
    }
    double averageHighestPlayerLevelByGroup(int GroupID, int m)
    {
        UnionNode *groupNode = findTreeNode((GroupID));
        if (m>groupNode->tree->getSize()) return -1;
        double avg = groupNode->tree->findM(groupNode->tree->getRoot(),m,0);
        return avg;
    }


    void insert(Node<PlayerKey> *playerNode, int group){
        UnionNode *node = findTreeNode(group);
        node->tree->insert(playerNode);
    }

    void remove(PlayerKey &key, int group){
        UnionNode *node = findTreeNode(group);
        node->tree->remove(key);
    }

    void mergeGroups(int group1, int group2){
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
            bigger->tree->mergeWith(smaller_nodes, smaller->tree->getSize());
            smaller->next = bigger;

            smaller->tree->clearTree();
            delete smaller->tree;
            smaller->tree = nullptr;
        }
    }

    int countPlayersWithScoreInBounds(int lowerLimit, int upperLimit, int group, int score){
        UnionNode *node = findTreeNode(group);
        return node->tree->getPercentOfPlayersWithScoreInBounds(lowerLimit, upperLimit, score);
    }

    int getUnionGroupSize(int group_id){
        UnionNode *node = findTreeNode(group_id);
        if(node != nullptr){
            return node->tree->getSize();
        }

        return 0;
    }
};


#endif //EX2_UNIONTREE_H

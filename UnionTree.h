#ifndef EX2_UNIONTREE_H
#define EX2_UNIONTREE_H

#include "SearchTree.h"

class UnionTree {
private:
    class UnionNode{
    public:
        SearchTree<PlayerKey> *tree;
        UnionNode *next, *current_root;
        UnionNode(): tree(new SearchTree<PlayerKey>()), next(nullptr), current_root(nullptr){}
    };

    const int num_of_groups;

    UnionNode **rank_trees;

    UnionNode *findTreeNode(int group){
        UnionNode *node = rank_trees[group];
        while(node->next != nullptr && node->current_root != nullptr) {
            if(node->current_root != nullptr){
                node = node->current_root;
            } else {
                node = node->next;
            }
        }

        // Updating path after finding the root
        UnionNode *updatePath = rank_trees[group];
        while(updatePath->next != nullptr) {
            updatePath->current_root = node;
            updatePath = updatePath->next;
        }

        return node;
    }

public:
    UnionTree(UnionTree const &tree) = delete;
    UnionTree &operator=(UnionTree const &tree) = delete;
    UnionTree(int groups) : num_of_groups(groups){
        rank_trees = new UnionNode*[num_of_groups];
        for(int i = 0; i < num_of_groups; i++){
            rank_trees[i] = new UnionNode();
        }
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
            smaller->current_root = bigger;

            smaller->tree->clearTree();
            delete smaller->tree;
            smaller->tree = nullptr;
        }
    }
};


#endif //EX2_UNIONTREE_H

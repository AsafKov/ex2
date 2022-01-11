#ifndef EX2_UNIONHISTS_H
#define EX2_UNIONHISTS_H

class UnionHists {
private:
    class UnionNode{
    public:
        int size;
        UnionNode *next;
        int *hist;

        explicit UnionNode(int scale): size(1), next(nullptr){
            hist = new int[scale]();
        }

        ~UnionNode(){
            next = nullptr;
            delete[] hist;
        }
    };

    UnionNode *findGroupHist(int group){
        UnionNode *node = hists[group];
        while(node->next != nullptr) {
            node = node->next;
        }

        // Updating path after finding the root
        UnionNode *updatePath = hists[group];
        UnionNode *curr_next;
        while(updatePath->next != nullptr) {
            curr_next = updatePath->next;
            updatePath->next = node;
            updatePath = curr_next;
        }

        return node;
    }

    const int num_of_groups;
    const int scale;

    UnionNode **hists;

public:
    UnionHists(UnionHists const &hists) = delete;
    UnionHists &operator=(UnionHists const &hists) = delete;
    explicit UnionHists(int groups, int scale) : num_of_groups(groups), scale(scale){
        hists = new UnionNode*[num_of_groups];
        for(int i = 0; i < num_of_groups; i++){
            hists[i] = new UnionNode(scale+1);
        }
    }
    ~UnionHists(){
        for(int i = 0; i < num_of_groups; i++){
            delete hists[i];
        }
        delete[] hists;
    }

    void increaseScoreCount(int group, int score){
        group--;
        UnionNode *node = findGroupHist(group);
        node->hist[score]++;
    }

    void decreaseScoreCount(int group, int score){
        group--;
        UnionNode *node = findGroupHist(group);
        node->hist[score]--;
    }

    void mergeGroups(int group1, int group2){
        group1--;
        group2--;
        UnionNode *group1_node = findGroupHist(group1);
        UnionNode *group2_node = findGroupHist(group2);
        if(group1_node == group2_node){
            return;
        }
        UnionNode *smaller, *bigger;
        if(group1_node != group2_node){
            if(group1_node->size > group2_node->size){
                bigger = group1_node;
                smaller = group2_node;
            } else {
                bigger = group2_node;
                smaller = group1_node;
            }

            for(int i=0; i<scale+1; i++){
                bigger->hist[i] += smaller->hist[i];
            }

            smaller->next = bigger;
            bigger->size += smaller->size;

            delete[] smaller->hist;
            smaller->hist = nullptr;
        }
    }

    void countPlayersWithScore(int group, int score, int *count_in_range, int *count_in_range_with_score){
        group--;
        UnionNode *node = findGroupHist(group);
        int count_total = 0;
        for(int i=0; i<scale + 1; i++){
            count_total += node->hist[i];
        }

        *count_in_range += count_total;
        *count_in_range_with_score += node->hist[score];
    }

    int getGroupSize(int group){
        group--;
        UnionNode *node = findGroupHist(group);
        int count = 0;
        for(int i=0; i<scale+1; i++){
            count += node->hist[i];
        }

        return count;
    }
};


#endif //EX2_UNIONHISTS_H

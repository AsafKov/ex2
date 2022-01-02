#ifndef EX2_PLAYERMANAGER_H

#define EX2_PLAYERMANAGER_H

class PlayerManager{
private:
    int num_of_groups;
    int scale;
    Node<PlayerKey> *groups_ranked_trees;

public:
    PlayerManager(int number_of_groups, int scale): num_of_groups(number_of_groups), scale(scale){}

};


#endif //EX2_PLAYERMANAGER_H

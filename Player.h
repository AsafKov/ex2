#ifndef EX1_PLAYER_H
#define EX1_PLAYER_H

class Player{
private:
    int id;
    int level, groupId;
    int score;

public:
    Player(int id, int score, int groupId) : id(id), level(0), groupId(groupId), score(score){}
    int getId() const { return this->id; }
    int getLevel() const { return this->level; }
    void setScore(int newScore){ this->score = newScore; }
    void increaseLevel(int increaseBy){
        this->level += increaseBy;
    }
    void increaseScore(int increaseBy){
        this->score += increaseBy;
    }
    void setLevel(int newLevel){ this->level = newLevel; }
    int getGroupId() const { return this->groupId; }
    void setGroupId(int newGroup){ this->groupId = newGroup;}
    int getScore() { return this->score;}
};

class PlayerKey{
private:
    const int IGNORE_ID = -1;
    int id;
    int level;

public:
    explicit PlayerKey(Player *player): id(player->getId()), level(player->getLevel()){};
    PlayerKey(int id, const int level): id(id), level(level){};
    PlayerKey(): id(-1), level(-1){};
    ~PlayerKey() = default;
    bool operator==(const PlayerKey &key) const {
        if(key.getId() == IGNORE_ID){
            return this->level == key.getLevel();
        }
        return this->level == key.getLevel() && this->id == key.getId();
    }
    bool operator<(const PlayerKey &key) const {
        if(key.getId() == IGNORE_ID){
            return this->level < key.getLevel();
        }
        return this->level < key.getLevel() || (this->level == key.getLevel() && this->id > key.getId());
    }
    bool operator>(const PlayerKey &key) const {
        if(key.getId() == IGNORE_ID){
            return this->level > key.getLevel();
        }
        return this->level > key.getLevel() || (this->level == key.getLevel() && this->id < key.getId());
    }
    int getId() const { return this->id; }
    int getLevel() const { return this->level; }
    void setLevel(int newLevel){ this->level = newLevel; }
};

#endif //EX1_PLAYER_H

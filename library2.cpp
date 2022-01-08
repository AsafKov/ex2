#include "PlayerManager.h"
#include "library2.h"

void *Init(int k, int scale){
    return new PlayerManager(k, scale);
}

StatusType MergeGroups(void *DS, int GroupID1, int GroupID2){
    if(DS == nullptr){
        return INVALID_INPUT;
    }

    return ((PlayerManager*) DS)->mergeGroups(GroupID1, GroupID2);
}

StatusType AddPlayer(void *DS, int PlayerID, int GroupID, int score){
    if(DS == nullptr){
        return INVALID_INPUT;
    }
    return ((PlayerManager*) DS)->addPlayer(PlayerID, GroupID, score);
}

StatusType RemovePlayer(void *DS, int PlayerID){
    if(DS == nullptr){
        return INVALID_INPUT;
    }
    return ((PlayerManager*) DS)->removePlayer(PlayerID);

}

StatusType IncreasePlayerIDLevel(void *DS, int PlayerID, int LevelIncrease){
    if(DS == nullptr){
        return INVALID_INPUT;
    }
    return ((PlayerManager*) DS)->increasePlayerLevel(PlayerID, LevelIncrease);
}

StatusType ChangePlayerIDScore(void *DS, int PlayerID, int NewScore){
    if(DS == nullptr){
        return INVALID_INPUT;
    }
    return ((PlayerManager*) DS)->changePlayerIDScore(PlayerID, NewScore);
}

StatusType GetPercentOfPlayersWithScoreInBounds(void *DS, int GroupID, int score, int lowerLevel, int higherLevel,
                                                double *players){
    if(DS == nullptr){
        return INVALID_INPUT;
    }
    return ((PlayerManager*) DS)->getPercentOfPlayersWithScoreInBounds(GroupID, score, lowerLevel, higherLevel, players);
}

StatusType AverageHighestPlayerLevelByGroup(void *DS, int GroupID, int m, double * level){
    if(DS == nullptr){
        return INVALID_INPUT;
    }
    return ((PlayerManager*) DS)->averageHighestPlayerLevelByGroup(GroupID, m, level);
}

StatusType GetPlayersBound(void *DS, int GroupID, int score, int m,
                           int * LowerBoundPlayers, int * HigherBoundPlayers) {
    if(DS == nullptr){
        return INVALID_INPUT;
    }
    return SUCCESS;
}

void Quit(void** DS){
    PlayerManager* pm = (PlayerManager*)*DS;
    pm->quit();
    delete pm;
    //((PlayerManager*)*DS)->Quit();
    //((PlayerManager*)*DS)->~PlayerManager();
    *DS = nullptr;
}


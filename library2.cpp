#include "PlayerManager.h"
#include "library2.h"

void *Init(int k, int scale){
    return nullptr;
}

StatusType MergeGroups(void *DS, int GroupID1, int GroupID2){
    return SUCCESS;
}

StatusType AddPlayer(void *DS, int PlayerID, int GroupID, int score){
    return SUCCESS;
}

StatusType RemovePlayer(void *DS, int PlayerID){
    return SUCCESS;
}

StatusType IncreasePlayerIDLevel(void *DS, int PlayerID, int LevelIncrease){
    return SUCCESS;
}

StatusType ChangePlayerIDScore(void *DS, int PlayerID, int NewScore){
    return SUCCESS;
}

StatusType GetPercentOfPlayersWithScoreInBounds(void *DS, int GroupID, int score, int lowerLevel, int higherLevel,
                                                double * players){
    return SUCCESS;
}

StatusType AverageHighestPlayerLevelByGroup(void *DS, int GroupID, int m, double * level){
    return SUCCESS;
}

StatusType GetPlayersBound(void *DS, int GroupID, int score, int m,
                           int * LowerBoundPlayers, int * HigherBoundPlayers) {
    return SUCCESS;
}

void Quit(void** DS){
}


// killAllOnCap.cpp
//

#include "bzfsAPI.h"

class killAllOnCap : public bz_Plugin
{
public:
    virtual const char* Name (){return "KillAllOnCap";}
    virtual void Init ( const char* config);
    virtual void Event ( bz_EventData *eventData );
    virtual void Cleanup ( void );
// Variables
int noLossOnDeath = 1;
// Utility functions
void killAll(int noLoss) {
    bz_APIIntList *playerList = bz_newIntList();
    if (playerList) {
    
    bz_getPlayerIndexList(playerList);
    for ( unsigned int i = 0; i < playerList->size(); i++) {
    int targetID = (*playerList)[i];
    bz_BasePlayerRecord *playRec = bz_getPlayerByIndex ( targetID );

        if (playRec != NULL) {
            if (playRec->spawned) {
                bz_killPlayer(targetID, true, BZ_SERVER);
                if (noLoss)
                    bz_incrementPlayerLosses(targetID, -1);
            }
        }
        bz_freePlayerRecord(playRec);
    }
    // Originally deleted playerlist here.
    }
        bz_deleteIntList(playerList);
}
// 
};

BZ_PLUGIN(killAllOnCap)

void killAllOnCap::Init( const char* commandLine ) {
    if (strlen(commandLine) != 0)
        noLossOnDeath = 0;
    Register(bz_eCaptureEvent);
}

void killAllOnCap::Cleanup (void) {
    Flush();
}

void killAllOnCap::Event ( bz_EventData *eventData ) {
    if (eventData->eventType !=  bz_eCaptureEvent) 
        return;

    bz_CTFCaptureEventData_V1* captureData = (bz_CTFCaptureEventData_V1*)eventData;
        killAll(noLossOnDeath);
}
  

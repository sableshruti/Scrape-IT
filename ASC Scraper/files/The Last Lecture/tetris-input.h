#include "tetris-blocks.h"
#include "tetris-anim.h"


class QuitException {};
class BadKeyException {};

class playerInput {

    boardDrawer* brdDrw; // to display notifications/prompts
    
    //TODO: Allow overriding via a configuration file
    KeySym DOWNKEY = XK_Return, LEFTKEY = XK_Left, RIGHTKEY = XK_Right,  ROTCWKEY = XK_Up, ROTACWKEY = XK_Down, DROPKEY = XK_space,
                 PAUSEKEY = XK_p, QUITKEY = XK_q, RESUMEKEY = XK_Return, CONFIRMKEY = XK_Return, CANCELKEY = XK_Escape;

    bool getKeySym(XEvent& ev, KeySym& ks) {
        if(!keyPressEvent(ev))
            return false;
        auto kev = ev.xkey;
        ks = XLookupKeysym(&kev,0);
        return true;
    }

    bool checkKeySym(XEvent& ev, KeySym kchk) { KeySym k; return getKeySym(ev,k) && k==kchk; }
    bool isQuit(XEvent& ev) { return checkKeySym(ev,QUITKEY); }
    bool isConfirm(XEvent& ev) { return checkKeySym(ev,CONFIRMKEY); }
    bool isCancel(XEvent& ev) { return checkKeySym(ev,CANCELKEY); }
    bool isPause(XEvent& ev) { return checkKeySym(ev,PAUSEKEY); }
    bool isResume(XEvent& ev) { return checkKeySym(ev,RESUMEKEY); }

    // Checks if quitting (with confirmation), and if so throws QuitException
    // Returns true if updateInfo called
    bool quit(XEvent& ev) {
        if(!isQuit(ev)) return false;
        brdDrw->updateInfo("Quit? Enter/Esc");
        do {
            nextEvent(ev);
        } while(!isConfirm(ev) && !isCancel(ev));
        if(isConfirm(ev)) throw QuitException();
        brdDrw->updateInfo("Not quitting");
        return true;
    }


    bool pause(XEvent& ev) {
        if (!isPause(ev)) return false;
        brdDrw->updateInfo("Enter to resume");
        do {
            nextEvent(ev);
            if(quit(ev)) // allow quitting while paused. Returns true if quitting cancelled
                brdDrw->updateInfo("Enter to resume"); // Redraw info 
        } while(!isResume(ev));
        return true;
    }

    move_t moveFromKey(XEvent& ev) {
        KeySym k;
        if (!getKeySym(ev,k))
            throw BadKeyException();
        if(k==LEFTKEY) return lft;
        if(k==RIGHTKEY) return rgt;
        if(k==ROTCWKEY) return rotCW;
        if(k==ROTACWKEY) return rotACW;
        if(k==DROPKEY) return drp;
        if(k==DOWNKEY) return dwn;
        return none;
    }

public:

    playerInput (boardDrawer* b) : brdDrw(b) {}


    void start() {
        XEvent ev;
        brdDrw->updateInfo("Press Enter to start");
        do { 
            nextEvent(ev);
            if(quit(ev)) // tried to quit. prompt again
                brdDrw->updateInfo("Press Enter to start");
        } while(!isConfirm(ev));
        brdDrw->showScore();
    }


    void flushMoves() {
        XEvent ev;
        while(checkEvent(ev)) {
            pause(ev);
            quit(ev);
        }
    }

    move_t nextMove() {
        XEvent ev;
        // look for next valid key available
        while (checkEvent(ev) ) {
            if(pause(ev) || quit(ev)) { // if a pause/quit event, returns with resume/cancel event in ev
                brdDrw->showScore();  // redraw the score
                continue;
            }
   try { return moveFromKey(ev); }
   catch(BadKeyException) {}
        }
        return none;
    }
};
#include <iostream>
using std::cout; 
using std::cerr; 
using std::cin; 
using std::endl; 
#include "tetris-game.h"
#include "tetris-history.h"



int main() {
    history H("tetris-scores.txt");
    game G;
    try {
        while(1) {
            int s = G.play();
            if(H.eligible(s)) {
                record r;
                r.setTime();
                r.score = s;
                cout << "Congratulations on a high-score! Please enter your name: ";
                std::getline(cin >> std::ws,r.name);
    if(r.name=="") r.name = "-";
                H.addRecord(r);
            }
        }
    } catch(QuitException q) {
            cout << "Bye!" << endl;
    }
}
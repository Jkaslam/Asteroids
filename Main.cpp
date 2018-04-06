#include <iostream>
#include "Game.h"
#include "Ship.h"

using namespace std;
using namespace asteroids;

/** 
 * @namespace asteroids A version of the classic asteroids computer
 * game.
 *
 * @author Jai Aslam
 */

/**
 * The main program for asteroids. Runs the asteroids game. 
 *
 * @return The status code. Normal is 0 and 1 is bad. 
 */
int main() {
  try {

    Game game;

    while (true) {
      game.processRequests();
      game.refresh();
    }
  }
  catch (const exception& e) {
    cerr << e.what() << endl;
    return 1;
  }
}

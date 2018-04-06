#ifndef ASTEROIDS_GAME_H
#define ASTEROIDS_GAME_H

#include <vector>
#include <algorithm>
#include <SDL2/SDL_ttf.h>

#include "Ship.h"

class SDL_Window;
class SDL_Renderer;
class SDL_Texture;

namespace asteroids {

/**
 * An asteroids game which allows the player to move a space ship around.
 * The object of the game is to destroy as many asteroids as you can until
 * you run out of lives. In order to move around use the left and right arrow keys
 * to change the direction being moved in, the up and down keys to move forwards or backwards
 * and the space bar to fire a bullet. A life is lost when the ship collides with an asteroid.
 *
 * @author Jai Aslam
 */
class Game {
public:
  /**
  * Initializes a game of asteroids where the player controls a space ship to 
  * destroy asteroids which are floating around. The default size of the screen is
  * set here as well. 
  */
  Game(/** The width of the game screen */int width = 640, /** The height of the game screen */int height = 480);
  
  /**
  * Destructs the Game object.
  */
  ~Game();
  
  /**
  * Spawns asteroids of a given size at random positions. 
  */
  void spawnAsteroids(/** The radius of the asteroids being spawned. */int radius) noexcept;

  /**
  * Closes the game. Closes SDL libraries and TTF resources. 
  */
  void close() noexcept;

  /**
  * Refreshes the game screen by moving asteroids, ships and bullets as well as 
  * the score and number of lives. 
  */
  void refresh(); 

  /**
  * Deals with all of the user interactions with the game such as moving
  * the ship and firing bullets. 
  */
  void processRequests() noexcept;
 
  /**
  * Spawns a bullet from the front of the ship traveling in the
  * direction that the front of the ship was facing. 
  */
  void fireBullet() noexcept;

  /**
  * Checks if any bullets and asteroids are colliding, if they are
  * removes the bullets and asteroids. It also increases the score
  * and spawns more asteroids if none are left. 
  */
  void checkBulletAsteroidCollisions() noexcept;

  /**
  * Checks if the ship is colliding with any asteroids. If the ship is,
  * then restart the level by respawning new asteroids and updates
  * the number of lives. 
  */
  void checkShipAsteroidCollisions() noexcept; 

  /**
  * Updates the score based on the size of the given asteroid that was destroyed.
  */
  void updateScore(/** The asteroid that was destroyed by the bullet */std::shared_ptr<Asteroid> ast) noexcept;

  /**
  * Decreases the number of lives that the player has left. 
  */
  void updateLives() noexcept;
  
  /**
  * Checks if the player is still alive or not. 
  */
  bool stillAlive() noexcept;

  /**
  * Draws the score and number of lives on the game board. 
  */
  void drawScoreAndLives();

  /**
  * Draws the game over screen which includes the player's final score. 
  */
  void drawGameOver();
private:
  /** The window which the game is being displayed on */
  SDL_Window* window_ = nullptr;
  
  /** The renderer which renders the game */
  SDL_Renderer* renderer_ = nullptr;

  /** The width of the screen */
  const int width_ = 0;

  /** The height of the screen */
  const int height_ = 0;

  /** The ship controlled by the player */
  Ship player_;

  /** The current score of the game */
  int score_;
  
  /** The current number of lives left */
  int lives_;

  /** The current level the game is on which determines how many asteroids are spawned */
  int level_;
  
  /** The asteroids which are on the screen */
  std::vector<std::shared_ptr<Asteroid>> asteroids_;
  
  /** The bullets which are on the screen */
  std::vector<std::shared_ptr<Bullet>> bullets_;

  /** The font which all of the text is rendered in */
  TTF_Font* sans_ = nullptr;

  /**
  * Clear the background to opaque black.
  */
  void clearBackground();

};
}

#endif

#ifndef ASTEROIDS_SHIP_H
#define ASTEROIDS_SHIP_H

#include <SDL2/SDL.h>
#include <memory>
#include <vector>

#include "Bullet.h"
#include "Asteroid.h"

namespace asteroids {

/**
 * Represents a spaceship controlled by the player of the game. 
 * The ship can move around the screen, rotate  and shoot bullets 
 * according to the commands of the user. 
 *
 * @author Jai Aslam
 */
class Ship {
public:
  /**
  * Constructs a ship starting in the given initial position on the 
  * screen. 
  */
  Ship(/** The initial x coordinate of the ship. */ int initialX, /** The initial y coordinate of the ship */ int initialY, /** Length from the centroid to the front of the ship. */ int size);

  /**
  * Destructs a ship object. 
  */
  ~Ship();

  /**
  * @returns the current x coordinate of the ship.
  */
  int getX() const noexcept; 

  /**
  * @returns the current y coordinate of the ship.
  */
  int getY() const noexcept;

  /**
  * @returns the current angle of the ship. 
  */
  int getAngle() const noexcept;

  /**
  * Updates the current position of the ship given
  * the magnitude of the velocity vector.   
  */
  void updatePosition(/** The speed that the ship is moving at */ int velocityMagnitude) noexcept;

  /**
  * Updates the current angle that ship is facing given
  * the radians it should rotate. 
  */
  void updateAngle(/** The angle in radians to rotate the ship */ int radiansToRotate) noexcept;
  
  /**
  * Checks if a ship is colliding with the given asteroid. 
  */
  bool collides(/** The asteroid to check if the ship is colliding with it */std::shared_ptr<Asteroid> ast) const noexcept;

  
  /**
  * Draws the ship from its current position.
  */ 
  void draw(/** The SDL renderer that the ship is drawn onto.  */ SDL_Renderer* r) noexcept;

  /**
  * Rotates the given point about the center point by angle radians. 
  */
  SDL_Point rotateAboutCenter(/** The x coordinate of the given point */int pointX, /** The y coordinate of the given point */ int pointY, /** The angle to rotate the given point */int angle) noexcept;


private:
  /** The current x coordinate of the ship. */
  int x_;
 
  /** The current y coordinate of the ship. */
  int y_;

  /** How far the ship has been rotated currently in radians. */
  int angle_;

  /** The size of the ship. This is the distance between the centroid
  * and the front of the ship. */
  int size_;
 
  /**
   * Makes the ship wrap around if it goes off the edge of the screen.
   */
  void wrapAroundScreen() noexcept;
};
}

#endif

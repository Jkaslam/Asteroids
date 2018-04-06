#ifndef ASTEROIDS_ASTEROID_H
#define ASTEROIDS_ASTEROID_H

#include "Bullet.h"
#include <memory>

namespace asteroids {

/**
 * Represents an asteroid traveling through space. Asteroids
 * can be of variable sizes.
 *
 * @author Jai Aslam  
 */
class Asteroid {
public:
  /**
  * Constructs an asteroid starting in the given initial position on the 
  * screen. 
  */
  Asteroid(/** The initial x coordinate of the asteroid */int initialX, /** The initial y coordinate of the asteroid */int initialY, /** The initial radius of the asteroid */int radius, /** The initial direction the asteroid is traveling  */int direction);

  /**
  * Destructs an asteroid object. 
  */
  ~Asteroid();

  /**
  * @returns the current x coordinate of the asteroid.
  */
  int getX() const noexcept; 

  /**
  * @returns the current y coordinate of the asteroid. 
  */
  int getY() const noexcept;

  /**
  * @returns the radius of the asteroid. 
  */
  int getRadius() const noexcept;

  /**
  * @returns the direction in radians that the asteroid is traveling in.
  */
  int getDirection() const noexcept;

  /**
  * Updates the current position of the given
  * the magnitude of the velocity vector.   
  */
  void updatePosition(/** The current speed the ship is moving at */int velocityMagnitude) noexcept;

  /**
  * Checks if the given bullet collides with the asteroid. 
  */
  bool collides(/** The given bullet */ std::shared_ptr<Bullet> bullet) const noexcept;

  /**
  * Draws the ship from its current position.
  */ 
  void draw(/** The renderer which draws the asteroid */ SDL_Renderer* r) noexcept;

private:
  /** The current x coordinate of the asteroid. */
  int x_;
 
  /** The current y coordinate of the asteroid. */
  int y_;

  /** The radius of the asteroid. */
  int radius_;

  /** The direction in degrees that the asteroid is traveling in. */
  int direction_;

  /**
  * Rotates the given point about the center point by the angle the asteroid
  * is facing.
  */
  SDL_Point rotateAboutCenter(/** X coordinate of point */ int pointX, /** Y coordinate of point */ int pointY, /** Angle of rotation */ int angle);

  /**
  * Makes the ship wrap around if it goes off the edge of the screen.
  */
  void wrapAroundScreen() noexcept; 
};
}

#endif

#ifndef ASTEROIDS_BULLET_H
#define ASTEROIDS_BULLET_H

#include <SDL2/SDL.h>

namespace asteroids {

/**
 * Represents an bullet traveling through space which can destroy asteroids.
 *
 * @author Jai Aslam  
 */
class Bullet {
public:
  /**
  * Constructs a bullet starting in the given initial position on the 
  * screen heading in the given direction.
  */
  Bullet(/** The initial x position of the bullet */int initialX, /** The initial y position of the bullet */int initialY, /** The initial direction the bullet is traveling */int initialDirection);

  /**
  * Destructs a bullet object. 
  */
  ~Bullet();

  /**
  * @returns the current x coordinate of the bullet.
  */
  int getX() const noexcept; 

  /**
  * @returns the current y coordinate of the bullet. 
  */
  int getY() const noexcept;

  /**
  * Updates the current position of bullet the given
  * the magnitude of the velocity vector.   
  */
  void updatePosition(/** The initial speed of the bullet */int velocityMagnitude) noexcept;

  /**
  * Draws the bullet to the given SDL renderer. 
  */
  void draw(/** The renderer to draw the bullet on */SDL_Renderer* r) noexcept;

  /**
  * Checks if the bullet is on the screen.
  */
  bool bulletOnScreen() const noexcept;

private:
  /** The current x coordinate of the bullet. */
  int x_;
 
  /** The current y coordinate of the bullet. */
  int y_;

  /** The direction in degrees that the bullet is traveling in. */
  int direction_;

};
}
#endif

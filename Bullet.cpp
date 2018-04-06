#include <math.h>
#include "Bullet.h"

using namespace std;
using namespace asteroids;

Bullet::Bullet(int initialX, int initialY, int initialDirection) {
  //Initializes the initial position of the bullet and the direction
  //it is traveling in.
  x_ = initialX;
  y_ = initialY;
  direction_ = initialDirection;
}

Bullet::~Bullet() {}

int Bullet::getX() const noexcept {
  //Returns the current x coordinate of the bullet.
  return x_;
} 

int Bullet::getY() const noexcept {
  //Returns the curreny y coordinate of the bullet.
  return y_;
}

void Bullet::updatePosition(int velocityMagnitude) noexcept {
  //Updates x position using the x-component of velocity
  x_ += velocityMagnitude * cos(direction_);
  //Updates y position using the y-component of velocity
  y_ += velocityMagnitude * sin(direction_);
}

void Bullet::draw(SDL_Renderer* r) noexcept {
  //Draws the bullet as a short line between two points.
  SDL_Point bulletPoints[2];
  bulletPoints[0] = {getX(), getY()};
  bulletPoints[1] = {(int) (getX() + 3 * cos(direction_)),(int) (getY() + 3 * sin(direction_))};

  //Draws the bullet in white
  SDL_SetRenderDrawColor(r, 255, 255, 255, 255);
  SDL_RenderDrawLines(r, bulletPoints, 2);
}

bool Bullet::bulletOnScreen() const noexcept {
  //If the bullet has gone off the right or left of the screen
  //then it is not on the screen.
  if (x_ > 640 || x_ < 0) {
    return false;
  }
  //If the bullet has gone off the bottom or the top of the screen
  //then it is not on the screen.
  if (y_ > 480 || y_ < 0) {
    return false;
  }

  return true;
}



#include <SDL2/SDL.h>
#include <math.h>
#include <iostream>

#include "Asteroid.h"
using namespace std;
using namespace asteroids;

Asteroid::Asteroid(int initialX, int initialY, int initialRadius, int direction) {
  //Initializes the initial position, radius and direction of the asteroid
  x_ = initialX;
  y_ = initialY;
  radius_ = initialRadius;
  direction_ = direction;
}

Asteroid::~Asteroid() {}

int Asteroid::getX() const noexcept {
  //Returns the current x coordinate
  return x_;
}

int Asteroid::getY() const noexcept {
  //Returns the current y coordinate
  return y_;
}

int Asteroid::getRadius() const noexcept {
  //Returns the current radius
  return radius_;
}

int Asteroid::getDirection() const noexcept {
  //Returns the current direction of the asteroid
  return direction_;
}

void Asteroid::updatePosition(int velocityMagnitude) noexcept {
  //Updates x position using the x-component of velocity
  x_ += velocityMagnitude * cos(direction_);
  //Updates y position using the y-component of velocity
  y_ += velocityMagnitude * sin(direction_);

  //Wraps the asteroids around the screen
  wrapAroundScreen();
}

bool Asteroid::collides(shared_ptr<Bullet> bullet) const noexcept {
  //If the bullt point is within the radius distance of the asteroid then the are colliding
  return pow(radius_, 2) >= pow(bullet->getX() - getX(), 2) + pow(bullet->getY() - getY(), 2);
}
 
void Asteroid::draw(SDL_Renderer* r) noexcept {
  //Initializes the array which will hold the points of the asteroid
  SDL_Point asteroidPoints[4];
  
  //Gets the coordinate of the front point of the asteroid
  int frontX = getX() + radius_;
  int frontY = getY(); 
  
  //Finds the coordinates of the other two points of the asteroids by rotating the front point
  asteroidPoints[0] = rotateAboutCenter(frontX, frontY, direction_);
  asteroidPoints[1] = rotateAboutCenter(asteroidPoints[0].x, asteroidPoints[0].y, 2);
  asteroidPoints[2] = rotateAboutCenter(asteroidPoints[0].x, asteroidPoints[0].y, 4);
  asteroidPoints[3] = asteroidPoints[0];

  //Draw the asteroid in green
  SDL_SetRenderDrawColor(r, 0, 255, 0, 0);
  SDL_RenderDrawLines(r, asteroidPoints, 4);
}

SDL_Point Asteroid::rotateAboutCenter(int pointX, int pointY, int angle) {
  //Center coordinates
  int centerX = getX();
  int centerY = getY();

  //Translation according to the center as well as applying the 2D rotation matrix
  //to the position vector
  int finalX = cos(angle) * (pointX - centerX) - sin(angle) * (pointY - centerY) + centerX;
  int finalY = sin(angle) * (pointX - centerX) + cos(angle) * (pointY - centerY) + centerY;

  SDL_Point rotatedPoint = {finalX, finalY};
  return rotatedPoint;
}

void Asteroid::wrapAroundScreen() noexcept {
  //If the asteroid goes off the left side, send it to the right side
  if (x_ < 0) {
    x_ = 640 + x_;
  }
  //If the asteroid goes off the right side, send it to the left side
  if (x_ > 640) {
    x_ %= 640;
  }
  //If the asteroid goes off the bottom, send it to the top
  if (y_ < 0) {
    y_ = 480 + y_;
  }
  //If the asteroid goes off the top, send it to the bottom
  if (y_ > 480) {
    y_ %= 480;
  }
}

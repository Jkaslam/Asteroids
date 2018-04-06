#include <math.h>
#include "Ship.h"

using namespace std;
using namespace asteroids;

Ship::Ship(int initialX, int initialY, int size) {
  //Sets the initial coordinates and size of the ship
  x_ = initialX;
  y_ = initialY;
  size_ = size;
}

Ship::~Ship() {}

int Ship::getX() const noexcept {
  //Returns the current x coordinate
  return x_;
}

int Ship::getY() const noexcept {
  //Returns the current y coordinate
  return y_;
}

int Ship::getAngle() const noexcept {
  //Returns the current angle of the ship
  return angle_;
}

void Ship::updatePosition(int velocityMagnitude) noexcept {
  //Updates x position using the x-component of velocity
  x_ += (velocityMagnitude * cos(angle_));
 
  //Updates y position using the y-component of velocity
  y_ += (velocityMagnitude * sin(angle_));
  
  //Wraps the ship's movement around the screen
  wrapAroundScreen();
}

void Ship::updateAngle(int radiansToRotate) noexcept {
  angle_ += radiansToRotate;
  //No higher than 2pi = 6.28
  angle_ %= 6; 
}

bool Ship::collides(shared_ptr<Asteroid> ast) const noexcept {
 //Gives the asteroid and ship a bounding circle and checks
 //if the cirlces intersect
 return pow(ast->getRadius() + size_, 2) >= pow(getX() - ast->getX(), 2) + pow(getY() - ast->getY(), 2);
}



void Ship::draw(SDL_Renderer* r) noexcept {
  //We separate the ship into its 3 lines. 
  //The back will be red so that we know where the front is.
  SDL_Point shipSide1[2];
  SDL_Point shipSide2[2];
  SDL_Point shipBack[2]; 
  
  //Assume we are pointing to the right (i.e. 0 radians)
  //So our front point is just the distance to the right along
  //the x direction at the same height
  int frontX = getX() + size_;
  int frontY = getY(); 
  
  //Now we rotate about our centroid by however much the ship has been rotated
  //by the player so far
  //The other two points will be the front of our ship rotated 2pi/3 and 4pi/3
  //radians - with integer division these truncate to 2 and 4

  //The first side of the ship
  shipSide1[0] = rotateAboutCenter(frontX, frontY, angle_);
  shipSide1[1] = rotateAboutCenter(shipSide1[0].x, shipSide1[0].y, 2);

  //The back of the ship (i.e. the side behind the front of the ship)
  shipBack[0] = shipSide1[1];
  shipBack[1] = rotateAboutCenter(shipSide1[0].x, shipSide1[0].y, 4);

  //The other side of the ship 
  shipSide2[0] = shipBack[1];
  shipSide2[1] = shipSide1[0];

  //Draw the sides of the ship in white
  SDL_SetRenderDrawColor(r, 255, 255, 255, 255);
  SDL_RenderDrawLines(r, shipSide1, 2);
  SDL_RenderDrawLines(r, shipSide2, 2);
  
  //Draw the back of the ship in red
  SDL_SetRenderDrawColor(r, 255, 0, 0, 0);
  SDL_RenderDrawLines(r, shipBack, 2);
   
}

SDL_Point Ship::rotateAboutCenter(int pointX, int pointY, int angle) noexcept {

  //Center coordinates
  int centerX = getX();
  int centerY = getY();

  //Translation according to the center as well as applying the 2D rotation matrix
  //to the position vector
  int finalX = cos(angle) * (pointX - centerX) - sin(angle) * (pointY - centerY) + centerX;
  int finalY = sin(angle) * (pointX - centerX) + cos(angle) * (pointY - centerY) + centerY;

  //Packages and returns the new point in an SDL_Point.
  SDL_Point rotatedPoint = {finalX, finalY};
  return rotatedPoint;
}

void Ship::wrapAroundScreen() noexcept {
  //Checks if the ship has gone off the left of the screen
  //if so puts it on the right side of the screen
  if (x_ < 0) {
    x_ = 640 + x_;
  }
  //Checks if the ship has gone off the right side of the screen
  //if so puts it on the left side of the screen
  if (x_ > 640) {
    x_ %= 640;
  }
  //Checks if the ship has gone off the bottom of the screen
  //if so puts the ship on the top of the screen
  if (y_ < 0) {
    y_ = 480 + y_;
  }
  //Checks if the ship has gone off the top of the screen
  //if so puts it on the bottom of the screen
  if (y_ > 480) {
    y_ %= 480;
  }
}

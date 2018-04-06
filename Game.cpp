#include <SDL2/SDL.h>
#include <stdexcept>
#include <iostream>
#include <cstdlib>
#include <time.h>
#include <string>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include "Game.h"
#include "Ship.h"

using namespace std;
using namespace asteroids;

//A game starts with a ship in the center of the grid
//3 lives
//1 asteroids
Game::Game(int width, int height)
  : width_(width), height_(height), player_(Ship(width_/2, height_/2, 10)), score_(0), lives_(3), level_(1){

  
  //Create a large initial asteroid with size 50
  spawnAsteroids(50);

  //Initialize SDL2
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    throw domain_error(string("SDL Initialization failed due to: ") + SDL_GetError());
  }

  //Initializes true type font for drawing the score and lives
  if (TTF_Init() == -1) {
    throw domain_error(string("SDL_ttf could not initialize due to: ") + TTF_GetError());
  }

  //Initializes drawing imgs for rendering true type font
  if (IMG_Init(IMG_INIT_PNG) == -1) {
   throw domain_error(string("SDL_image could not initialize due to: ") + IMG_GetError()); 
  }  

  //Construct the screen window
  window_ = SDL_CreateWindow("Asteroids", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width_, height_, SDL_WINDOW_SHOWN);

  //If the window could not be created for some reason displays the reason why
  if (!window_) {
    close();
    throw domain_error(string("Unable to create the window due to: ") + SDL_GetError());
  }

  //Constructs the renderer which will draw the game
  renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_SOFTWARE);
  
  //If the renderer could not be constructed for some reason displays the reason why
  if (!renderer_) {
    close();
    throw domain_error(string("Unable to create the renderer due to: ") + SDL_GetError());
  }

  //Initializes the font which will be used to draw the score and the number of lives
  sans_ = TTF_OpenFont("Sans.ttf", 24);
  
  //Clear the window?
  clearBackground();
}
  
Game::~Game() {
  //Closes the game upon destruction of the game object.
  close();
}

void Game::drawScoreAndLives() {
  // Creates a surface with the text from the score 
  SDL_Surface* surfaceScore = TTF_RenderText_Solid(sans_, ("Score: " + to_string(score_)).c_str(), {255, 255, 255});
  
  //Turns the surface into a texture
  SDL_Texture* message = SDL_CreateTextureFromSurface(renderer_, surfaceScore);
  //Frees the surface so we don't have a memory leak from it
  free(surfaceScore);

  //Constructs the rectangle that the message will live in. 
  SDL_Rect messageRect;
  messageRect.x = 0;
  messageRect.y = 0;
  messageRect.w = 50;
  messageRect.h = 50;
 
  //Constructs a surface with text from the number of lives left
  SDL_Surface* surfaceLives = TTF_RenderText_Solid(sans_, ("Lives: " + to_string(lives_)).c_str(), {255, 255, 255});
  
  //Constrcts a texture from the surface and frees the surface
  SDL_Texture* message2 = SDL_CreateTextureFromSurface(renderer_, surfaceLives);
  free(surfaceLives);

  //Constructs the rectangle that the message will live in
  SDL_Rect messageRect2;
  messageRect2.x = width_ - 60;
  messageRect2.y = 0;
  messageRect2.w = 50;
  messageRect2.h = 50;
  
  //Displays the score and the number of lives on the screen
  SDL_RenderCopy(renderer_, message, NULL, &messageRect); 
  SDL_RenderCopy(renderer_, message2, NULL, &messageRect2);

   
}

void Game::drawGameOver() {
  //Constructs the surface containing the text from the gameover screen 
  SDL_Surface* surfaceGameOver = TTF_RenderText_Solid(sans_, ("Game Over, Score: " + to_string(score_)).c_str(), {255, 255, 255});
  
  //Creates a texture from the surface and frees the surface to
  //prevent memory leaks
  SDL_Texture* message = SDL_CreateTextureFromSurface(renderer_, surfaceGameOver);
  free(surfaceGameOver);

  //Constructs the rectangle that the gameover text will live in 
  SDL_Rect messageRect;
  messageRect.x = width_/2 - 200;
  messageRect.y = height_/2 - 100;
  messageRect.w = 400;
  messageRect.h = 200;
 
  //Renders the game over message to the screen
  SDL_RenderCopy(renderer_, message, NULL, &messageRect);   
}


void Game::spawnAsteroids(int radius) noexcept {
  //Initializes random seed for number generator
  srand(time(NULL));
  
  //Allows asteroids to spawn on each other for now  
  for (auto i = 0; i < level_; i++) {
    asteroids_.push_back(make_shared<Asteroid>(Asteroid(rand() % width_ + radius, rand() % height_ + radius, radius, rand() % 6 ))); 
  }
}

void Game::close() noexcept {
  //Destroy the renderer and window, and set the variables to nullptr
  //to ensure idempotence
  if (renderer_) {
    SDL_DestroyRenderer(renderer_);
    renderer_ = nullptr;
  }

  if (window_) {
    SDL_DestroyWindow(window_);
    window_ = nullptr;
  }

  //Closes the font that was used to render text
  TTF_CloseFont(sans_);

  //Quits out of true type font, images and sdl
  TTF_Quit();
  IMG_Quit();
  SDL_Quit();
}

void Game::refresh() {
  //If we are still displaying to the screen
  if (renderer_) {
    //Clear the background
    clearBackground();

    if (stillAlive()) {
      //Draw the ship
      player_.draw(renderer_);
    
      //Draws all of the asteroids currently on the screen
      //Updates their position as well
      for (auto asteroid : asteroids_) {
        asteroid->draw(renderer_);
        asteroid->updatePosition(2);
      }
    
      //Draws the bullets that the ship has fired if they are on screen
      vector<int> bltIdxOffScreen;
      for (unsigned i = 0; i < bullets_.size(); i++) {
        if (bullets_.at(i)->bulletOnScreen()) {
          bullets_.at(i)->draw(renderer_);
          bullets_.at(i)->updatePosition(7);
        }

        else {
          //Saves all the indices of the bullets that went off screen
          bltIdxOffScreen.push_back(i);
        }
      }

      //Checks about all of the collisions between bullets and asteroids
      //as well as asteroids and the ship
      checkBulletAsteroidCollisions();
      checkShipAsteroidCollisions();

      //Removes all the bullets that are off the screen from the ship
      for (signed i = bltIdxOffScreen.size() - 1; i > -1 ; i--) {
        bullets_.erase(bullets_.begin() + bltIdxOffScreen.at(i));
      } 
    
      //If there are no asteroids left on the screen
      if (asteroids_.size() == 0) {
        //Increase the level and spawn new asteroids
        level_ ++;
        spawnAsteroids(50);
      }
        //Display the current score and number of lives left
        drawScoreAndLives();
     }
     else {
       //If the player no longer has lives then draw trhe game over screen
       drawGameOver();
     }
      //Displays the renderer info to the screen
      SDL_RenderPresent(renderer_);
    }
}

void Game::fireBullet() noexcept {
  //The front of the ship
  SDL_Point front = player_.rotateAboutCenter(player_.getX() + 5, player_.getY(), player_.getAngle());

  //Adds a bullet to the list of bullets starting at the front of the
  //gun and heading in the direction the ship was pointing. 
  bullets_.push_back(make_shared<Bullet>(Bullet(front.x, front.y, player_.getAngle())));

}

void Game::checkShipAsteroidCollisions() noexcept {
  //Checks if any of the player is colliding with any of the asteroids
  //if so restart the level and decrease the number of lives left. 
  for (unsigned int i = 0; i < asteroids_.size(); i++) {
    if (player_.collides(asteroids_.at(i))) {
      asteroids_.clear();
      level_--;
      lives_--;
    }
  }  
}

void Game::checkBulletAsteroidCollisions() noexcept {
  //The asteroids and bullets that are colliding with
  //one another
  vector<int> collidingAsteroids;
  vector<int> collidingBullets;
  
  //Runs through all the asteroids
  for (unsigned i = 0; i < asteroids_.size(); i++) {
   //Runs through all the bullets
   for (unsigned j = 0; j < bullets_.size(); j++) {
     //If there are any collisions between the asteroids and bullets then note which ones
     if (asteroids_.at(i)->collides(bullets_.at(j))) {
       collidingAsteroids.push_back(i);
       if (find(collidingBullets.begin(), collidingBullets.end(), j) == collidingBullets.end()) {
         collidingBullets.push_back(j);
       }
       break;
     }
   }
  }

  //Runs through all of the asteroids that are colliding
  for (int ct = collidingAsteroids.size() - 1; ct > -1; ct--) {
    //The current asteroid that is colliding with a bullet
    auto currAst = asteroids_.at(collidingAsteroids.at(ct));

    //Updates the score based on the size of the asteroid that was destroyed
    updateScore(currAst);

    //If the asteroids are big enough make them break apart into 3 smaller asteroids traveling
    //in random directions
    if (currAst->getRadius()/2 > 10) {
      asteroids_.push_back(make_shared<Asteroid>(Asteroid(currAst->getX() + (currAst->getRadius() / 2), currAst->getY(), currAst->getRadius()/2, rand() % 6)));
      asteroids_.push_back(make_shared<Asteroid>(Asteroid(currAst->getX() - (currAst->getRadius() / 2), currAst->getY(), currAst->getRadius()/2, rand() % 6)));
      asteroids_.push_back(make_shared<Asteroid>(Asteroid(currAst->getX(), currAst->getY() - (currAst->getRadius() / 2), currAst->getRadius()/2, rand() % 6)));
    }
    //Removes all the asteroids from the screen that were colliding with bullets
    asteroids_.erase(asteroids_.begin() + collidingAsteroids.at(ct));
  }

  //Runs through all the bullets that are colliding with asteroids and removes
  //them from the screen
  for (int k = collidingBullets.size() - 1; k > -1; k--) {
    bullets_.erase(bullets_.begin() + collidingBullets.at(k));
  }
}

void Game::updateScore(shared_ptr<Asteroid> ast) noexcept {
  //Updates the score inversely proportional to the size of the asteroid
  //that was exploded
  score_ += 200/ast->getRadius();
}

void Game::updateLives() noexcept {
  //Player loses a life after colliding with an asteroid
  lives_--;
}

bool Game::stillAlive() noexcept {
  //Returns whether or not the player still has lives left. 
  return lives_ > 0; 
}

void Game::processRequests() noexcept {
  //Remove one event from the queue
  SDL_Event event;
  while (SDL_PollEvent(&event) != 0) {
    //The type determines what kind of request occurred

    switch (event.type) {
    //The window was closed
    case SDL_QUIT:
      close();
    //Look for a keypress
    case SDL_KEYDOWN:
      //Check the SDLKey vals
      switch(event.key.keysym.sym) {
        //Checks if the player has pressed the left key
        //in this case rotate the ship counter clockwise 
        case SDLK_LEFT:
          player_.updateAngle(-1);
          break;
        case SDLK_RIGHT:
          //Checks if the player has pressed the right key
          //in this case rotate the ship clockwise
          player_.updateAngle(1);
          break;
        //Checks if the player has pressed the up key if so 
        //moves the ship in the direction that its front is
        //facing
        case SDLK_UP:
          player_.updatePosition(10);
          break;
        //Checks if the player has pressed the down key if so
        //moves the ship in the opposite direction that its
        //front is facing.
        case SDLK_DOWN:
          player_.updatePosition(-10);
          break;
        //Checks if the player has pressed the space bar
        //if so fires a bullet. 
        case SDLK_SPACE:
          fireBullet();
        default: 
          break;
      }
    default: 
      break;
    }
  }
}

void Game::clearBackground() {
  if (renderer_) {
    //If the renderer cannnot set the background color
    //close the game and throw an error saying why 
    if (SDL_SetRenderDrawColor(renderer_, 0x00, 0x00, 0x00, 0x00) != 0) {
      close(); 
      throw domain_error(string("Unable to set the background color due to: ") + SDL_GetError());
    }
    //If the renderer cannot be cleared then close the game
    //and throw an error saying why
    if (SDL_RenderClear(renderer_) != 0) {
      close();
      throw domain_error(string("Unable to clear the renderer due to: ") + SDL_GetError());
    }
  }
}

#pragma once

#include "EventHandler.h"
#include "GameData.h"

#include <SFML/Graphics/RenderWindow.hpp>

class GameOfLife {
 public:
  GameOfLife(GameData& gameData);

  ~GameOfLife();

  void run();

  void draw(GameData& data);

  void addEventHandler(EventHandler* eventHandler);

  void stop();

 private:
  GameData& gameData;

  sf::RenderWindow* window = nullptr;

  EventHandler* eventHandler = nullptr;

  bool shouldStop = false;
};

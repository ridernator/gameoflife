#pragma once

#include <string>

#include "EventHandler.h"
#include "GameData.h"

#include <SFML/Graphics/RenderWindow.hpp>

class GameOfLife {
 public:
  GameOfLife(const std::string& title,
             GameData& gameData);

  ~GameOfLife();

  void run();

  void draw(const GameData& data);

  void addEventHandler(EventHandler* eventHandler);

  void stop();

 private:
  const std::string title;

  GameData& gameData;

  sf::RenderWindow* window = nullptr;

  EventHandler* eventHandler = nullptr;

  bool shouldStop = false;
};

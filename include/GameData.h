#pragma once

#include <SFML/Graphics/RenderWindow.hpp>

class GameData {
 public:
  virtual void draw(sf::RenderWindow& window) = 0;
};

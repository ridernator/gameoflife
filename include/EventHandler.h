#pragma once

#include <SFML/Window/Event.hpp>

class EventHandler {
 public:
  virtual void newEvent(const sf::Event& event) = 0;
};

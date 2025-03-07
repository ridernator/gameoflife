#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Window/Event.hpp>
#include <cstdlib>
#include <iostream>

#include "EventHandler.h"
#include "GameData.h"
#include "GameOfLife.h"

class EV : public EventHandler {
 public:
  EV(GameOfLife& gameOfLife) : gameOfLife(gameOfLife) {}

  void newEvent(const sf::Event& event) override {
    if (event.is<sf::Event::KeyReleased>()) {
      const sf::Event::KeyReleased* krEvent = event.getIf<sf::Event::KeyReleased>();

      if (krEvent->code == sf::Keyboard::Key::Escape) {
        gameOfLife.stop();
      }
    }
  }

 private:
  GameOfLife& gameOfLife;
};

class GD : public GameData {
 public:
  GD() {
    circle.setRadius(50);
    circle.setPosition({100, 100});
  }

  void draw(sf::RenderWindow& window) const override {
    window.draw(circle);
  }

 private:
  sf::CircleShape circle;
};

int main(const int    argc,
         const char** argv) {
  if (argc != 2) {
    std::cerr << "Usage: sfmlStub windowName" << std::endl;

    std::exit(EXIT_FAILURE);
  }

  GD gd;

  GameOfLife sfmlStub(*(argv + 1), gd);

  EV ev(sfmlStub);
  sfmlStub.addEventHandler(&ev);

  sfmlStub.run();
}

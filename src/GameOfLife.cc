#include "GameOfLife.h"

#include <optional>
#include <string>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/VideoMode.hpp>

GameOfLife::GameOfLife(const std::string& title,
                       GameData&          gameData) : title(title),
                                                      gameData(gameData) {
}

GameOfLife::~GameOfLife() {
  if (window != nullptr) {
    stop();
  }
}

void GameOfLife::run() {
  window = new sf::RenderWindow(sf::VideoMode::getFullscreenModes()[0], title, sf::State::Fullscreen);

  // sf::View view = window->getDefaultView();
  // view.setRotation(180);
  // window->setView(view);

  std::optional<sf::Event> event;

  while ((window->isOpen()) && (!shouldStop)) {
    while ((event = window->pollEvent()) != std::nullopt) {
      if (eventHandler != nullptr) {
        eventHandler->newEvent(event.value());
      }
    }

    window->clear();
    gameData.draw(*window);
    window->display();
  }

  delete window;

  shouldStop = false;
}

void GameOfLife::stop() {
  shouldStop = true;
}

void GameOfLife::draw(const GameData& data) {
  data.draw(*window);
}

void GameOfLife::addEventHandler(EventHandler* eventHandler) {
  this->eventHandler = eventHandler;
}

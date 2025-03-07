#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Window/Event.hpp>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <mutex>
#include <sys/types.h>
#include <thread>
#include <set>

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
  GD(const std::uint32_t x,
     const std::uint32_t y,
     const std::string& births,
     const std::string& survives) : x(x),
                                    y(y),
                                    vertexArray(sf::VertexArray(sf::PrimitiveType::Triangles, 6 * x * y)) {
    data = new bool*[x];
    next = new bool*[x];

    for (std::uint32_t index = 0; index < x; ++index) {
      data[index] = new bool[y];
      next[index] = new bool[y];
    }

    for (std::uint32_t col = 0; col < x; ++col) {
      for (std::uint32_t row = 0; row < y; ++row) {
        data[col][row] = (rand() > (RAND_MAX / 2));
      }
    }

    for (const auto& b : births) {
      this->births.insert(b - 48);
    }

    for (const auto& s : survives) {
      this->survives.insert(s - 48);
    }
  }

  ~GD() {
    for (std::uint32_t index = 0; index < x; ++index) {
      delete [] data[index];
      delete [] next[index];
    }
  }

  void draw(sf::RenderWindow& window) override {
    lock.lock();

    auto windowSize = window.getSize();
    float xOffset;
    float yOffset;
    float width = windowSize.x / (float) x;
    float height = windowSize.y / (float) y;
    std::size_t index = 0;

    for (std::uint32_t col = 0; col < x; ++col) {
      for (std::uint32_t row = 0; row < y; ++row) {
        xOffset = width * col;
        yOffset = height * row;

        vertexArray[index * 6 + 0].position = sf::Vector2f(xOffset, yOffset);
        vertexArray[index * 6 + 1].position = sf::Vector2f(xOffset + width, yOffset);
        vertexArray[index * 6 + 2].position = sf::Vector2f(xOffset, yOffset + height);
        vertexArray[index * 6 + 3].position = sf::Vector2f(xOffset, yOffset + height);
        vertexArray[index * 6 + 4].position = sf::Vector2f(xOffset + width, yOffset + height);
        vertexArray[index * 6 + 5].position = sf::Vector2f(xOffset + width, yOffset);

        if (data[col][row]) {
          vertexArray[index * 6 + 0].color = sf::Color::Green;
          vertexArray[index * 6 + 1].color = sf::Color::Green;
          vertexArray[index * 6 + 2].color = sf::Color::Green;
          vertexArray[index * 6 + 3].color = sf::Color::Green;
          vertexArray[index * 6 + 4].color = sf::Color::Green;
          vertexArray[index * 6 + 5].color = sf::Color::Green;
        } else {
          vertexArray[index * 6 + 0].color = sf::Color::Black;
          vertexArray[index * 6 + 1].color = sf::Color::Black;
          vertexArray[index * 6 + 2].color = sf::Color::Black;
          vertexArray[index * 6 + 3].color = sf::Color::Black;
          vertexArray[index * 6 + 4].color = sf::Color::Black;
          vertexArray[index * 6 + 5].color = sf::Color::Black;
        }

        ++index;
      }
    }

    window.draw(vertexArray);

    lock.unlock();
  }

  std::uint32_t getNumNeighbours(const std::uint32_t col,
                                 const std::uint32_t row) {
    std::uint32_t returnVal = 0;

    for (std::int32_t testCol = (std::int32_t) col - 1; testCol <= (std::int32_t) col + 1; ++testCol) {
      if ((testCol >= 0) && (testCol < (std::int32_t) x)) {
        for (std::int32_t testRow = (std::int32_t) row - 1; testRow <= (std::int32_t) row + 1; ++testRow) {
          if ((testRow >= 0) && (testRow < (std::int32_t) y)) {
            if (!((testRow == (std::int32_t) row) && (testCol == (std::int32_t) col))) {
              if (data[testCol][testRow]) {
                ++returnVal;
              }
            }
          }
        }
      }
    }

    return returnVal;
  }

  void update() {
    std::uint32_t numNeighbours;

    for (std::uint32_t col = 0; col < x; ++col) {
      for (std::uint32_t row = 0; row < y; ++row) {
        numNeighbours = getNumNeighbours(col, row);

        if (data[col][row]) {
          if (survives.find(numNeighbours) != survives.end()) {
            next[col][row] = true;
          } else {
            next[col][row] = false;
          }
        } else {
          if (births.find(numNeighbours) != births.end()) {
            next[col][row] = true;
          } else {
            next[col][row] = false;
          }
        }
      }
    }

    lock.lock();

    bool** temp = data;
    data = next;
    next = temp;

    lock.unlock();
  }

 private:
  const std::uint32_t x;

  const std::uint32_t y;

  bool** data;
  bool** next;

  std::mutex lock;

  sf::VertexArray vertexArray;

  std::set<std::uint32_t> births;
  std::set<std::uint32_t> survives;
};

int main(const int    argc,
         const char** argv) {
  if (argc != 5) {
    std::cerr << "Usage: gameOfLife x y births survives (use B=3, S=23 for Life)" << std::endl;

    std::exit(EXIT_FAILURE);
  }

  GD gd(std::strtod(*(argv + 1), nullptr), std::strtod(*(argv + 2), nullptr), *(argv + 3), *(argv + 4));

  GameOfLife gameOfLife(gd);

  EV ev(gameOfLife);
  gameOfLife.addEventHandler(&ev);

  bool shouldStop = false;
  std::thread thread([&gd, &shouldStop] {
    while (!shouldStop) {
      gd.update();
    }
  });

  gameOfLife.run();

  shouldStop = true;

  thread.join();
}

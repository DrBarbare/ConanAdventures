#include <cstdint>
#include <SFML/Graphics.hpp>
#include <entt/entt.hpp>

struct position {
  float x;
  float y;
};

struct velocity {
  float dx;
  float dy;
};

void draw(sf::RenderWindow& window, entt::registry<> &registry) {
  auto transform = registry.view<position, sf::RectangleShape>();

  for (auto entity : transform) {
    auto pos   = transform.get<position>(entity);
    auto shape = transform.get<sf::RectangleShape>(entity);
	shape.setPosition(pos.x, pos.y);
    window.draw(shape);
  }
}

void update(float dt, entt::registry<> &registry) {
  registry.view<position, velocity>().each([dt](const auto, auto &pos, const auto &vel) {
    // gets all the components of the view at once ...

    pos.x += vel.dx * dt;
    pos.y += vel.dy * dt;

    // ...
  });
}

int main() {
  entt::registry registry;

  for (auto i = 0; i < 10; ++i) {
    auto entity = registry.create();
    registry.assign<position>(entity, i * 1.f, i * 1.f);
    registry.assign<sf::RectangleShape>(entity, sf::Vector2f{10.0f, 10.0f});

    if (i % 2 == 0) {
      registry.assign<velocity>(entity, i * 10.0f, i * 10.0f);
    }
  }

  sf::RenderWindow sfmlWin(sf::VideoMode(600, 360), "Conan adventure begins!", sf::Style::Close);
  sf::Clock clock;

  while (sfmlWin.isOpen()) {

    sf::Event e;
    while (sfmlWin.pollEvent(e)) {

      switch (e.type) {
      case sf::Event::EventType::Closed:
        sfmlWin.close();
        break;
      }
    }

    update(clock.getElapsedTime().asSeconds(), registry);

    sfmlWin.clear(sf::Color(174, 198, 207));
    draw(sfmlWin, registry);
    sfmlWin.display();
	clock.restart();
  }
  return 0;
}

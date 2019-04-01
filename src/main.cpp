#include <cstdint>
#include <random>

#include <SFML/Graphics.hpp>
#include <entt/entt.hpp>

struct position
{
	int x;
	int y;
};

struct velocity
{
	float dx;
	float dy;
};

void
draw(sf::RenderWindow& window, entt::registry<>& registry)
{
	auto transform = registry.view<position, sf::RectangleShape>();

	for (auto entity : transform)
	{
		auto pos = transform.get<position>(entity);
		auto shape = transform.get<sf::RectangleShape>(entity);
		shape.setPosition(pos.x, pos.y);
		window.draw(shape);
	}
}

void
update(float dt, entt::registry<>& registry)
{
	registry.view<position, velocity>().each([dt](const auto, auto& pos, const auto& vel) {
		// gets all the components of the view at once ...

		pos.x += vel.dx * dt;
		pos.y += vel.dy * dt;

		// ...
	});
}

static auto rng_engine = std::default_random_engine();
static std::uniform_int_distribution x_dist(0, 600);
static std::uniform_int_distribution y_dist(0, 360);


static std::uniform_real_distribution dx_dist(-100.0f, 100.0f);
static std::uniform_real_distribution dy_dist(-100.0f, 100.0f);

position
randomPosition()
{
	return {x_dist(rng_engine), y_dist(rng_engine)};
}

velocity
randomVelocity()
{
	return {dx_dist(rng_engine), dy_dist(rng_engine)};
}

static sf::Vector2f shapesSize{10.0f, 10.0f};

void
createEntity(entt::registry<>& registry)
{
	auto entity = registry.create();

	registry.assign<position>(entity, randomPosition());
	registry.assign<sf::RectangleShape>(entity, shapesSize);

	// Assign initial velocity
	registry.assign<velocity>(entity, randomVelocity());
}

int
main()
{
	entt::registry registry;

	for (auto i = 0; i < 10; ++i)
	{
		createEntity(registry);
	}

	sf::RenderWindow sfmlWin(sf::VideoMode(600, 360), "Conan adventure begins!", sf::Style::Close);
	sf::Clock clock;

	while (sfmlWin.isOpen())
	{

		sf::Event e;
		while (sfmlWin.pollEvent(e))
		{

			switch (e.type)
			{
				case sf::Event::EventType::Closed:
					sfmlWin.close();
					break;
				case sf::Event::EventType::KeyPressed:
				{
					break;
				}
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

#include <cstdint>
#include <iostream>
#include <random>

#include <SFML/Graphics.hpp>
#include <entt/entt.hpp>
#include <jpadkins/GlyphTileMap.h>

using jpadkins::GlyphTileMap;

static sf::Vector2f shapesSize{10.0f, 10.0f};

static auto rng_engine = std::default_random_engine();
static std::uniform_int_distribution<> x_dist(0, 20);
static std::uniform_int_distribution<> y_dist(0, 20);

static std::uniform_real_distribution<> dx_dist(-5.0f, 5.0f);
static std::uniform_real_distribution<> dy_dist(-5.0f, 5.0f);

struct position
{
	int x;
	int y;
};

position
randomPosition()
{
	return {x_dist(rng_engine), y_dist(rng_engine)};
}

void
placeEntities(GlyphTileMap& tileMap, entt::registry<>& registry)
{
	GlyphTileMap::Tile goblin(L'g', GlyphTileMap::Tile::Center, sf::Color::White, sf::Color::Black);
	registry.view<position>().each([&tileMap, goblin=std::move(goblin)](const auto&, auto& pos) {
		tileMap.setTile({pos.x, pos.y}, goblin);
	});
}

void
update(sf::Vector2u charPos, entt::registry<>& registry)
{
	registry.view<position>().each([&charPos, &registry](const auto entity, auto& pos) {
		if (charPos.x == pos.x && charPos.y == pos.y)
		{
			registry.destroy(entity);
		}
	});
}

void
createEntity(entt::registry<>& registry)
{
	auto entity = registry.create();

	registry.assign<position>(entity, randomPosition());
}

void
clearMap(GlyphTileMap& tileMap)
{

	const static GlyphTileMap::Tile base(
	    L'.', GlyphTileMap::Tile::Center, sf::Color(200, 200, 200, 100), sf::Color::Black);
	for (sf::Uint32 i = 0; i < tileMap.getArea().x; ++i)
	{
		for (sf::Uint32 j = 0; j < tileMap.getArea().y; ++j)
		{
			tileMap.setTile({i, j}, base);
		}
	}
}

int
main()
{
	const int frameWidth = 640;
	const int frameHeight = 480;
	const sf::VideoMode windowMode(frameWidth, frameHeight, 32);

	entt::registry registry;

	for (auto i = 0; i < 16; ++i)
	{
		createEntity(registry);
	}

	sf::Clock clock;

	sf::RenderWindow window(windowMode, "Conan adventure begins!", sf::Style::Close);
	window.setFramerateLimit(60);
	window.setVerticalSyncEnabled(true);

	sf::RenderTexture frame;
	if (!frame.create(frameWidth, frameHeight))
	{
		exit(EXIT_FAILURE);
	}
	window.setActive();

	sf::Font font;
	if (!font.loadFromFile("/usr/share/fonts/TTF/FreeMono.ttf"))
	{
		std::cerr << "Could lont open font\n";
		exit(-1);
	}

	GlyphTileMap tileMap(font, {20, 20}, {16, 16}, 16);
	GlyphTileMap::Tile tile(L'@', GlyphTileMap::Tile::Center, sf::Color::Red, sf::Color::Black);

	sf::Vector2u charPos{10, 10};

	while (window.isOpen())
	{

		sf::Event e;
		while (window.pollEvent(e))
		{

			switch (e.type)
			{
				case sf::Event::EventType::Closed:
					window.close();
					break;
				case sf::Event::EventType::KeyPressed:
				{
					if (e.key.code == sf::Keyboard::Key::H)
					{
						charPos.x = std::max<int>(0, charPos.x - 1);
					}
					else if (e.key.code == sf::Keyboard::Key::L)
					{
						charPos.x = std::min<int>(19, charPos.x + 1);
					}
					else if (e.key.code == sf::Keyboard::Key::K)
					{
						charPos.y = std::max<int>(0, charPos.y - 1);
					}
					else if (e.key.code == sf::Keyboard::Key::J)
					{
						charPos.y = std::min<int>(19, charPos.y + 1);
					}
					break;
				}
			}
		}

		update(charPos, registry);

		clearMap(tileMap);
		placeEntities(tileMap, registry);
		tileMap.setTile(charPos, tile);
		// tileMap.setScale(0.5f, 0.5f);
		tileMap.setPosition(16, 16);

		frame.clear(sf::Color(174, 198, 207));
		frame.draw(tileMap);
		frame.display();
		sf::Sprite frameSprite(frame.getTexture());
		// frameSprite.setScale(frameScale);
		window.draw(frameSprite);
		window.display();

		clock.restart();
	}
	return 0;
}

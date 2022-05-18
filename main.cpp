#include <SFML/Graphics.hpp>
#include <vector>
# define M_PI           3.14159265358979323846

bool ccw(sf::Vertex A, sf::Vertex B, sf::Vertex C)
{
	return (C.position.y - A.position.y) * (B.position.x - A.position.x) > (B.position.y - A.position.y) * (C.position.x - A.position.x);
}

bool intersect(sf::Vertex A, sf::Vertex B, sf::Vertex C, sf::Vertex D)
{
	return ccw(A, C, D) != ccw(B, C, D) and ccw(A, B, C) != ccw(A, B, D);
}

std::tuple<float, float> intersection(sf::Vertex A, sf::Vertex B, sf::Vertex C, sf::Vertex D)
{
	float x = (C.position.y - (D.position.y - C.position.y) / (D.position.x - C.position.x + 0.0001) * C.position.x - A.position.y + (B.position.y - A.position.y) / (B.position.x - A.position.x) * A.position.x) / ((B.position.y - A.position.y) / (B.position.x - A.position.x) - (D.position.y - C.position.y) / (D.position.x - C.position.x + 0.0001));
	float y = (B.position.y - A.position.y) / (B.position.x - A.position.x) * x + A.position.y - (B.position.y - A.position.y) / (B.position.x - A.position.x) * A.position.x;

	return std::tuple<float, float> {x,y};
}

int main()
{
	sf::RenderWindow* window = new sf::RenderWindow(sf::VideoMode(), "SFML", sf::Style::Fullscreen);
	sf::Event e;

	int nb_ray(60);

	std::vector<sf::RectangleShape> obstacles;

	sf::RectangleShape r1(sf::Vector2f(100, 100));
	r1.setFillColor(sf::Color::Black);
	r1.setOutlineColor(sf::Color::White);
	r1.setOutlineThickness(-5);
	r1.setPosition(500, 500);
	obstacles.push_back(r1);

	sf::RectangleShape r2(sf::Vector2f(500, 100));
	r2.setFillColor(sf::Color::Black);
	r2.setOutlineColor(sf::Color::White);
	r2.setOutlineThickness(-5);
	r2.setPosition(1000, 900);
	obstacles.push_back(r2);

	sf::RectangleShape r3(sf::Vector2f(500, 100));
	r3.setFillColor(sf::Color::Black);
	r3.setOutlineColor(sf::Color::White);
	r3.setOutlineThickness(-5);
	r3.setPosition(700, 50);
	obstacles.push_back(r3);

	sf::RectangleShape r4(sf::Vector2f(300, 400));
	r4.setFillColor(sf::Color::Black);
	r4.setOutlineColor(sf::Color::White);
	r4.setOutlineThickness(-5);
	r4.setPosition(1200, 200);
	obstacles.push_back(r4);

	while (window->isOpen())
	{
		while (window->pollEvent(e))
		{
			if (e.type == sf::Event::Closed)
			{
				window->close();
			}
		}

		window->clear();

		for (sf::RectangleShape o : obstacles)
		{
			window->draw(o);
		}

		for (int i(0); i < nb_ray; i++)
		{
			sf::Vertex line[] =
			{
				sf::Vertex(sf::Vector2f(sf::Mouse::getPosition().x, sf::Mouse::getPosition().y)),
				sf::Vertex(sf::Vector2f(2203 * cos((360 / nb_ray * i + 0.001) * M_PI / 180) + sf::Mouse::getPosition().x, 2203 * sin((360 / nb_ray * i + 0.001) * M_PI / 180) + sf::Mouse::getPosition().y))
			};

			float min(2203);

			for (sf::RectangleShape r : obstacles)
			{
				sf::Vertex cote1[] =
				{
					sf::Vertex(sf::Vector2f(r.getPosition().x, r.getPosition().y)),
					sf::Vertex(sf::Vector2f(r.getPosition().x, r.getPosition().y + r.getSize().y))
				};
				if (intersect(line[0], line[1], cote1[0], cote1[1]))
				{
					float a = std::get<0>(intersection(line[0], line[1], cote1[0], cote1[1])) - sf::Mouse::getPosition().x;
					float b = std::get<1>(intersection(line[0], line[1], cote1[0], cote1[1])) - sf::Mouse::getPosition().y;
					float d = sqrt(pow(a, 2) + pow(b, 2));
					if (d < min)
					{
						min = d;
						line[1] = sf::Vector2f(std::get<0>(intersection(line[0], line[1], cote1[0], cote1[1])), std::get<1>(intersection(line[0], line[1], cote1[0], cote1[1])));
					}
				}
				sf::Vertex cote2[] =
				{
					sf::Vertex(sf::Vector2f(r.getPosition().x, r.getPosition().y)),
					sf::Vertex(sf::Vector2f(r.getPosition().x + r.getSize().x, r.getPosition().y))
				};
				if (intersect(line[0], line[1], cote2[0], cote2[1]))
				{
					float a = std::get<0>(intersection(line[0], line[1], cote2[0], cote2[1])) - sf::Mouse::getPosition().x;
					float b = std::get<1>(intersection(line[0], line[1], cote2[0], cote2[1])) - sf::Mouse::getPosition().y;
					float d = sqrt(pow(a, 2) + pow(b, 2));
					if (d < min)
					{
						min = d;
						line[1] = sf::Vector2f(std::get<0>(intersection(line[0], line[1], cote2[0], cote2[1])), std::get<1>(intersection(line[0], line[1], cote2[0], cote2[1])));
					}
				}
				sf::Vertex cote3[] =
				{
					sf::Vertex(sf::Vector2f(r.getPosition().x + r.getSize().x, r.getPosition().y + r.getSize().y)),
					sf::Vertex(sf::Vector2f(r.getPosition().x, r.getPosition().y + r.getSize().y))
				};
				if (intersect(line[0], line[1], cote3[0], cote3[1]))
				{
					float a = std::get<0>(intersection(line[0], line[1], cote3[0], cote3[1])) - sf::Mouse::getPosition().x;
					float b = std::get<1>(intersection(line[0], line[1], cote3[0], cote3[1])) - sf::Mouse::getPosition().y;
					float d = sqrt(pow(a, 2) + pow(b, 2));
					if (d < min)
					{
						min = d;
						line[1] = sf::Vector2f(std::get<0>(intersection(line[0], line[1], cote3[0], cote3[1])), std::get<1>(intersection(line[0], line[1], cote3[0], cote3[1])));
					}
				}
				sf::Vertex cote4[] =
				{
					sf::Vertex(sf::Vector2f(r.getPosition().x + r.getSize().x, r.getPosition().y + r.getSize().y)),
					sf::Vertex(sf::Vector2f(r.getPosition().x + r.getSize().x, r.getPosition().y))
				};
				if (intersect(line[0], line[1], cote4[0], cote4[1]))
				{
					float a = std::get<0>(intersection(line[0], line[1], cote4[0], cote4[1])) - sf::Mouse::getPosition().x;
					float b = std::get<1>(intersection(line[0], line[1], cote4[0], cote4[1])) - sf::Mouse::getPosition().y;
					float d = sqrt(pow(a, 2) + pow(b, 2));
					if (d < min)
					{
						min = d;
						line[1] = sf::Vector2f(std::get<0>(intersection(line[0], line[1], cote4[0], cote4[1])), std::get<1>(intersection(line[0], line[1], cote4[0], cote4[1])));
					}
				}

			}

			window->draw(line, 2, sf::Lines);

		}

		window->display();

	}

	return 0;
}
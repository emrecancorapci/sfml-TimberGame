
#include <iostream>
#include <SFML/Graphics.hpp>
// The .hpp file extension means it's a header file.

using namespace std;
using namespace sf;

class GameObject
{
	private :
		Texture* texture;
		Sprite sprite;
		bool active;

	public :
		float speed;

	GameObject(string tf, float x, float y, bool a = false, float s = 0.0f)
		:active(a), speed(s)
	{
		texture->loadFromFile(tf);
		sprite.setTexture(*texture);
		sprite.setPosition(x, y);
	}

	GameObject(Texture* t, float x, float y, bool a = false, float s = 0.0f)
		:active(a), speed(s), texture(t)
	{
		sprite.setTexture(*t);
		sprite.setPosition(x, y);
	}
	void setActive(bool active);
	bool isActive();
	Sprite getSprite();
};

void GameObject::setActive(bool a)
{
	active = a;
}
bool GameObject::isActive()
{
	return active;
}
Sprite GameObject::getSprite()
{
	return sprite;
}

int main()
{
	// Creates a video mode object
	VideoMode vm(1920, 1080);

	// Create and open a window for the game
	RenderWindow window(vm, "Timberman", Style::Fullscreen);

	// Game Objects
	GameObject background("graphics/background.png", 0,0);
	GameObject tree("graphics/tree.png", 810, 0);
	GameObject bee("graphics/bee.png", 0, 800);
	bee.speed = 0.0f;
	bee.setActive(false);

	Texture* textureCloud = new Texture();
	textureCloud->loadFromFile("graphics/cloud.png");
	GameObject cloud1(textureCloud, 0, 0);
	GameObject cloud2(textureCloud, 0, 250);
	GameObject cloud3(textureCloud, 0, 500);

	// Clock
	Clock clock;

	while (window.isOpen())
	{

		#pragma region Input

		if (Keyboard::isKeyPressed(Keyboard::Escape))
			window.close();

		#pragma endregion


		#pragma region Update

		// Measure time
		Time dt = clock.restart();

		// Setup the bee
		if (!bee.isActive())
		{
			srand((int)time(0)); // Seed the random number generator
			bee.speed = (rand() % 200) + 200; // Get a random number between 200-399

			srand((int)time(0) * 10); // Seed the random number generator again
			float height = (rand() % 500) + 500; // Get a random number between 500-999
			bee.getSprite().setPosition(2000, height); // Set pos to (2000,height)
			bee.setActive(true);
		}
		// Move the bee
		else
		{
			bee.getSprite().setPosition
			(
				bee.getSprite().getPosition().x - (bee.speed * dt.asSeconds()),
				bee.getSprite().getPosition().y
			);

			// Has the bee reached the left-hand edge of the screen
			if (bee.getSprite().getPosition().x < -100)
			{
				bee.setActive(false);
			}
		}

		// Manage the clouds
		// Cloud 1
		if (!cloud1.isActive())
		{
			// How fast is the cloud
			srand((int)time(0) * 10);
			cloud1.speed = rand() % 200;

			// How high is the cloud
			srand((int)time(0) * 10);
			float height = rand() % 150;
			cloud1.getSprite().setPosition(-200, height);
			cloud1.setActive(true);
		}

		#pragma endregion


		#pragma region Draw

		window.clear(); // Clear last frame

		window.draw(background.getSprite());
		window.draw(tree.getSprite());
		window.draw(bee.getSprite());
		window.draw(cloud1.getSprite());
		window.draw(cloud2.getSprite());
		window.draw(cloud3.getSprite());

		window.display(); // Display current frame

		#pragma endregion
	}

	return 0;
}
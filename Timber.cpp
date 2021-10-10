
#include <iostream>
#include <sstream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
// The .hpp file extension means it's a header file.

using namespace std;
using namespace sf;

// Creates a video mode object
VideoMode vm(1920, 1080);

// Create and open a window for the game
RenderWindow window(vm, "Timberman", Style::Fullscreen);

#pragma region Branch

// Function declaration
void updateBranches(int seed);
const int NUM_BRANCHES = 6;
Sprite branches[NUM_BRANCHES];

// Where is the player/branch?
// Left or Right
enum class Side { LEFT, RIGHT, NONE };
Side branchPositions[NUM_BRANCHES];

// Function definition
void updateBranches(int seed)
{
	// Move all the branches down one place
	for (int j = NUM_BRANCHES - 1; j > 0; j--)
	{
		branchPositions[j] = branchPositions[j - 1];
	}

	// Spawn a new branch at position 0
	// LEFT, RIGHT or NONE
	srand((int)time(0) + seed);
	int r = (rand() % 5);
	switch (r) {
	case 0:
		branchPositions[0] = Side::LEFT;
		break;
	case 1:
		branchPositions[0] = Side::RIGHT;
		break;
	default:
		branchPositions[0] = Side::NONE;
		break;
	}
}
#pragma endregion


#pragma region GameObject

class GameObject
{
protected:
	Texture* texture = new Texture();
	Sprite sprite;
	bool active;
public:
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
	void setPosition(float x, float y);
	Vector2f getPosition();
};

class Player : public GameObject
{
public:
	Side side;

	Player(string tf, float x, float y, bool a = false)
		:GameObject(tf, x, y, a)
	{
		texture->loadFromFile(tf);
		sprite.setTexture(*texture);
		sprite.setPosition(x, y);
	}
};

class Axe : public GameObject
{
public:
	const float leftPosition;
	const float rightPosition;
	Axe(string tf, float x, float y, float lp, float rp, bool a = false)
		:GameObject(tf, x, y, a), leftPosition(lp), rightPosition(rp)
	{
		texture->loadFromFile(tf);
		sprite.setTexture(*texture);
		sprite.setPosition(x, y);
	}
};

class Log : public GameObject
{
public:
	float speed[2] =  {0,0};
	Log(string tf, float x, float y, float sx, float sy, bool a = false)
		:GameObject(tf, x, y, a)
	{
		speed[0] = sx;
		speed[1] = sy;
		texture->loadFromFile(tf);
		sprite.setTexture(*texture);
		sprite.setPosition(x, y);
	}
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
void GameObject::setPosition(float x, float y)
{
	sprite.setPosition(x, y);
}
Vector2f GameObject::getPosition() {
	float x = sprite.getPosition().x;
	float y = sprite.getPosition().y;
	return sprite.getPosition();
}

#pragma endregion


/*Text* TextBlock(string t, Font f, int s, float x = 0, float y = 0, Color c = Color::White,
	bool centered = false)
{
	Text text;
	text.setCharacterSize(s);
	text.setFillColor(c);
	text.setFont(f);
	text.setString(t);

	if (centered)
	{
		FloatRect textRect = text.getLocalBounds();
		text.setOrigin(textRect.left + textRect.width / 2.0f,
					   textRect.top + textRect.height / 2.0f);
	}

	text.setPosition(x, y);

	return &text;
}*/

int main()
{
	#pragma region Start

	#pragma region GameObjects

	GameObject background("graphics/background.png", 0, 0);
	GameObject tree("graphics/tree.png", 810, 0);
	GameObject bee("graphics/bee.png", 0, 800);
	bee.speed = 0.0f;
	bee.setActive(false);

	Texture* textureCloud = new Texture();
	textureCloud->loadFromFile("graphics/cloud.png");
	GameObject cloud1(textureCloud, 0, 0);
	GameObject cloud2(textureCloud, 0, 250);
	GameObject cloud3(textureCloud, 0, 500);

#pragma endregion

	// Clock
	Clock clock;

	#pragma region Time Bar

	RectangleShape timeBar;
	float timeBarStartWidth = 400;
	float timeBarHeight = 80;
	timeBar.setSize(Vector2f(timeBarStartWidth, timeBarHeight));
	timeBar.setFillColor(Color::Red);
	timeBar.setPosition((vm.width / 2.0f) - timeBarStartWidth / 2, vm.height - 100);

	Time gameTimeTotal;
	float timeRemaining = 6.0f;
	float timeBarWidthPerSecond = timeBarStartWidth / timeRemaining;

	// Pause
	bool paused = true;

#pragma endregion

	#pragma region Text

	// Font
	Font font;
	font.loadFromFile("fonts/KOMIKAP_.ttf");

	// Message Text
	Text messageText;
	messageText.setCharacterSize(75);
	messageText.setFillColor(Color::White);
	messageText.setFont(font);
	messageText.setString("Press Enter to start!");

	FloatRect textRect = messageText.getLocalBounds();
	messageText.setOrigin(textRect.left + textRect.width / 2.0f,
		textRect.top + textRect.height / 2.0f);

	messageText.setPosition(vm.width / 2, vm.height / 2);

	// messageText = TextBlock("Press Enter to start!", font, 75, vm.width / 2, vm.height / 2, Color::White, true);

	// Score Text
	int score = 0;
	Text scoreText;
	scoreText.setCharacterSize(100);
	scoreText.setFillColor(Color::White);
	scoreText.setFont(font);
	scoreText.setString("Score = 0");
	scoreText.setPosition(20, 20);

	//scoreText = TextBlock("Score = 0", font, 100, 20, 20, Color::White, true);

#pragma endregion

	#pragma region Branches

		// Prepare 6 branches
	Texture textureBranch;
	textureBranch.loadFromFile("graphics/branch.png");
	// Set the texture for each branch sprite
	for (int i = 0; i < NUM_BRANCHES; i++)
	{
		branches[i].setTexture(textureBranch);
		branches[i].setPosition(-2000, -2000);

		// Set the sprite's origin to dead centre
		// We can then spin it round without changing its position
		branches[i].setOrigin(220, 20);
	}

#pragma endregion

	// Prepare the player
	Player player("graphics/player.png", 580, 720);
	player.side = Side::LEFT;
	GameObject gravestone("graphics/rip.png", 800, 600);
	Axe axe("graphics/axe.png", 700, 830, 700, 1075);
	Log log("graphics/log.png", 810, 720, 1000, -1500);

	// Line the axe up with the tree
	#pragma endregion

	while (window.isOpen())
	{

		#pragma region Input

		if (Keyboard::isKeyPressed(Keyboard::Escape)) window.close();
		if (Keyboard::isKeyPressed(Keyboard::Return) && paused)
		{
			paused = false;
			score = 0;
			timeRemaining = 6;
		}

		#pragma endregion

		#pragma region Update

		if (!paused)
		{
			// Measure time
			Time dt = clock.restart();

			// Substract from the amount of time remaining
			timeRemaining -= dt.asSeconds();

			// Size up the time bar
			timeBar.setSize(Vector2f(timeBarWidthPerSecond * timeRemaining, timeBarHeight));

			if (timeRemaining <= 0.0f)
			{
				messageText.setString("Out of time!");

				// Reposition the text based on its new size
				FloatRect textRect = messageText.getLocalBounds();
				messageText.setOrigin(textRect.left + textRect.width / 2.0f,
					textRect.top + textRect.height / 2.0f);

				paused = true;
			}

			#pragma region Bee
			// Setup the bee
			if (!bee.isActive())
			{
				srand((int)time(0)); // Seed the random number generator
				bee.speed = (rand() % 200) + 200; // Get a random number between 200-399

				srand((int)time(0) * 10); // Seed the random number generator again
				float height = (rand() % 500) + 500; // Get a random number between 500-999
				bee.setPosition(2000, height); // Set pos to (2000,height)
				bee.setActive(true);
			}
			// Move the bee
			else
			{
				bee.setPosition
				(
					bee.getPosition().x - (bee.speed * dt.asSeconds()),
					bee.getPosition().y
				);

				// Has the bee reached the left-hand edge of the screen
				if (bee.getPosition().x < -100)
				{
					bee.setActive(false);
				}
			}

#pragma endregion

			#pragma region Clouds

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
				cloud1.setPosition(-200, height);
				cloud1.setActive(true);
			}
			else
			{
				cloud1.setPosition
				(
					cloud1.getPosition().x + (cloud1.speed * dt.asSeconds()),
					cloud1.getPosition().y
				);

				// Has the cloud reached the right edge?
				if (cloud1.getPosition().x > 1920)
					cloud1.setActive(false);
			}

			// Cloud 2
			if (!cloud2.isActive())
			{
				// How fast is the cloud
				srand((int)time(0) * 20);
				cloud2.speed = rand() % 200;

				// How high is the cloud
				srand((int)time(0) * 20);
				float height = (rand() % 300) - 150;
				cloud2.setPosition(-200, height);
				cloud2.setActive(true);
			}
			else
			{
				cloud2.setPosition
				(
					cloud2.getPosition().x + (cloud2.speed * dt.asSeconds()),
					cloud2.getPosition().y
				);

				// Has the cloud reached the right edge?
				if (cloud2.getPosition().x > 1920)
					cloud2.setActive(false);
			}

			// Cloud 3
			if (!cloud3.isActive())
			{
				// How fast is the cloud
				srand((int)time(0) * 30);
				cloud3.speed = rand() % 200;

				// How high is the cloud
				srand((int)time(0) * 30);
				float height = (rand() % 450) - 150;
				cloud3.setPosition(-200, height);
				cloud3.setActive(true);
			}
			else
			{
				cloud3.setPosition
				(
					cloud3.getPosition().x + (cloud3.speed * dt.asSeconds()),
					cloud3.getPosition().y
				);

				// Has the cloud reached the right edge?
				if (cloud3.getPosition().x > 1920)
					cloud3.setActive(false);
			}

#pragma endregion

			stringstream ss;
			ss << "Score = " << score;
			scoreText.setString(ss.str());

			#pragma region Branch

			// update the branch sprites
			for (int i = 0; i < NUM_BRANCHES; i++)
			{
				float height = i * 150;

				if (branchPositions[i] == Side::LEFT)
				{
					// Move the sprite to the left side
					branches[i].setPosition(610, height);
					// Flip the sprite round the other way
					branches[i].setRotation(180);
				}
				else if (branchPositions[i] == Side::RIGHT)
				{
					// Move the sprite to the right side
					branches[i].setPosition(1330, height);
					// Set the sprite rotation to normal
					branches[i].setRotation(0);
				}
				else
				{
					// Hide the branch
					branches[i].setPosition(3000, height);
				}
			}

#pragma endregion

		}

		#pragma endregion

		#pragma region Draw

		window.clear(); // Clear last frame

		#pragma region Sprites

		window.draw(background.getSprite());
		window.draw(cloud1.getSprite());
		window.draw(cloud2.getSprite());
		window.draw(cloud3.getSprite());
		window.draw(tree.getSprite());

		#pragma endregion

		// Draw the branches
		for (int i = 0; i < NUM_BRANCHES; i++) {
			window.draw(branches[i]);
		}

		window.draw(bee.getSprite());

		window.draw(timeBar);
		window.draw(scoreText);
		if (paused) window.draw(messageText);

		window.display(); // Display current frame

		#pragma endregion

	}

	return 0;
}
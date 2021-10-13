#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class GameObject
{
protected:
	Texture* texture = new Texture();
	Sprite sprite;
	bool active;
public:
	float speed;

	GameObject(std::string tf, float x, float y, bool a = false, float s = 0.0f);
	GameObject(Texture* t, float x, float y, bool a = false, float s = 0.0f);

	void setActive(bool active);
	bool isActive() const;
	Sprite getSprite() const;
	void setPosition(float x, float y);
	Vector2f getPosition() const;
};
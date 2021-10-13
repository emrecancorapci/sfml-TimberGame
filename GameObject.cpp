#include "GameObject.h"


using namespace sf;

GameObject::GameObject(std::string tf, float x, float y, bool a, float s)
:active(a), speed(s)
{
	texture->loadFromFile(tf);
	sprite.setTexture(*texture);
	sprite.setPosition(x, y);
}

GameObject::GameObject(Texture* t, float x, float y, bool a, float s)
:texture(t), active(a), speed(s)
{
	sprite.setTexture(*t);
	sprite.setPosition(x, y);
}


void GameObject::setActive(bool a)
{
	active = a;
}
bool GameObject::isActive() const
{
	return active;
}
Sprite GameObject::getSprite() const
{
	return sprite;
}
void GameObject::setPosition(float x, float y)
{
	sprite.setPosition(x, y);
}
Vector2f GameObject::getPosition() const
{
	float x = sprite.getPosition().x;
	float y = sprite.getPosition().y;
	return sprite.getPosition();
}

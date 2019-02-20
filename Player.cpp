#include "Player.h"

Player::Player(SDL_Renderer* r, int x, int y)
{
	Object ob;
	texture = ob.SpriteLoader(r, "bluebox.png");
	p_rect = {x, y, OB_WIDTH, OB_HEIGHT};
}

void Player::jumping() {
	this->p_rect.y -= 50; //jump
}

Player::~Player()
{
}
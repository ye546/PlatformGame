#include "Player.h"

Player::Player(SDL_Renderer* r, int x, int y)
{
	Object ob;
	PlayerTexture = ob.SpriteLoader(r, PlayerSprite);
	PlayerRect = {x, y, OB_WIDTH, OB_HEIGHT};
}

//bool Player::Collision(Player* p, SDL_Rect* rect) {
//	Vector2d PlayerVector;
//	Vector2d ObjectVector;
//	
//	PlayerVector.SetX(p->ObjectRect.x);
//	PlayerVector.SetY(p->ObjectRect.y);
//
//	ObjectVector.SetX(rect->x);
//	ObjectVector.SetY(rect->y);
//
//	if(Dot(PlayerVector, ObjectVector) > 0)
//}

bool Player::Grab() {
	if (MouseInsideBoundsCheck(this->PlayerRect) && SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT))
	{
		return true;
	}
	else return false;
}

void Player::PlayerRender(SDL_Renderer* r) {
	SDL_RenderCopy(r, this->PlayerTexture, NULL, &this->PlayerRect );
	//also handle other things such as physics and grabbing here
	bool grab = this->Grab();
	if (grab)
	{
		int x(0), y(0);
		//Update objects position to the mouse pos
		SDL_GetMouseState(&x, &y);
		this->PlayerRect.x = (x - this->PlayerRect.w / 2);
		this->PlayerRect.y = (y - this->PlayerRect.h / 2);
		printf("Player POS: X:%d Y:%d\n", this->PlayerRect.x, this->PlayerRect.y);
	}
}

void Player::jumping() {
	this->PlayerRect.y -= 50; //jump
}

Player::~Player()
{
}

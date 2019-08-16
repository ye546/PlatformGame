#include "Object.h"

Object::Object(SDL_Renderer* r) {
	//Apply texture for the object
	this->ObjectTexture = SpriteLoader(r, ObjectSprite);
	int x, y;
	//Set its position to the mouse
	SDL_GetMouseState(&x, &y);
	//set midpoint for circle to be mousepos
	x -= OB_WIDTH / 2;
	y -= OB_HEIGHT / 2;
	this->ObjectRect = { x, y, OB_WIDTH, OB_HEIGHT }; //and this will define the position for the object
}

Object::Object(){
	//do nothing
}

Object::~Object() {
	SDL_DestroyTexture(ObjectTexture);
}

void Object::Render(SDL_Renderer* r, SDL_Event& ev, bool flags) {
	SDL_RenderCopy(r, ObjectTexture, NULL, &this->ObjectRect);

	bool grab = this->Grab();
	if (grab)
	{
		int x(0), y(0);
		//Update objects position to the mouse pos
		SDL_GetMouseState(&x, &y);
		this->ObjectRect.x = (x - this->ObjectRect.w / 2);
		this->ObjectRect.y = (y - this->ObjectRect.h / 2);
		printf("Object(%i) POS: X:%d Y:%d\n", this->ObjectID, this->ObjectRect.x, this->ObjectRect.y);
	}

	Resize(ev, flags);
}

void Object::Resize(SDL_Event& ev, bool flags) {
	if (MouseInsideBoundsCheck(this->ObjectRect) && SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT))
	{
		if (flags)
		{
			if (ev.type == SDL_MOUSEWHEEL)
			{
				if (ev.wheel.y > 0)
					this->ObjectRect.w += 5;
				if (ev.wheel.y < 0)
					this->ObjectRect.w -= 5;
				if (ev.wheel.y = 0)
					this->ObjectRect.w = this->ObjectRect.w;
			}
		}

		else if (!flags)
		{
			if (ev.type == SDL_MOUSEWHEEL)
			{
				if (ev.wheel.y > 0)
					this->ObjectRect.h += 10;
				if (ev.wheel.y < 0)
					this->ObjectRect.h -= 10;
				if (ev.wheel.y = 0)
					this->ObjectRect.h = this->ObjectRect.h;
			}
		}
	}

	if (this->ObjectRect.w < 5)
		this->ObjectRect.w = 5;

	if (this->ObjectRect.h < 5)
		this->ObjectRect.h = 5;
}

bool Object::Grab() {
	if (MouseInsideBoundsCheck(this->ObjectRect) && SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT))
	{
		return true;
	}
	else return false;
}

void Object::Collision(Object* ob) {
	if (this->ObjectRect.x + this->ObjectRect.w >= ob->ObjectRect.x)
	{
		this->ObjectID < ob->ObjectID;
		this->ObjectRect.x--;
	}
	if (this->ObjectRect.x >= ob->ObjectRect.x + ob->ObjectRect.w)
	{
		this->ObjectID < ob->ObjectID;
		this->ObjectRect.x++;
	}
	if (this->ObjectRect.y + this->ObjectRect.h >= ob->ObjectRect.y)
	{
		this->ObjectID < ob->ObjectID;
		this->ObjectRect.y--;
	}
	if (this->ObjectRect.y >= ob->ObjectRect.y + ob->ObjectRect.h)
	{
		this->ObjectID < ob->ObjectID;
			this->ObjectRect.y++;
	}
}

void Object::KillAllMonsters() {
	if (!m_Objects.empty()) {
		for (auto &i : m_Objects) {
			SDL_DestroyTexture(i->ObjectTexture);
			i = nullptr;
		}
		printf("Destroyed %i blocks\n", m_Objects.size());
		m_Objects.clear();
	}
	else
		printf("There are no blocks to destroy!\n");
}

void Object::CreateNewObject(SDL_Renderer* r) {
	Object* ob = new Object(r);

	//assign an ID to the object
	for (int i = 0; i < m_Objects.size(); i++) {
		if (m_Objects.empty())
			ob->ObjectID += 1;
		else if (m_Objects.size() > 0) {
			ob->ObjectID = m_Objects[i]->ObjectID += 1;
		}
	}
	//ob->ObjectID = rand() % 100 + 1;
	m_Objects.push_back(ob);
	printf("Added a new Monster to m_Objects!\n");
}

bool Object::Check(Object *o, Object *l) {
	if (SDL_HasIntersection(&o->ObjectRect, &l->ObjectRect))
		return true;

	return false;
}

bool Object::MouseInsideBoundsCheck(SDL_Rect &box) {
	int x(0), y(0);

	SDL_GetMouseState(&x, &y);

	if ((x > box.x) && (x < box.x + box.w) && (y > box.y) && (y < box.y + box.h)) {
		//printf("I am inside the bounds!\n");
		return true;
	}
	return false;
}
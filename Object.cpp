#include "Object.h"

Object::Object(SDL_Renderer* r, const char* path) {
	this->_objectTexture = SpriteLoader(r, path);
}

Object::Object(){}

//this spawns objects mid on mousepointer
SDL_Rect Object::rect(SDL_Rect rect) {
	int x, y;
	SDL_GetMouseState(&x, &y);
	//set midpoint for circle to be mousepos
	x -= OB_WIDTH / 2;
	y -= OB_HEIGHT / 2;
	return(rect = {x, y, OB_WIDTH, OB_HEIGHT});
}

void Object::Grab(SDL_Rect *r) {
	int x(0), y(0);
	SDL_GetMouseState(&x, &y);	//insert mouse coordinates to x and y
	if (mouse_inside_bounds_check(*r)) {
		if (SDL_GetMouseState(0, 0) & SDL_BUTTON(SDL_BUTTON_RIGHT)) {
			//set X and Y to be the midpoint of the grabbed object
			r->x = (x - r->w / 2);	
			r->y = (y - r->h / 2);
		}
	}
}

void Object::Render(SDL_Renderer* rend, SDL_Texture* t, SDL_Rect* crop, SDL_Rect* rect) {
	SDL_RenderCopy(rend, t, crop, rect);
}

Object::~Object() {
	SDL_DestroyTexture(_objectTexture);
}

bool Object::mouse_inside_bounds_check(SDL_Rect &box) {
	int x(0), y(0);
	SDL_GetMouseState(&x, &y);
	if ((x > box.x) && (x < box.x + box.w) && (y > box.y) && (y < box.y + box.h)) {
		//printf("I am inside the bounds!\n");
		return true;
	}
	return false;
}

void Object::resize(SDL_Rect& rect, SDL_Event& ev, bool one){
	if (mouse_inside_bounds_check(rect) && SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT))
	{
		if (one)
		{
			if (ev.type == SDL_MOUSEWHEEL)
			{
				if (ev.wheel.y > 0)
					rect.w += 5;
				if (ev.wheel.y < 0)
					rect.w -= 5;
				if (ev.wheel.y = 0)
					rect.w = rect.w;
			}
		}

		else if (!one)
		{
			if (ev.type == SDL_MOUSEWHEEL)
			{
				if (ev.wheel.y > 0)
					rect.h += 10;
				if (ev.wheel.y < 0)
					rect.h -= 10;
				if (ev.wheel.y = 0)
					rect.h = rect.h;
			}
		}
	}
	if (rect.w < 5)
		rect.w = 5;
	if (rect.h < 5)
		rect.h = 5;
}

bool Object::check(Object *o, Object *l) {
	if (SDL_HasIntersection(&o->_objectRect, &l->_objectRect))
		return true;
	return false;
}

void Object :: occupied_spot(Object *o, Object *l) {
	if (check(o, l)) {
		if (o->_objectRect.x >= (l->_objectRect.x + l->_objectRect.w)) {
			o->_objectRect.x = l->_objectRect.x + l->_objectRect.w;
		}

		if (o->_objectRect.x + o->_objectRect.w <= l->_objectRect.x) {
			o->_objectRect.x = l->_objectRect.x - l->_objectRect.w;
		}

		if (o->_objectRect.y >= (l->_objectRect.y + l->_objectRect.h)) {
			o->_objectRect.y = l->_objectRect.y + l->_objectRect.h;
		}

		if (o->_objectRect.y + o->_objectRect.h <= l->_objectRect.y) {
			o->_objectRect.y = l->_objectRect.y - l->_objectRect.h;
		}
	}
}
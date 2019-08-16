#pragma once
#include <SDL.h>

class Vector2d
{
public:
	Vector2d();

	int Dot(Vector2d one, Vector2d two);
	int Magnitude(const Vector2d v);
	int GetX() { return m_x; };
	int GetY() { return m_y; };
	int SetX(int x) { m_x = x; }
	int SetY(int y) { m_y = y; }


	~Vector2d();
	
private:
	int m_x;
	int m_y;
};


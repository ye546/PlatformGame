#include "Vector2d.h"

Vector2d::Vector2d()
{
}

int Vector2d::Dot(Vector2d one, Vector2d two) {
	return (one.m_x * two.m_x + one.m_y * two.m_y);
}

int Vector2d::Magnitude(const Vector2d v) {
	return sqrt(Dot(v, v));
}

Vector2d::~Vector2d()
{
}

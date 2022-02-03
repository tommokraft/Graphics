#pragma once
#include "Vector4.h"
#include "Vector3.h"
class Light
{
public :
	Light() {};
	Light(const Vector3& position, const Vector4& color, float radius, bool direction) {
		this->position = position;
		this->radius = radius;
		this->colour = color;
		this->directional = direction;
	}
	Light(const Light&) = default;
	Light(Light&&) = default;
	Light& operator=(const Light&) = default;
	Light& operator=(Light&&) = default;
	~Light(void) {};
	bool getDirectional() const { return directional; }
	void setDirectional(bool direction) { directional = direction; }
	Vector3 getPosition() const { return position; }
	void SetPosition(const Vector3& val) { position = val; }
	float getRadius() const { return radius; }
	void SetRadius( float val) { radius = val; }
	Vector4 getColour() const { return colour; }
	void SetColour(const Vector4& val) { colour = val; }
protected:
	Vector3 position;
	float radius;
	Vector4 colour;
	bool directional;
};


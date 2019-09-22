#ifndef OBSTACLE_H
#define OBSTACLE_H
//------------------------------------------------------------------------
//
//  Name:   Obstacle.h
//
//  Desc:   Simple obstacle class
//
//  Author: Mat Buckland 2002 (fup@ai-junkie.com)
//
//------------------------------------------------------------------------
#include "../Common/2d/Vector2D.h"
#include "../Common/Game/BaseGameEntity.h"
#include "../Common/misc/Cgdi.h"

#include <windows.h>

class Obstacle : public BaseGameEntity
{
public:
	Obstacle(double x, double y, double r) :BaseGameEntity(0, Vector2D(x, y), r) {}

	Obstacle(Vector2D pos, double radius) :BaseGameEntity(0, pos, radius) {}

	Obstacle(std::ifstream& in) { Read(in); }

	virtual ~Obstacle() {}

	void      Update(double time_elapsed) {}

	void      Render() { gdi->BlackPen(); gdi->Circle(Pos(), BRadius()); }

	void      Write(std::ostream& os) const;
	void      Read(std::ifstream& in);
};

#endif
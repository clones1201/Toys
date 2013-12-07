#ifndef __STRAWWALL_H__
#define __STRAWWALL_H__

#include "Point2D.h"
#include "Line.h"

class Wall : public Line{

public:
	Wall(const Point2D &p1,const Point2D &p2);
	~Wall(){}
	bool isHit(const Line &l);
};

#endif

#ifndef __LINE_H__
#define __LINE_H__

#include "Point2D.h"

class Line{
public:
	Point2D s,d;

	double a;
	double b;
	double c;

	Line(const Line& l){ s = l.s; d = l.d; a = l.a; b = l.b; c = l.c; };
	Line(const Point2D &_s,const Point2D &_d);
	~Line(){}
	bool isHit(const Line &l);
};

#endif
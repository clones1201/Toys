#ifndef __POINT2D_H__
#define __POINT2D_H__

class Point2D{
public:
	int x,y;

	Point2D(){x = 0;y = 0;}
	Point2D(int _x,int _y){
		if( _x > 10000 ) 	_x = 10000;
		if( _x < -10000)	_x = -10000;
		if( _y > 10000 ) 	_y = 10000;
		if( _y < -10000)	_y = -10000;
		this->x = _x ; this->y = _y;}
	Point2D(const Point2D& p){
		this->x = p.x; this->y = p.y;
	}
	~Point2D(){}
};

#endif
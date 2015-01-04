
#include "global.h"
#include "StrawWall.h"

Wall::Wall(const Point2D &p1,const Point2D &p2):Line(p1,p2){
}

bool Wall::isHit( const Line& l){
	if( Line::isHit(l) ){

		double hit_x = (double)(c * l.b - l.c * b) / (double)( a * l.b - l.a * b );
		double hit_y = (double)(l.c * a - c * l.a) / (double)( a * l.b - l.a * b );

		if( b == 0 || l.b == 0 ){
			if( (( hit_y <= s.y && hit_y >= d.y ) || ( hit_y >= s.y && hit_y <= d.y))
				&& (( hit_y <= l.s.y && hit_y >= l.d.y ) || (hit_y >= l.s.y && hit_y <= l.d.y))
				)
				return true;
			else
				return false;
		}else{
			if( (( hit_x <= s.x && hit_x >= d.x ) || ( hit_x >= s.x && hit_x <= d.x))
				&& (( hit_x <= l.s.x && hit_x >= l.d.x ) || (hit_x >= l.s.x && hit_x <= l.d.x))
				)
				return true;
			else 
				return false;
		}
	}
	else
		return false;
}
#include "Line.h"
#include "global.h"
#include "commonException.h"

Line::Line(const Point2D &_s,const Point2D &_d){
	if( _s.x == _d.x && _s.y == _d.y )
		throw new NotLine;
	s = _s; d = _d;
	
	if(  _s.x == _d.x ){
		a = 1; 
		b = 0;
		c = _s.x;
	}else if( _s.y == _d.y ){
		a = 0;  //标准式系数
		b = 1;
		c = _s.y;
	}
	else{
		a = (double)1 / double(_d.x - _s.x);
		b = (double)1 / double(_s.y - _d.y);
		c = (double)( _s.x * a ) + (double)( _s.y * b );
	}
}

bool Line::isHit(const Line &l){

	if( a * l.b - l.a * b == 0 )	//   当且仅当行列式
		return false;				//  | a1  b1 |
	else							//	| a2  b2 |  = a1 * b2 - a2 * b1 不等于0时相交(重合不算，也就是侧对着墙是射不中的)
		return true;
}

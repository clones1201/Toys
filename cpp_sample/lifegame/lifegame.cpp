#include <GL\glew.h>
#include <GL\freeglut.h>

#include "global.h"

#include "lifegame.h"

LifeGame::LifeGame(int width,int height, int num){
//	createGameWindow(width,height);
	_width = width;
	_height = height;
	gen = 0; cur = 0;
	zoom = 1;
	buffer[0] = new Byte[width * height];
	buffer[1] = new Byte[width * height];
	pixelBuffer = new Byte[width * height * 3];

	for( int i = 0 ; i < width * height ; i++ ){
		buffer[0][i] = 0;
		buffer[1][i] = 0;
	}

	seed(num);
	cell = num;
}

LifeGame::~LifeGame(){
	delete [] buffer[0];
	delete [] buffer[1];
	delete [] pixelBuffer;
}

void LifeGame::seed(int num){
	int pos = 0;
	int step = _width * _height / num;
	if( step < 2 ) step = 2;
	while( pos < _width * _height ){
		buffer[cur][pos] = 1;
		pos = pos + step * rand()/double(RAND_MAX) + 1;
	}
}

int LifeGame::GetGeneration(){
	return gen;
}

void LifeGame::display_pixel(){
	for( int i = 0 ; i < _width * _height * 3 ; i++ ){
		pixelBuffer[  i  ] = 0;
	}

	for( int i = 0 ; i < _width * _height ; i++ ){
		if( 1 == buffer[cur][i] )
			pixelBuffer[ 3 * i + 1 ] = 255;
	}

	glDrawPixels(_width,_height,GL_RGB,GL_UNSIGNED_BYTE,pixelBuffer);
	glFlush();
}

void LifeGame::display_points(){
	glPointSize( ( zoom > 1 )?zoom:1 );
	glColor3f(0,1,0);
	int r,c,x,y;
	double px,py;
	glBegin(GL_POINTS);
	
	for( int i = 0 ; i < _width * _height ; i++ ){
		c = i / _width;
		r = i % _width;
		x = r - _width / 2;
		y = c - _height / 2;
		px = zoom * (x * 2) / _width;
		py = zoom * (y * 2) /_height;

		if( buffer[cur][i] == 1)
			glVertex2d( px , py );

	}

	glEnd();
	glFlush();

}

void LifeGame::loop(){
	int nx[8],ny[8];

	for( int i = 0 ; i < _width * _height ; i ++ ){
		buffer[(cur + 1) % 2][i] = 0;
	}
	for( int i = 0 ; i < _width * _height ; i ++ ){
		int x = i % _width;
		int y = i / _width;

		nx[0] = x - 1;	ny[0] = y - 1;
		nx[1] = x;		ny[1] = y - 1;
		nx[2] = x + 1;	ny[2] = y - 1;
		nx[3] = x - 1;	ny[3] = y;
		nx[4] = x + 1;	ny[4] = y;
		nx[5] = x - 1;	ny[5] = y + 1;
		nx[6] = x;		ny[6] = y + 1;
		nx[7] = x + 1;	ny[7] = y + 1;

		for(int j = 0 ; j < 8 ; j ++){
			if( nx[j] < 0 ){
				nx[j] = _width - 1;
			}else if( nx[j] >= _width ){
				nx[j] = 0;
			}
			if( ny[j] < 0 ){
				ny[j] = _height - 1;
			}else if( ny[j] >= _height ){
				ny[j] = 0;
			}
		}

		int countNeighbors = 0;
		for(int j = 0 ; j < 8 ; j ++ ){
			if(buffer[ cur ][ nx[j] + _width * ny[j] ] == 1){
				countNeighbors ++;
			}
		}

		int newBuf = (cur + 1) % 2;
		if(1 == buffer[cur][ i ]){
			if( countNeighbors == 3 || countNeighbors == 2){
				buffer[ newBuf ][ i ] = 1;
			}else{
				buffer[ newBuf ][ i ] = 0;
				cell --;
			}
		}else if( 0 == buffer[cur][ i ]){
			if( countNeighbors == 3 ){
				buffer[ newBuf ][ i ] = 1;
				cell ++;
			}else{
				buffer[ newBuf ][ i ] = 0;
			}
		}else{
			buffer[ newBuf][ i ] = 0;
		}

	}

	gen = gen + 1;
	cur =  (cur + 1) % 2;
}

int LifeGame::GetCell(){
	return cell;
}

void LifeGame::ZoomIn(){
	zoom = zoom * 2;
}

void LifeGame::ZoomOut(){
	zoom = zoom / 2;
}
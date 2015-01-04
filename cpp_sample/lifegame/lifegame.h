#if !defined __LIFEGAME_H__
#define __LIFEGAME_H__

typedef unsigned char Byte;

class LifeGame{
public:
	LifeGame(int width, int height,int num); 
	~LifeGame();
	
	void display_pixel();
	void display_points();
	
	void loop();

	int GetGeneration();
	int GetCell();

	void ZoomIn();
	void ZoomOut();
private:
	int _width,_height; //windows size, also the size of world
	int gen;
	int cur;
	int cell;
	float zoom;
	Byte *buffer[2];
	Byte *pixelBuffer;

	void createGameWindow(int width, int height);

	void seed(int num); 
	
};

#endif
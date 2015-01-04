#include <GL\glew.h>
#include <GL\freeglut.h>

#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <ctime>
#include <cmath>
using namespace std;

#include "lifegame.h"
#include "hashlife.h"

LifeGame *life;
HashLife *hashlife;

void display(){
	clock_t t1,t2;

	t1 = clock();

	glClear(GL_COLOR_BUFFER_BIT);
	life->display_points();
	life->loop();

	t2 = clock();	
	clock_t dt = t2 - t1;

	char fps[256];
    float ifps = 1.f / ( float( dt ) / 1000.f);
	sprintf(fps,"Game of Life,%d Cells, %d Generations, %0.2f fps",life->GetCell(),life->GetGeneration(),ifps);
	glutSetWindowTitle(fps);

	glutPostRedisplay();
}

void mouse(int button,int state,int x,int y){
	if( state == GLUT_DOWN ){
		switch( button ){
		case GLUT_LEFT_BUTTON:
			life->ZoomIn();
			break;
		case GLUT_RIGHT_BUTTON:
			life->ZoomOut();
			break;
		default:
			break;
		}
	}
}

int main( int argc, char* argv[]){
	glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_RGB);
    glutInitWindowPosition(0,0);
    glutInitWindowSize(1024,768);
    glutCreateWindow("Game of Life");
    glutDisplayFunc(&display);
	glutMouseFunc(&mouse);

	clock_t t1,t2;
	t1 = clock();
	life = new LifeGame(1024,768,500000);
	t2 = clock();
	printf("LifeGame initial time: %d ms", t2 - t1);
	hashlife = new HashLife(1024,768,500000);
	t1 = clock();
	printf("HashLife initial time: %d ms", t1 - t2);
    
	glutMainLoop();
	return 0;
}
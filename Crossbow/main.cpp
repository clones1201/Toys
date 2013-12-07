#include "global.h"
#include "StrawWall.h"
#include "commonException.h"

#define Vector2D Point2D

int kernel(vector<Wall> &wallList,vector<Point2D> &pointList,Point2D jw){
	int maxHit = 0;
	for_each(pointList.begin(),pointList.end(),
		[&](Point2D &p){
			Point2D Inf;
			Vector2D Direction(p.x - jw.x,p.y - jw.y);  //��������
			Inf.x = 20000 * Direction.x + jw.x;		//Point2D�������޷�����[-10000��10000]�����ǿ����޸ĵó���
			Inf.y = 20000 * Direction.y + jw.y;		//���Ǿͳ�������Զ����
			Line Course(jw,Inf);

			int hit = 0;
			for_each(wallList.begin(),wallList.end(),
				[&](Wall &w){
					if( w.isHit(Course) )
						hit++;
			});
			if( hit >= maxHit )
				maxHit = hit;
	});
	return maxHit;
}

int main(int argc,char* argv[]){

	istream *inputStream = NULL;

	if( argc == 2 ){      //�������ļ������ļ����룬û�оͼ�������
		inputStream = new fstream(argv[1],ios_base::in);
	}
	else if( argc > 2 )	{
		cerr<<"Too many arguments"<<endl;
		exit(0);
	}
	else if( argc == 1 ){
		inputStream = &cin;
	}
	else{
		cerr<<"Error input"<<endl;
	}

	try{

	int nLoop;
	*inputStream>>nLoop;

	for(int t = 0 ; t < nLoop ; ++t ){

		vector<Wall> wallList;
		vector<Point2D> pointList;

		int nWalls;
		*inputStream>>nWalls;
		Point2D *p1,*p2;
		int x,y;

		for(int w = 0 ; w < nWalls ; ++w ){
			*inputStream>>x>>y;
			p1 = new Point2D(x,y);
			*inputStream>>x>>y;
			p2 = new Point2D(x,y);
			pointList.push_back((Point2D&)*p1);
			pointList.push_back((Point2D&)*p2);
			wallList.push_back(* new Wall(*p1,*p2));
		}

		*inputStream>>x>>y;
		p1 = new Point2D(x,y);

		int maxHit = kernel(wallList,pointList,*p1);
		cout<<maxHit<<endl;
	}
	}catch(NotLine){
		cerr<<"EXCEPTION:NotLine"<<endl;
	}

	//cout<<"press any key to exit..."<<endl;
	//cin.get();
	return 0;
}
#include <GL\glew.h>
#include <GL\freeglut.h>

#include "global.h"

#include "hashlife.h"

enum ErrorCode{
	ButtomReached
};

class Error: public exception{
private:
	ErrorCode _code;
public:
	Error(ErrorCode code):_code(code){};
};

enum Direction{
	nw = 0, ne = 1, sw = 2, se = 3 
};
/*
static int Pattern[16][4] = {
	{0,0,0,0},
	{0,0,0,1},
	{0,0,1,0},
	{0,0,1,1},
	{0,1,0,0},
	{0,1,0,1},
	{0,1,1,0},
	{0,1,1,1},
	{1,0,0,0},
	{1,0,0,1},
	{1,0,1,0},
	{1,0,1,1},
	{1,1,0,0},
	{1,1,0,1},
	{1,1,1,0},
	{1,1,1,1}
};
*/
Node::Node(int level):_level(level){
	/*if(_level == 0 ){
		id.pattern = 0;
	}
	else{
		id.children[nw] = new Node( level - 1 );
		id.children[ne] = new Node( level - 1 );
		id.children[sw] = new Node( level - 1 );
		id.children[se] = new Node( level - 1 );
	}*/
}

Node::~Node(){
	/*if( _level != 0){
		delete id.children[nw];		
		delete id.children[ne];
		delete id.children[sw];
		delete id.children[se];
	}*/
}
const 
Node& Node::GetNorthWest() const{
	if(_level != 0){
		return *(id.children[nw]);
	}
	else{
		return *this;
	}
}

const
Node& Node::GetNorthEast() const{
	if(_level != 0){
		return *(id.children[ne]);
	}
	else{
		return *this;
	}
}

const
Node& Node::GetSouthWest() const{
	if(_level != 0){
		return *(id.children[sw]);
	}
	else{
		return *this;
	}
}

const
Node& Node::GetSouthEast() const{
	if(_level != 0){
		return *(id.children[se]);
	}
	else{
		return *this;
	}
}

bool Node::isButtom() const{
	if(_level == 0){
		return true;
	}else{
		return false;
	}
}

void Node::SetLevel(int level){
	_level = level;
}

void Node::SetNorthEast(Node& node){
	if( _level != 0 ){
		id.children[ne] = &node;
	}else{
		throw new Error(ButtomReached);
	}
}

void Node::SetNorthWest(Node& node){
	if( _level != 0 ){
		id.children[nw] = &node;
	}else{
		throw new Error(ButtomReached);
	}
}

void Node::SetSouthEast(Node& node){
	if( _level != 0 ){
		id.children[se] = &node;
	}else{
		throw new Error(ButtomReached);
	}
}

void Node::SetSouthWest(Node& node){
	if( _level != 0 ){
		id.children[sw] = &node;
	}else{
		throw new Error(ButtomReached);
	}
}

int Node::GetLevel() const{
	return _level;
}

void Node::SetDead(){
	id.pattern = 0;
}

void Node::SetAlive(){
	id.pattern = 1;
}

bool Node::isAlive() const{
	if( id.pattern == 0){
		return false;
	}else{
		return true;
	}
}
/*
Node& Node::operator=(const Node& param){

}
*/

bool operator==(const Node& param1, const Node& param2){
	bool result = false;
	if( param1._level != param2._level ){
		result = false;
	}
	else if( param1._level == 0 ){
		if( param1.id.pattern == param2.id.pattern ){
			result = true;
		}
	}
	else{
		result = ( param1.GetNorthEast() == param2.GetNorthEast() )
			&& ( param1.GetNorthWest() == param2.GetNorthWest() )
			&& ( param1.GetSouthEast() == param2.GetSouthEast() )
			&& ( param1.GetSouthWest() == param2.GetSouthWest() );
	}
	return result;
}

void Node::display(float x,float y,float zoom) const{
	if( _level == 0 ){
	}
}

int HashLifeHelp(float odd, Node* &node, vector< vector< Node* > > &table){
	int result = 0;
	if(node->GetLevel() == 0 ){
		float dice = rand()/(float)RAND_MAX;
		delete node;
		if( dice < odd ){
			node = table[0][1];
			result = 1;
		}else{
			node = table[0][0];
			result = 0;
		}
	}else{
		Node *nw,*ne,*sw,*se;
		nw = new Node( node->GetLevel() - 1);
		ne = new Node( node->GetLevel() - 1);
		sw = new Node( node->GetLevel() - 1);
		se = new Node( node->GetLevel() - 1);

		result += HashLifeHelp(odd, nw, table);
		result += HashLifeHelp(odd, ne, table);
		result += HashLifeHelp(odd, sw, table);
		result += HashLifeHelp(odd, se, table);

		node->SetNorthEast( *ne );
		node->SetNorthWest( *nw );
		node->SetSouthEast( *se );
		node->SetSouthWest( *sw );

		Node* temp = node;
		for(int i = 0; i < table[node->GetLevel()].size() ; i++ ){
			Node *p = table[node->GetLevel()][i];
			if( *node == *( p ) ){
				delete node;
				node = p;
			}
		}

		if( temp == node )/* node is a new one */{
			table[node->GetLevel()].push_back(node);
		}
	}
	return result;
}

HashLife::HashLife(int width, int height, int cells){
	vector< vector< Node* > > table;

	int totaln = cells * 4;
	while( totaln != 0 ){
		totaln = totaln >> 2;
		table.push_back( *(new vector< Node* >));
	}
	/* the cell at level 0 is pre-initialized before recursion*/
	Node* alive = new Node(0);
	Node* dead = new Node(0);
	alive->SetDead();
	dead->SetAlive();
	table[0].push_back(dead);
	table[0].push_back(alive);
	
	float odd = (float)cells / ( 1 << (2 * (table.size() - 1) ));

	root = new Node( table.size() - 1 );
	cell = HashLifeHelp(odd , root, table);

	/*
	free the memory  */
	/*for_each( table.begin(), table.end(), [&]( vector< Node* > &p){
		for_each( p.begin(), p.end(), []( Node* n ){
			delete n;
		});
		p.clear();
	});
	table.clear();
	*/
}

HashLife::~HashLife(){

}

void HashLife::display(){
	glPointSize( ( zoom > 1 )?zoom:1 );
	glColor3f(0,1,0);

	glBegin(GL_POINTS);
	/*
	root.display(
		cells_x - lookat_x,
		,zoom);
		*/
	glEnd();
	glFlush();
}

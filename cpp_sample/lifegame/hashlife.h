#if !defined __HASHLIFE_H__
#define __HASHLIFE_H__

class Node;
class HashLife;

class Node{
	/*
	class Node represents the data structure for quadtree.
	*/
private:
	union{
		int pattern;
		Node* children[4];
	}id;
	int _level;
public:
	Node(int level);
	~Node();
	const Node& GetNorthWest() const;
	const Node& GetNorthEast() const;
	const Node& GetSouthWest() const;
	const Node& GetSouthEast() const;
	
	void SetNorthWest(Node& node);
	void SetNorthEast(Node& node);
	void SetSouthWest(Node& node);
	void SetSouthEast(Node& node);

	int GetLevel() const;
	void SetLevel(int level);
	bool isButtom() const;

	void SetAlive();
	void SetDead();
	bool isAlive() const;

	void display(float x,float y,float zoom) const;

	friend bool operator==(const Node& param1,const Node& param2);

	//Node& operator=(const Node& param);
};


class HashLife{
private:
	Node *root;
	int cells_x,cells_y;   //where the topest Node located
	int lookat_x,lookat_y;  //where the watching window located
	int width,height;
	float zoom;
	int cell;
public:
	HashLife(int width = 1024, int height = 768, int cells = 500000);
	~HashLife();
	
	void display();
};




#endif
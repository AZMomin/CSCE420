#include <iostream>
#include <vector>

using namespace std;

class Node{
	public:
		int v;				// index in vertices
		Node * parent; 		// parent of current node
		int depth;			// depth
		Node(int i);
		Node(int i, Node * p);
		//vector<Node * > successors();
		vector<Node * > successors;
		vector<Node * > traceback();
};

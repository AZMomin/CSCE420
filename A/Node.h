#include <iostream>
#include <vector>
#include <stack>
using namespace std;

class Node{
	public:
		//int v;				// index in vertices
		vector<stack<int> > state;
		Node * parent; 		// parent of current node
		int depth;			// depth
		int heuristic; 		// A* heuristics
		Node(vector<stack<int> > s);
		Node(vector<stack<int> > s, Node * p);
		//vector<Node * > successors();
		vector<Node * > successors;
		vector<Node * > traceback();
		
};

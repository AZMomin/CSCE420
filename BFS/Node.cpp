#include "Node.h"

Node::Node(int i){
	v = i;
}

Node::Node(int i, Node * p){
	v = i;
	parent = p;
}

/*vector<Node * > Node::successors(){
	vector<Node * > children;
	for(int i = 0; i < num_of_edges; i++){
		if (edges[i].first == v){
			Node * temp = new Node(edges[i].second, this);
			temp -> depth = this -> depth +1;
			children.push_back(temp);
		}
		else if (edges[i].second == v){
			Node * temp = new Node(edges[i].first, this);
			temp -> depth = this -> depth +1;
			children.push_back(temp);
		}
	}
	return children;
}*/

vector<Node * > Node::traceback(){
	vector<Node * > path;
	path.push_back(this);
	Node * p = this -> parent;
	while (p != NULL){
		path.push_back(p);
		p = p -> parent;
	}
	return path;
}

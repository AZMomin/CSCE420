#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <math.h>   
#include "Node.h"
using namespace std;


int num_of_vertices, num_of_edges; 
vector<pair<int,int> > vertices;
vector<pair<int,int> > edges;
int input_vertex, goal_vertex;
int max_frontier_size = 0;
int vertices_visited = 1;
// ------------------------ COMPARATOR CLASS -------------------------
struct compareNodes {
    bool operator()(const Node &n1, const Node &n2) {
        return n1.heuristic < n2.heuristic;
    }
};
// -------------------------------------------------------------------
// 							LOCAL FUNCTIONS
// -------------------------------------------------------------------
double set_heuristic(Node * node){
	return sqrt(  (pow((double(vertices[node->v].first) - double(vertices[goal_vertex].first)),2)) +
				  (pow((double(vertices[node->v].second) - double(vertices[goal_vertex].second)),2))
				);
}

vector<Node * > successors(Node * parent){
	vector<Node * > children;
	for(int i = 0; i < num_of_edges; i++){
		if (edges[i].first == parent -> v){
			Node * temp = new Node(edges[i].second, parent);
			temp -> depth = parent -> depth +1;
			temp -> heuristic = set_heuristic(temp);
			children.push_back(temp);
		}
		else if (edges[i].second ==  parent -> v){
			Node * temp = new Node(edges[i].first, parent);
			temp -> depth = parent -> depth +1;
			temp -> heuristic = set_heuristic(temp);
			children.push_back(temp);
		}
	}
	return children;
}


// -------------------------------------------------------------------
// 					BREADTH-FIRST SEARCH FUNCTION 
// -------------------------------------------------------------------
Node * Search(Node * initial_state, int goal){
	queue <Node*>  frontier;
	vector<bool> visted;
	
	for(int i = 0; i < num_of_vertices; i++){
		visted.push_back(false);
	}
	cout << "pushing vertex " << initial_state -> v << endl;
	frontier.push(initial_state);

	while(!(frontier.empty())){
		if (frontier.size() > max_frontier_size)
			max_frontier_size = frontier.size();
		Node * temp = frontier.front();
		cout << "Checking Vertex:  " << temp -> v << endl;
		frontier.pop();
		if (temp -> v == goal && visted[temp -> v] == false)
			return temp; 
		else if (visted[temp -> v] == false){
			visted[temp -> v] = true;
			vertices_visited++;
			vector<Node * > children = successors(temp);
			cout << "Pushing child vertices: ";
			for (int i = 0; i < children.size(); i++){
				if (visted[children[i]->v] == false){
					cout << children[i]->v << "  ";
					frontier.push(children[i]);
				}
			}
			cout << endl << endl;

		}

	}
	return NULL;
}

int main(){
	// --------------------- LOCAL VARIABLES ----------------------------
	ifstream myfile;
	int index, element1, element2;			
	string line;
	string filename;

	cout << "Enter file name!" << endl;
	cin >> filename; 
	// -------------------------------------------------------------------
	// 		OPEN FILE AND LOAD VERTICES AND EDGES IN APPROPRIATE VECTOR
	// -------------------------------------------------------------------
  	myfile.open(filename.c_str());
	if (myfile.is_open()){
		cout << "File found" << endl;
		myfile >> line; 
		if(line == "vertices:"){					
			myfile >> num_of_vertices;
			bool temp = getline(myfile, line);		// getline to avoid blank line 
			for (int i = 0; i < num_of_vertices; i++){
				myfile >> index;					// index 
				myfile >> element1;					// X coordinate
				myfile >> element2; 				// Y coordinate
				vertices.push_back(make_pair(element1, element2)); // create pair & add to vertices 
			}
			myfile >> line;
			if(line == "edges:"){
				myfile >> num_of_edges;
				temp = getline(myfile, line);		// getline to avoid blank line 
				for (int i = 0; i < num_of_edges; i++){
					myfile >> index;				// index
					myfile >> element1;				// Vertex 1 
					myfile >> element2; 			// Vertex 2 
					edges.push_back(make_pair(element1, element2));  // create pair & add to edges 
				}
			} 
		}
	// -------------------------------------------------------------------
	// 				REQUEST STARTING AND GOAL VERTEX - CHECK VALIDITY 
	// -------------------------------------------------------------------
	cout << "Enter input vertex" << endl;
	cin >> input_vertex;
	while (input_vertex >= num_of_vertices || input_vertex < 0){
		cout << "Invalid Input Vertex. Try again." << endl;
		cin >> input_vertex;
	}
	cout << "Enter goal vertex" << endl;
	cin >> goal_vertex;
	while (goal_vertex >= num_of_vertices || goal_vertex < 0){
		cout << "Invalid Goal Vertex. Try again." << endl;
		cin >> goal_vertex;
	}
	// -------------------------------------------------------------------
	// 						CREATE FIRST NODE 
	// -------------------------------------------------------------------
  	Node * initial = new Node(input_vertex, NULL);
  	vector<Node * > children = successors(initial);
	initial -> successors = children;
	initial -> heuristic = set_heuristic(initial);
	for (int i = 0; i < initial -> successors.size(); i++){
		cout << initial -> successors[i] -> v << endl; 
	}
	//-----------------   START SEARCH ON FIRST NODE ---------------------
	Node * _goal = Search(initial, goal_vertex);

	cout << "\nGoal: " << _goal -> v << endl;
	vector<Node * > path = _goal -> traceback();
	// -------------------------------------------------------------------
	// 						PRINT OUT DATA
	// -------------------------------------------------------------------
	cout << "Vertices: " << num_of_vertices << "\tEdges: " << num_of_edges << endl;
	cout << "Start: (" << vertices[input_vertex].first << "," << vertices[input_vertex].second << ")\t";
	cout << "Goal: (" << vertices[goal_vertex].first << "," << vertices[goal_vertex].second << ")\t";
	cout << "Vertices: " << input_vertex << " and " << goal_vertex << endl;
	cout << "Verticies Visited: " << vertices_visited << endl;
	cout << "Path Length: " << path.size() << endl << endl;
	cout << "Max queue size: " << max_frontier_size << endl;
	cout << "Solution: " << endl;
	// ------------------------- TRACEBACK ------------------------------- 
	for(int i = path.size() -1 ; i >= 0; i--){
		cout << "Vertex" << " " << path[i] -> v << "\t" ;
		cout << "(" << vertices[path[i] -> v].first << "," << vertices[path[i] -> v].second << ")" << endl; 
	}

	}
	else
		cout << "File not found" << endl;
  	myfile.close();
  	
}

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stack>
#include <queue>
#include <math.h>   
#include "Node.h"
#include <stdlib.h>    
#include <time.h>       
using namespace std;


int max_frontier_size = 0;
int num_of_stacks = 0;
int num_of_elements = 0;
// ------------------------ COMPARATOR CLASS -------------------------
struct comparator{
	bool operator()(const Node * a, const Node* b) const{
		return a -> heuristic > b -> heuristic;
	}
};

// -------------------------------------------------------------------
// 							LOCAL FUNCTIONS
// -------------------------------------------------------------------
int set_heuristic(Node * node){
	return node -> depth;
}

void print_state(vector<stack<int> > v){
	for(int j = 0; j < num_of_stacks; j++){
				cout << j << "  |  ";
				while(!v[j].empty()){
					cout << v[j].top() << " ";
					v[j].pop();
				}
			cout << endl;
			}
}

string state_to_string(vector<stack<int> > v){
	string s = "";
	for(int i = 0; i < num_of_stacks; i++){
		string st = to_string(i);
		s += st + 'x';
		while(!v[i].empty()){
			string str = to_string(v[i].top());
			s+= str;
			v[i].pop();
		}
	}
	return s;
}

vector<Node * > successors(Node * parent){
	cout << "Finding successors" << endl;
	vector<Node * > children;
	for(int i = 0; i < num_of_stacks; i++){
		if (!parent -> state[i].empty()){
			int top = parent -> state[i].top();
			for (int j = 0; j < num_of_stacks; j++){
				Node * temp = new Node(parent->state, parent);
				if(i != j){
					temp -> state[i].pop();
					temp -> state[j].push(top);
					temp -> depth = parent -> depth +1;
					temp -> parent = parent;
					temp -> heuristic = set_heuristic(temp);
					children.push_back(temp);
				}
			}
		}
		//else
		//	cout << "Stack " << i << "is empty" << endl;
	}

	return children;
}

bool isGoal(Node * s){
	vector<stack<int> > temp = s -> state;

	for(int i = 0; i < num_of_stacks; i++){
		if (i != 0)
			if(!temp[i].empty()){
				return false;
			}
	}
	for (int i = 0; i < num_of_elements; i++){
		if (temp[0].top() == i)
			temp[0].pop();
		else {
			return false;
		}		
	}
	return true;
}

// -------------------------------------------------------------------
// 					GREEDY SEARCH FUNCTION 
// -------------------------------------------------------------------
Node * Search(Node * initial_state){
	priority_queue<Node*,vector<Node*>,comparator> frontier;
	vector<string> visited;
	frontier.push(initial_state);
	cout << "pushing initial state" << endl;

	while(!(frontier.empty())){
		if (frontier.size() > max_frontier_size)
			max_frontier_size = frontier.size();
		Node * temp = frontier.top();
		//visited.push_back(state_to_string(temp -> state));
		cout << "Checking State:  " << state_to_string(temp -> state)<< endl;
		print_state(temp -> state);
		frontier.pop();
		if (isGoal(temp)){
			cout << "Goal Found!" << endl;
			print_state(temp -> state);
			return temp; 
		}
		else{
			vector<Node * > children = successors(temp);
			cout << children.size() <<" possible child vertices: ";
			for (int i = 0; i < children.size(); i++){
					string s_state = state_to_string(children[i] -> state); 
					bool isVisited = false;
					for(int j = 0; j < visited.size(); j++){
						if(s_state == visited[j])
							isVisited = true;
					}
					if(!isVisited){
					//	cout << s_state << "  |  " ;
						frontier.push(children[i]);
						visited.push_back(s_state);
					}
				}
			cout << endl << endl;

		}

	}
	cout << "ran out of successors" << endl;
	return NULL;
}


int main(){

	// -------------------------------------------------------------------
	// 						REQUEST INITIAL DATA 
	// -------------------------------------------------------------------
	cout << "Total number of stacks: " << endl;
	cin >> num_of_stacks;
	cout << "Max Number of elements: " << endl;
	cin >> num_of_elements;
	// -------------------------------------------------------------------
	// 						CREATE FIRST NODE 
	// -------------------------------------------------------------------
	vector<stack<int> > initial_blocks (num_of_stacks);
	for(int i = 0; i < num_of_elements; i++){  //Initialize random blocks 
		int random = rand() % num_of_stacks;
		cout << "Pushing " << i << " onto stack " << random << endl;
		initial_blocks[random].push(i);
	}
  	Node * initial = new Node(initial_blocks, NULL);
  	vector<Node * > children = successors(initial);
	initial -> successors = children;
	cout << "Number of first node children: " << initial -> successors.size() << endl;
	initial -> heuristic = set_heuristic(initial);
	//-----------------   START SEARCH ON FIRST NODE ---------------------
	cout << "Starting Search" << endl;
	Node * _goal = Search(initial);

	//cout << "\nGoal: " << _goal -> v << endl;
	//vector<Node * > path = _goal -> traceback();
	// -------------------------------------------------------------------
	// 						PRINT OUT DATA
	// -------------------------------------------------------------------
//	cout << "Path Length: " << path.size() << endl << endl;
	cout << "Max queue size: " << max_frontier_size << endl;
	cout << "Solution: " << endl;
	// ------------------------- TRACEBACK ------------------------------- 
	/*for(int i = path.size() -1 ; i >= 0; i--){
		print_state(path[i]->state);
		cout << "\n--------------------------------------------------------------------\n";
		
	}*/
  	
}


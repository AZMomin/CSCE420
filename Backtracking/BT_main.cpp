#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <iterator>
#include <sstream>
#include "Node.h"

using namespace std;
// --------------------- GLOBAL VARIABLES ----------------------------
	vector<Node * >  states;
	vector<pair<string,string> > constraints;
	vector<pair<string,string> > initial_goal;		// (initial, goal)
	vector<pair<string,string> > edges;
	vector<pair<int,int> > coordinates; 
	int finish_time;
	int iteration = 0;
// --------------------- LOCAL FUNCTION ----------------------------
	
	vector<string> _split(string s){		// SPLITS STRINGS INTO WORDS (SEPERATED BY SPACES)
		stringstream ss(s);
		istream_iterator<string> _begin(ss);
		istream_iterator<string> _end;
		vector<string> _vstrings(_begin, _end);

		return _vstrings;
	}

	void create_variables(string s){			// INITIALIZES DATA STRUCTRED BASED ON INPUT OF VARIABLES
		vector <string> _states = _split(s);
		for(int i = 0; i < _states.size(); i++){
			Node * temp = new Node(_states[i]);
			char a = _states[i][4];
			int x = a - '0';
			temp -> time = x;
			states.push_back(temp);
		}
	}

	void add_domains(string s){			// ADDS DOMAIN FOR EACH STATE BASED ON INPUT OF VARIABLES AND DOMAINS
		vector<string> tokens = _split(s);
		for(int i = 0; i < states.size(); i++){
			if(states[i]->name == tokens[0]){
				for(int j = 1; j < tokens.size(); j++){
					states[i] -> domains.push_back(tokens[j]);
				}
			}
		}

	}
	void add_contraints(string s){		//ADDS CONSTRAINTS TO A VECTOR OF PAIRS, EACH PAIR MUST HAVE COLORS NOT EQUAL TO EACH OTHER
		vector<string> c = _split(s);
		constraints.push_back(make_pair(c[1],c[2]));
	}

	vector<Node *> names_to_nodes(vector<string> s){	// TURNS A LIST OF STATE NAMES INTO A LIST OF CORRESPONDING NODES
		vector<Node *> ss;
		for(int i = 0; i < s.size(); i++){
			for(int j = 0; j< states.size(); j++){
				if(states[j] -> name == s[i])
					ss.push_back(states[j]);
			}
		}
		return ss;
	}
	vector<string> unique(vector<string> v){
		vector<string> u;
		bool unique = true;
		for(int i = 0; i < v.size(); i++){
			if(u.empty())
				u.push_back(v[i]);
			else{
				for(int j = 0; j < u.size(); j++){
					if(v[i] == u[j]){
						unique = false;
						break;
					}
				}
				if(unique == true)
					u.push_back(v[i]);
			}
			unique = true;
		}
		return u;
	}
	vector<string> adjacent_nodes(vector<pair<string,string> > edges, vector<string> prev){
		cout << "calling adjacent_nodes" << endl;
		cout << "prev: " << prev.size() << " edges: "<< edges.size() <<  endl;
		vector<string> v = prev;
		for(int i = 0; i< prev.size(); i++){
			for(int j = 0; j < edges.size(); j++){
				if(prev[i] == edges[j].first){
					v.push_back(edges[j].second);
				}
				else if(prev[i] == edges[j].second){
					v.push_back(edges[j].first);
				}
			}
		}
		cout << "returning " << v.size() << "number of elements" << endl;
		return unique(v);
	}
	bool is_edge(string f, string s){
		if(f == s)
			return true;
		for(int i = 0; i < edges.size(); i++)
			if((edges[i].first == f || f == "none") && (edges[i].second == s || s == "none"))
				return true;
			else if((edges[i].first == s || s == "none") && (edges[i].second == f || f == "none"))
				return true;
		return false;
	}
	bool is_adjacenct(vector<Node * > _states, int index, string s){		//makes sure that the location we choose is adjacent from previous time location
		//get previous time
		string x,y;
		if(_states[index] -> time == 1)
			return true; 
		else if(_states[index] -> time == finish_time)
			return true;
		else{
			x = states[index-1] -> location;
			y = states[index+1] -> location;

			if(is_edge(x,s))
				if(is_edge(y,s))
					return true;
		}
		return false;
	}


	bool satisfied(vector<Node * > _states, int index, string c){			// CHECKS IF LOCATION IS APPROPRIATE AND DOES NOT BREAK ANY CONSTRAINTS 
		if(!is_adjacenct(_states, index, c))
			return false;

		string name = _states[index] -> name;
		vector<string> valid_constraints;
		for(int i = 0; i< constraints.size(); i++){
			if(constraints[i].first == name)
				valid_constraints.push_back(constraints[i].second);
			else if(constraints[i].second == name)
				valid_constraints.push_back(constraints[i].first);
		}
		vector<Node *> valid_nodes = names_to_nodes(valid_constraints);
		for(int i = 0; i < valid_nodes.size(); i++){
			if(valid_nodes[i] -> location == c)
				return false;
		}

		return true;

	}
	
	void print_states(vector<Node *> s){			// PRINTS OUT THE VARIABLES AND CORRESPONDING LOCATION (used for output testing)
		for(int i = 0; i < s.size(); i++)
			cout << s[i] -> name << " : " <<  " " <<  s[i] -> location << "  -  ";
		cout << endl << endl;
	}
	
	bool backtrack(vector<bool> assignment, vector <Node *> st){		// main backrack function
		bool finished = true;
		vector <Node *> s = st;
		vector <string> temp;
		int counter = 0;
		int min_value = 5000;
		vector<bool> new_assignment = assignment;
		int index;												//           ----- MRV HEURISTIC -----
		for(int i = 0; i< assignment.size(); i++){				// goes through assignment vector to see if all colors have been assigned 
			if(assignment[i] == false){							// if there is a state with no color 
				for(int j = 0; j< s[i]->domains.size(); j++){	
					if(satisfied(s, i, s[i]->domains[j])){		// check every domain to see how many work	 
						counter++;	
					}

				}
				if (counter < min_value){						// if this state has less posible solutions  
					min_value = counter;						
					index = i;									// this becomes the state we check next 
				}
				counter = 0;
				finished = false;
			}	
		}
		if(finished){											// if all states have been assigned, return true
			states = st;
			return true;
		}
		cout << "Picking Variable: " << s[index] -> name << endl;
		for(int i = 0; i< s[index]->domains.size(); i++){		// for each domain of the chosen state 
			iteration++;
			if(satisfied(s, index, s[index]->domains[i])){		// check if domain is satisfied 
				cout << "Adding: " << s[index]->domains[i] << " to " << s[index] -> name << endl;		//if so add the color 
				s[index] -> location = s[index]->domains[i];
				print_states(s);
				new_assignment[index] = true;
				cout << " -------BACKTRACKING-------" << endl;
				if(backtrack(new_assignment, s))				// recursively call backtracking 
					return true;
			}
			else
				cout << "Failed: " << s[index]->domains[i] << " to " << s[index]-> name << endl;
		}
		return false;
	}
	
	

int main(){

	// --------------------- LOCAL VARIABLES ----------------------------
	ifstream myfile, destFile, graphFile;
	ofstream constraintsFile;
	string line;
	string filename = "CSP.txt";
	int index1, index2;
	string vertex1, vertex2;
	string destination, graph;
	int num_of_variables = 0;
	vector<string> vertices;
	vector<string> temp_variables;
	vector<string> variables;
	int num_of_vertices;
	int num_of_edges;
	// -------------------------------------------------------------------
	// 						PRE-PRE-PROCESSING STAGE
	// -------------------------------------------------------------------
	cout << "Enter file names!" << endl;
	cin >> destination >> graph;
	destFile.open(destination.c_str());
	if(destFile.is_open()){
		while(!destFile.eof()){
			getline(destFile, line);
			vector<string> tokens = _split(line);
			temp_variables.push_back(tokens[0]);
			initial_goal.push_back(make_pair(tokens[1],tokens[2]));
		}

	}
	else{
		cout << "Destination File not found" << endl;
		return 0;
	}

	destFile.close();
	//--------------------------------------------------------------------
	graphFile.open(graph.c_str());
	if(graphFile.is_open()){
		graphFile >> num_of_vertices;
		graphFile >> num_of_edges;
		for(int i = 0; i < num_of_vertices; i++){
			graphFile >> line;
			graphFile >> index1;
			graphFile >> index2; 
			vertices.push_back(line);
			coordinates.push_back(make_pair(index1,index2));
		}

		for(int i = 0; i < num_of_edges; i++){
			graphFile >> vertex1;
			graphFile >> vertex2;
			edges.push_back(make_pair(vertex1,vertex2));
		}
	}
	else{
		cout << "Graph File not found" << endl;
		return 0;
	}
	destFile.close();

	// -------------------------------------------------------------------
	// 						GENERATE CONSTRAINTS FILE
	// -------------------------------------------------------------------
	cout << "Enter Completion Time" << endl;
	cin >> finish_time;
	string s = "";
	stringstream ss; 
	constraintsFile.open(filename.c_str());
	cout << "adding variables" << endl;
	constraintsFile << "#variables" << "\n";
	for(int i = 0; i < temp_variables.size(); i++){
		for(int j = 1; j <= finish_time; j++){
			ss << j;
			string t = temp_variables[i] + "-" + ss.str(); 
			s+= t + " ";
			ss.str("");
			variables.push_back(t);
		}
	}
	constraintsFile << s << "\n";
	cout << "adding domains" << endl;
	constraintsFile << "#domains" << "\n";
	vector<string> previous_vertices;			// previous vertices. 
	int j = 0;									// counter
	int first = 0;
	cout << "number of variables: " << variables.size() << endl;
	cout << "initial_goal size: " << initial_goal.size() << endl;
	cout << "finish time: " << finish_time << endl; 
	for(int i = 0; i < variables.size(); i++){
		if(j == 0){
			cout << variables[i] << " " << j << endl;
			constraintsFile << variables[i] + " " + initial_goal[first].first << "\n";
			previous_vertices.clear();
			previous_vertices.push_back(initial_goal[first].first);
			j++;
		}
		else if(j == finish_time-1){
			cout << variables[i] << " " << j << endl;
			constraintsFile << variables[i] + " " + initial_goal[first].second << "\n";
			//previous_vertices.push_back(initial_goal[i].second);
			previous_vertices.clear();
			j = 0;
			first++;
		}
		else{
			cout << variables[i] << " " << j << endl;
			vector<string> v = adjacent_nodes(edges, previous_vertices);
			string s;
			previous_vertices.clear();
			for(int z = 0; z < v.size(); z++){
				previous_vertices.push_back(v[z]);
				s+= v[z] + " ";
			}
			constraintsFile << variables[i] + " " + s << "\n";
			j++;
		}
	}
	constraintsFile << "#constraints" << "\n";
	int counter = 0;
	for(int i = 0; i < finish_time; i++){		// create constraints!!!
		for(int j = i; j < variables.size()-finish_time; j+=finish_time){
			for(int k = j+finish_time ; k < variables.size(); k+=finish_time){
				constraintsFile << "neq " << variables[j] << " " << variables[k] << "\n";
			}
		}
	}
	constraintsFile.close();
	// -------------------------------------------------------------------
	// 						PRE-PROCESSING STAGE
	// -------------------------------------------------------------------
	

	myfile.open(filename.c_str());
	if (myfile.is_open()){
		getline(myfile,line);	//#variables
		getline(myfile,line);	// get variables
		cout << line << endl;	
		create_variables(line);		// create vector of variables 

		getline(myfile,line);		//#domains
		cout << "Domains!" << endl;
		getline(myfile,line);
		while(!myfile.eof() && line != "#constraints"){
			//add line to constraint
			add_domains(line);		// add domains to each node
			cout << line << endl;
			getline(myfile,line);
		}


		//getline(myfile, line);
		cout << "Constraints!" << endl;
		while(getline(myfile,line)){
			add_contraints(line);
			cout << line << endl;
		}
		cout << "----------------------------------------------------------" << endl;
		/*
		cout << "Number of variables: " << states.size() << endl;
		for(int i = 0; i < states.size(); i++){
			cout << "For " << states[i]->name << " ";
			for(int j = 0; j < states[i]->domains.size(); j++){
				cout << states[i]->domains[j] << " ";
			}
			cout << endl;
		}*/

	// -------------------------------------------------------------------
	// 							BACKTRACKING
	// -------------------------------------------------------------------

	vector<bool> finished (states.size(), false);	// initialized vector of falses to indicate no state is complete 

	if(backtrack(finished, states))					// calling backtrack search
		cout << "success!!!" << endl;
	else
		cout << "failed!!!" << endl;

	for(int i = 0; i < states.size(); i++){			// print out final states and colors 
		cout << states[i] -> name << ": " << states[i]-> location << endl;
	}
	cout << endl << "iteration: " << iteration << endl; 
	
	}
	else
		cout << "File not found" << endl;
  	myfile.close();
}

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
// --------------------- LOCAL FUNCTION ----------------------------
	vector<string> _split(string s){
		stringstream ss(s);
		istream_iterator<string> _begin(ss);
		istream_iterator<string> _end;
		vector<string> _vstrings(_begin, _end);

		return _vstrings;
	}

	void create_states(string s){
		vector <string> _states = _split(s);
		for(int i = 0; i < _states.size(); i++){
			Node * temp = new Node(_states[i]);
			states.push_back(temp);
		}
	}

	void add_domains(string s){
		vector<string> tokens = _split(s);
		for(int i = 0; i < states.size(); i++){
			if(states[i]->state == tokens[0]){
				for(int j = 1; j < tokens.size(); j++){
					states[i] -> domains.push_back(tokens[j]);
				}
			}
		}

	}
	void add_contraints(string s){
		vector<string> c = _split(s);
		constraints.push_back(make_pair(c[1],c[2]));
	}

	vector<Node *> names_to_nodes(vector<string> s){
		vector<Node *> ss;
		for(int i = 0; i < s.size(); i++){
			for(int j = 0; j< states.size(); j++){
				if(states[j] -> state == s[i])
					ss.push_back(states[j]);
			}
		}
		return ss;
	}


	bool satisfied(Node * s, string c){
		string name = s -> state;
		vector<string> neighbors;
		for(int i = 0; i< constraints.size(); i++){
			if(constraints[i].first == name)
				neighbors.push_back(constraints[i].second);
			else if(constraints[i].second == name)
				neighbors.push_back(constraints[i].first);
		}
		/*cout << "neighbors: " << endl;
		for(int i = 0; i<neighbors.size(); i++)
			cout << neighbors[i] << " ";
		cout << endl;*/
		vector<Node *> s_neighbors = names_to_nodes(neighbors);
		for(int i = 0; i < s_neighbors.size(); i++){
			if(s_neighbors[i] -> color == c)
				return false;
		}

		return true;

	}

	bool backtrack(vector<bool> assignment, vector <Node *> st){
		bool finished = true;
		vector <Node *> s = st;
		int counter = 0;
		int min_value = 4;
		vector<bool> new_assignment = assignment;
		int index;
		for(int i = 0; i< assignment.size(); i++){
			if(assignment[i] == false){
				for(int j = 0; j< s[i]->domains.size(); j++){
					if(satisfied(s[i], s[i]->domains[j])){
						counter++;
					}

				}
				if (counter < min_value){
					min_value = counter;
					index = i;
				}
				counter = 0;
				finished = false;

			}	
		}

		if(finished){
			states = st;
			return true;
		}
		cout << "Picking state: " << s[index] -> state << endl;
		for(int i = 0; i< s[index]->domains.size(); i++){
			//cout << "Trying: " << s[index]->domains[i] << " to " << s[index] -> state << endl;
			if(satisfied(s[index], s[index]->domains[i])){
				cout << "\tAdding: " << s[index]->domains[i] << " to " << s[index] -> state << endl;
				s[index] -> color = s[index]->domains[i];
				new_assignment[index] = true;
				if(backtrack(new_assignment, s))
					return true;
			}
		}
		return false;
	}



int main(){

	// --------------------- LOCAL VARIABLES ----------------------------
	ifstream myfile;
	string line, filename;

	// -------------------------------------------------------------------
	// 						PRE-PROCESSING STAGE
	// -------------------------------------------------------------------
	
	cout << "Enter file name!" << endl;
	cin >> filename; 


	myfile.open(filename.c_str());
	if (myfile.is_open()){
		getline(myfile,line);
		getline(myfile,line);	// get states
		cout << line << endl;	
		create_states(line);		// create vector of states

		getline(myfile,line);
		cout << "Domains!" << endl;
		getline(myfile,line);
		while(!myfile.eof() && line != "#constraints"){
			//add line to constraint
			add_domains(line);		// add domains to each node
			cout << line << endl;
			getline(myfile,line);
		}


		//getline(myfile, line);
		cout << "constraint" << endl;
		while(getline(myfile,line)){
			add_contraints(line);
			cout << line << endl;
		}


	// -------------------------------------------------------------------
	// 							BACKTRACKING
	// -------------------------------------------------------------------
		/*cout << "number of states: " << states.size() << endl;
		for(int i = 0; i < states.size(); i++){
			cout << states[i] -> state << " ";
			for(int j = 0; j < states[i] -> domains.size(); j++){
				cout << states[i] -> domains[j] << " ";
			}
			cout << endl;
		}*/



	vector<bool> finished;
	for(int i = 0; i < states.size(); i++){
		finished.push_back(false);
	}

	if(backtrack(finished, states)){
		cout << "success!!!" << endl;
	}
	else
		cout << "failed!!!" << endl;

	for(int i = 0; i < states.size(); i++){
		cout << states[i] -> state << ": " << states[i]-> color << endl;
	}

	}
	else
		cout << "File not found" << endl;
  	myfile.close();
}

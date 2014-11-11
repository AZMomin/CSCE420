#include <iostream>
#include <vector>
#include <stack>

using namespace std;

class Node{
	public:
		string name; 
		string location; 
		int time;
		vector<string> domains;
		Node(string _name);
};

#include <iostream>
#include <vector>
#include <stack>

using namespace std;

class Node{
	public:
		string state; 
		string color; 
		vector<string> domains;
		Node(string _state);
};

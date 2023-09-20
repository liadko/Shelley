#pragma once
#include <string>

using std::string;

class Dict
{
private:
	struct Node
	{
		string key;
		string value;
		Node* next;
		Node(string key, string value, Node* next);
		~Node();
	};

	static int hash(string key);

	Node** data;
	int size;

public:
	Dict(int size);
	bool containsKey(string key) const;
	string getValue(string key) const;
	bool setValue(string key, string value);
	void add(string key, string value);
	void reset();
};

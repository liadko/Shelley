#include <iostream>
#include "Dict.h"

int Dict::hash(string key)
{
	int hash = 5381;
	int c;
	const char* str = key.c_str();

	while ((c = *str++))
	{
		hash = (hash << 5 + hash) + c;
	}
	return hash;
}

Dict::Dict(int size) : size(size)
{
	data = new Node * [size]();
}


bool Dict::containsKey(string key) const
{
	int index = hash(key) % size;
	Node* current = data[index];
	while (current)
	{
		if (data[index]->key == key)
			return true;

		current = current->next;
	}
	return false;
}

string Dict::getValue(string key) const
{
	int index = hash(key) % size;
	Node* current = data[index];
	while (current)
	{
		if (data[index]->key == key)
			return data[index]->value;

		current = current->next;
	}
	return "\0";
}

void Dict::add(string key, string value)
{
	int index = hash(key) % size;
	data[index] = new Node(key, value, data[index]);
}

void Dict::reset()
{
	for (int i = 0; i < size; i++)
	{
		delete data[i];
		data[i] = nullptr;
	}
}




// NODE

Dict::Node::Node(string key, string value, Node* next)
	: key(key), value(value), next(next) { };


Dict::Node::~Node()
{
	if (next)
		delete next;
}
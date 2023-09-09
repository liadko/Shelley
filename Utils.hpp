#pragma once
#include <iostream>
#include <sstream>
#include <vector>


using std::string;
using std::vector;

vector<string> SplitString(const string& s);
string CombineIntoString(const vector<string>& v);
string ToLower(const string& s);
string LastPart(const string&);
int CountLetter(const string& s, char c);
int IndexOf(const string& s, char c);
string TrimString(const string& s);
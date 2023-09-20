#pragma once
#include <iostream>
#include <sstream>
#include <vector>
#include <fstream>


using std::string;
using std::vector;

vector<string> SplitString(const string& s, char splitter);
string CombineIntoString(const vector<string>& v);
string ToLower(const string& s);
string LastPart(const string&);
int CountLetter(const string& s, char c);
int IndexOf(const string& s, char c);
string TrimString(const string& s);
void WriteToFile(const string& path, const string& s);

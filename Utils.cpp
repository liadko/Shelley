#include "Utils.hpp"


vector<string> SplitString(const string& s)
{
	std::stringstream ss(s);
	vector<string> words;

	string word;
	while (ss >> word)
		words.push_back(word);

	return words;
}

string CombineIntoString(const vector<string>& v)
{
	if (v.empty())
		return "";

	string result = v[0];
	for (int i = 1; i < v.size(); i++)
		result += " " + v[i];

	return result;
}

string ToLower(const string& s)
{
	string result = s;
	for (int i = 0; i < result.size(); i++)
		result[i] |= (1 << 5);

	return result;
}

string LastPart(const string& path)
{
	string new_path = path;

		for (int i = path.size() - 1; i >= 0; i--)
		{
			if (path[i] == '/' || path[i] == '\\')
			{
				new_path = path.substr(i + 1, path.size() - 1);
				break;
			}
		}

	return new_path;
}

int CountLetter(const string& s, char c)
{
	int count = 0;
	for (int i = 0; i < s.size(); i++)
		if (s[i] == c)
			count++;
	return count;
}

int IndexOf(const string& s, char c)
{
	for (int i = 0; i < s.size(); i++)
		if (s[i] == c)
			return i;
	return -1;
}

string TrimString(const string& s)
{
	int firstNonWhiteSpaceIndex = 0;
	for (int i = 0; i < s.size(); i++)
		if (!std::isspace(s[i]))
		{
			firstNonWhiteSpaceIndex = i;
			break;
		}

	int lastNonWhiteSpaceIndex = s.size() - 1;
	for (int i = s.size()-1; i >= firstNonWhiteSpaceIndex; i--)
		if (!std::isspace(s[i]))
		{
			lastNonWhiteSpaceIndex = i;
			break;
		}

	return s.substr(firstNonWhiteSpaceIndex, lastNonWhiteSpaceIndex - firstNonWhiteSpaceIndex + 1);
}
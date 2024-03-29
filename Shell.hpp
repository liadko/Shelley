#pragma once

#include <iostream>
#include <filesystem>
#include <fstream>
#include <stdio.h>

#include "Dict.hpp"

namespace fs = std::filesystem;
using std::string;
using std::vector;
using std::cout;

namespace Shell {

	bool MainLoop();

	string Prompt();
	
	//string RecursiveEvaluateCommand(const string& command, const string& program_input_string="");
	string ExecuteCommand(const string& command_args_string, const string& program_input_string="", bool toSTDOUT = false);

	string ParseUserInput(const string& user_input);
	string Echo(const vector<string>& args);
	string Clear();
	string Dir(const vector<string>& args);
	string Pwd();
	string Cd(const vector<string>& args);
	string RmDir(const vector<string>& args);
	string MkDir(const vector<string>& args);
	string Touch(vector<string>& args);
	string Type(const vector<string>& args);
	string Set(vector<string>& args);
	string ToggleShortPrompt();
	string RunProgram(const string& full_path, const vector<string>& args, const string& input="", bool toSTDOUT=false);
};

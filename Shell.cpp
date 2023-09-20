#include "Shell.hpp"
#include "Utils.hpp"

namespace Shell {

	fs::path current_path{ "E:\\Liadi_Google_Drive\\Liad\\Code\\CPP\\ForShelley" };
	fs::path system_path{ "C:\\Windows\\System32" };

	bool short_prompt = true;

	Dict variables(100);

	/// <returns>False if the program should be terminated</returns>
	bool MainLoop()
	{
		// print prompt
		cout << Prompt();

		string user_input;
		std::getline(std::cin, user_input);

		if (user_input == "exit" || user_input == "quit")
			return false;

		// feed each command into the next
		string prev_output = "";
		string expression = ParseUserInput(user_input);

		int pipe_location = IndexOf(expression, '|');
		while (pipe_location != -1)
		{
			string left_part = expression.substr(0, pipe_location);
			string right_part = expression.substr(pipe_location + 1);

			prev_output = ExecuteCommand(left_part, prev_output);
			expression = right_part;

			pipe_location = IndexOf(expression, '|');
		}

		// maybe redirect to file
		string output;
		int index_of_redirect = IndexOf(expression, '>');
		if (index_of_redirect > 0)
		{
			string filename = expression.substr(index_of_redirect + 1);
			fs::path file_to_write = current_path / TrimString(filename);
			output = ExecuteCommand(expression.substr(0, index_of_redirect), prev_output);
			

			cout << "Redirecting output to file at " << file_to_write.string() << "\n\n";

			WriteToFile(file_to_write.string(), output);

			return true;
		}


		output = ExecuteCommand(expression, prev_output, 1);
		cout << output << "\n\n";

		return true;
	}
	//Swap all $KEYS$ with their values
	string ParseUserInput(const string& user_input)
	{
		string expression = "";
		vector<string> words = SplitString(user_input, '$');
		for (int i = 0; i < words.size(); i ++)
		{
			if (i % 2 == 0)
				expression += words[i];
			else if (variables.containsKey(words[i]))
				expression += variables.getValue(words[i]);
			else
				expression += "$" + words[i] + "$";


		}
		return expression;

	}
	string Prompt()
	{
		if(short_prompt)
			return current_path.root_name().string() + "\\...\\" + current_path.filename().string() + " >";
		return Pwd() + ">";
	}
	string ExecuteCommand(const string& command__and_args, const string& program_input, bool toSTDOUT)
	{
		if (command__and_args == "")
			return "";

		string stripped_command_args = TrimString(command__and_args);
		vector<string> command_args_words = SplitString(stripped_command_args, ' ');

		string command = command_args_words[0];
		string command_lower = ToLower(command);

		vector<string>& args = command_args_words;
		args.erase(args.begin()); // erase the command from args


		if (command_lower == "echo")
			return Echo(args);

		if (command_lower == "cls" || command_lower == "clear")
			return Clear();

		if (command_lower == "dir" || command_lower == "ls")
			return Dir(args);

		if (command_lower == "cd")
			return Cd(args);

		if (command_lower == "pwd")
			return Pwd();

		if (command_lower == "rmdir")
			return RmDir(args);

		if (command_lower == "mkdir")
			return MkDir(args);

		if (command_lower == "touch")
			return Touch(args);

		if (command_lower == "type" || command_lower == "cat")
			return Type(args);

		if (command_lower == "set")
			return Set(args);

		if (command_lower == "changeprompt")
			return ToggleShortPrompt();

		string path = command;
		// search directory for matching file, and execute it
		for (int path_version = 0; path_version <= 1; path_version++)
		{
			if (path_version == 1) // also look for version with .exe on end
				path += ".exe";

			for (const auto& entry : fs::directory_iterator(current_path))
			{
				if (entry.is_regular_file() && entry.path().filename() == path)
					return RunProgram(current_path.string() + "/" + path, args, program_input, toSTDOUT);
			}

			// search environment variable path
			for (const auto& entry : fs::directory_iterator(system_path))
			{
				if (entry.is_regular_file() && ToLower(entry.path().filename().string()) == path)
					return RunProgram(system_path.string() + "/" + path, args, program_input, toSTDOUT);
			}
		}

		//unrecognised input
		return  "Command or File named '" + command + "' is unrecognised.";
	}
	string RunProgram(const string& full_path, const vector<string>& args, const string& input, bool toSTDOUT)
	{
		//cout << "Call to RunProgram: " << full_path << ", " << CombineIntoString(args) << ", " << input << "\n";

		string command_str(full_path + " " + CombineIntoString(args));

		const char* command = command_str.c_str();

		FILE* pipe; // Open a pipe to command
		if (input == "")
			pipe = _popen(command, "r");
		else
			pipe = _popen(command, "w");


		if (!pipe) return "Pipe Failed.\n";

		if (input != "")
			fprintf(pipe, "%s", input.c_str());

		string output = "";

		char buffer[100];
		while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {

			if (toSTDOUT)
				cout << buffer;
			else
				output += buffer;

		}

		//delete command;
		//delete command_str;
		_pclose(pipe);

		if (toSTDOUT) //already printed output to screen
			return "";

		return output;

	}
	string Echo(const vector<string>& args)
	{
		return CombineIntoString(args);
	}
	string Clear()
	{
		system("cls");
		return "";
	}
	string Dir(const vector<string>& args)
	{
		string directory;
		for (const auto& entry : fs::directory_iterator(current_path))
		{
			if (entry.is_directory())
				directory += "<DIR>";
			directory += "\t" + entry.path().filename().string();
			directory += "\n";
		}
		return directory;
	}
	string Pwd()
	{
		return current_path.string();
	}
	string Cd(const vector<string>& args)
	{
		string requested_path = CombineIntoString(args);

		if (requested_path == "")
			return Pwd();

		if (requested_path == ".")
			return "";

		if (requested_path == "..")
		{
			current_path = current_path.parent_path();
			return "";
		}

		fs::path localPath = current_path / requested_path;
		if (fs::exists(localPath) && fs::is_directory(localPath))
		{
			current_path = localPath;
			return "";
		}

		fs::path absolutePath = fs::path(requested_path);
		if (fs::exists(absolutePath) && fs::is_directory(absolutePath))
		{
			current_path = absolutePath;
			return "";
		}


		return "Can't Find Path '" + requested_path + "'.";

	}
	string RmDir(const vector<string>& args)
	{
		if (args.size() == 0)
			return "No arguments were provided.";

		//create directory for each argument
		string output = "";
		for (const string& arg : args) {

			fs::path dir_path = arg;
			if (!dir_path.is_absolute())
				dir_path = current_path / arg;

			if (!fs::is_directory(dir_path))
			{
				output += "Cannot find directory named '" + dir_path.string() + "'.\n";
				continue;
			}

			if (!fs::is_directory(dir_path))
			{
				output += "Cannot find directory named '" + dir_path.string() + "'.\n";
				continue;
			}

			std::error_code ec;
			if (fs::remove(dir_path, ec))
				output += "Removed directory at '" + dir_path.string() + "'.\n";
			else
				output += "Remove directory failed, maybe the directory isn't empty.\n";
		}
		return output;
	}
	string MkDir(const vector<string>& args)
	{
		if (args.size() == 0)
			return "No arguments were provided.";

		//create directory for each argument
		string output = "";
		for (const string& arg : args) {

			fs::path dir_path = arg;
			if (!dir_path.is_absolute())
				dir_path = current_path / arg;

			if (fs::exists(dir_path))
			{
				output += "subdirectory or file '" + dir_path.string() + "' Already exists.\n";
				continue;
			}

			fs::create_directory(dir_path);
			output += "Created directory at '" + dir_path.string() + "'.\n";
		}
		return output;
	}
	string Touch(vector<string>& args)
	{
		string output = "";
		output += "Creating file with name: '" + args[0] + "'.";
		std::ofstream ofs(current_path / args[0]);
		//write the rest of the arguments to the file
		args.erase(args.begin());
		if (args.size() > 0)
		{
			string initial_text = CombineIntoString(args);
			output += "\nInitializing file with text '" + initial_text + "'.";
			ofs << initial_text;
		}
		ofs.close();
		return output;
	}
	string Type(const vector<string>& args)
	{
		if(args.size() < 1)
			return "Not enough arguments.\nExample: type [FILENAME]";

		string file_path = (current_path / args[0]).string();

		FILE* file;
		if(fopen_s(&file, file_path.c_str(), "r"))
			return "Can't find readable file named '" + args[0] + "'.\n";

		string output;
		char buffer[100];
		while (fgets(buffer, 100, file) != nullptr)
		{
			output += buffer;
		}

		fclose(file);

		return output;

	}
	string Set(vector<string>& args)
	{
		if (args.size() < 2)
			return "Not enough arguments.\nExample: set [VARIABLE_NAME] [VARIABLE VALUE]\nUse: echo $[VARIABLE_NAME]$";

		string key = args[0];
		args.erase(args.begin());
		string value = CombineIntoString(args);

		if (CountLetter(key, '$') > 0 || CountLetter(value, '$') > 0)
			return "Variable or value has '$' in it. that's not allowed.";

		if (variables.containsKey(key))
			variables.setValue(key, value);
		else
			variables.add(key, value);


		return "Variable Named $" + key + "$ has value '" + value + "'.";

	}
	string ToggleShortPrompt()
	{
		short_prompt = !short_prompt;
		return "Prompt type changed.";
	}
};
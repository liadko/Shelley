///////////
//
//	OLD CODE. Don't Write Here!
// 
////////

//#include "ShellHeader.hpp"

//#include <cstdlib>

fs::path currentPath{ "E:\\Liadi_Google_Drive\\Liad\\Code\\CPP" };

int ShellMainLoop()
{
	cout << currentPath.string() << ">";

	//user input
	std::string userInput;
	std::getline(std::cin, userInput);

	//handle empty input
	if (userInput == "")
		return 1;

	//parse command
	ParsedCommand parsedCommand = ParseInput(userInput);

	//execute command based on the parsing of it

	if(!parsedCommand.isPipe)
		return ExecuteCommand(parsedCommand);
	
	//it is a pipe


}

int ExecuteCommand(ParsedCommand& parsedCommand)
{
	switch (parsedCommand.commandCode)
	{
	case Code::Exit:
		return 0;
	case Code::Echo:
		cout << CombineIntoString(parsedCommand.args) << '\n';
		return 1;
	case Code::Error:
		cout << parsedCommand.args[0] << "\n";
		return 1;
	case Code::Cls:
		system("cls");
		return 1;
	case Code::Dir:
		for (string arg : parsedCommand.args)
			cout << arg << '\n';
		return 1;
	case Code::Cd:
	{
		string requestedPath = CombineIntoString(parsedCommand.args);

		if (requestedPath == "")
		{
			cout << currentPath.string() << '\n';
			return 1;
		}
		if (requestedPath == "..")
		{
			currentPath = currentPath.parent_path();//BackwordsPath(currentPath.string());
			return 1;
		}
		fs::path localPath = currentPath / requestedPath;
		if (fs::exists(localPath) && fs::is_directory(localPath))
		{
			currentPath = localPath;
			return 1;
		}
		fs::path absolutePath = fs::path(requestedPath);
		if (fs::exists(absolutePath) && fs::is_directory(absolutePath))
		{
			currentPath = absolutePath;
			return 1;
		}
		//search locally

		cout << "Can't Find Path '" << requestedPath << "'.\n";

		return 1;
	}


	case Code::MkDir:
		cout << "Creating directory with name: '" << parsedCommand.args[0] << "'\n";
		fs::create_directory(currentPath / parsedCommand.args[0]);
		return 1;
	case Code::Touch:
	{
		cout << "Creating file with name: '" << parsedCommand.args[0] << "'\n";
		std::ofstream ofs(currentPath / parsedCommand.args[0]);
		//write the rest of the arguments to the file
		parsedCommand.args.erase(parsedCommand.args.begin());
		if (parsedCommand.args.size() > 0)
		{
			string text = CombineIntoString(parsedCommand.args);
			cout << "Initializing file with text '" << text << "'\n";
			ofs << text;
		}
		ofs.close();
		//fs::create(fs::path(currentPath.string() + "/" + parsedCommand.args[0]));
		return 1;
	}

	}

	cout << "Error: Command code not handled in switch statement\n";
	//couldn't parse command
	return 1;
}

ParsedCommand ParseInput(const string& userInput)
{
	int pipeCount = CountLetter(userInput, '|');

	if (pipeCount > 1)
		return { Code::Error, vector<string> { "Too many pipes in the command" } };

	if (pipeCount == 0)
		return ParseCommand(userInput);


	int index = userInput.find('|');
	//got one pipe
	string command1 = userInput.substr(0, index);
	string command2 = userInput.substr(index + 1);
	ParsedCommand parsedCommand = ParseCommand(command1);
	cout << command1 << "\n";
	cout << command2 << "\n";

	return { Code::Echo, vector<string> {"Yeah Bro"}, true };
}

ParsedCommand ParseCommand(const string& commandString)
{
	vector<string> words = SplitString(commandString);


	string command = ToLower(words[0]);
	words.erase(words.begin());

	vector<string>& args = words;

	if (command == "exit")
		return { Code::Exit };

	if (command == "echo")
		return { Code::Echo, args };

	if (command == "cls" || command == "clear")
		return { Code::Cls };

	if (command == "dir" || command == "ls")
	{
		vector<string> directory;
		for (const auto& entry : fs::directory_iterator(currentPath))
		{
			string line = "";
			if (entry.is_directory())
				line += "<DIR>";
			line += "\t" + entry.path().filename().string();
			directory.push_back(line);
		}
		return { Code::Dir, directory };
	}
	if (command == "cd")
		return { Code::Cd, args };
	if (command == "mkdir")
		return { Code::MkDir, args };
	if (command == "touch")
		return { Code::Touch, args };

	for (const auto& entry : fs::directory_iterator(currentPath))
	{
		if(entry.is_regular_file() && entry.path().filename() == )
	}


	//unrecognised input
	return { Code::Error, vector<string> { "Command '" + command + "' is unrecognised." } };
}




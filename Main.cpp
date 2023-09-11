#include <iostream>
#include "Shell.hpp"
#include <windows.h>
#include "Utils.hpp"

#include "Dict.h"

int main()
{
	SetConsoleTitleW(L"Shelley CLI");

	const char* titleText = R"""(
	 _____ _          _ _            
	/  ___| |        | | |           
	\ `--.| |__   ___| | | ___ _   _ 
	 `--. | '_ \ / _ | | |/ _ | | | |
	/\__/ | | | |  __| | |  __| |_| |
	\____/|_| |_|\___|_|_|\___|\__, |
	                            __/ |
	         By Liad Koren     |___/	)""";
	


	std::cout << titleText << "\n\n\n\n";

	Dict dic(100);
	dic.add("hey", "heyo there!");
	dic.add("bra", "this is a dictionary check");
	
	cout << dic.containsKey("yo") << "\n";
	cout << dic.containsKey("hey") << "\n";
	cout << dic.getValue("hey") << "\n";
	cout << dic.getValue("bra") << "\n";
	
	bool alive = true;
	while (alive)
	{
		alive = Shell::MainLoop();
	}




	return 0;
}
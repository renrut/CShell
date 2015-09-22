#include "Shell.h"
#include <string>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include "ShellParser.h"

/*
 * Reactor/Main method for our shell.
 * Prints out current path and reads command from user
 * Sends command to parser to be parsed
 * @author Turner Strayhorn
 */

int main(){
	
	//gets instance of shell singleton
	Shell* myShell = Shell::getInstance();
	//declares string that will be user's command
	std::string command;
	//reads user input as long as it is not 'exit'
	while(command != "exit"){
		std::cout<<(myShell->getPath())<<"/$";
		std::getline(std::cin, command);
		//sends command to parser
		ShellParser::parseCommand(command);
		
		std::cout<<std::endl;
	}
	return 0;
}



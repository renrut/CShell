#ifndef SHELLPARSER_H
#define SHELLPARSER_H

#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "Shell.h"
#include <vector>
#include "Process.h"
#include "ShellCommand.h"
#include <memory>

	/**
	 * Parser for our shell commands 
	 * @Author Turner Strayhorn
	 */


class ShellParser{
public:
	
    /**
     * Parses command, pushes it into a string vector
     * searches for splits based off of " " character
     * creates process out of it
     */
	static void parseCommand(std::string command);
	
	
	/**
	 * iterates through a vector of valid commands
	 * returns whether the user command exists in the vectors of 
	 * valid commands.
	 */
	static bool isValidCommand(std::vector<std::string> myCommands);

	/**
	 * parses multiple jobs from user commands and 
	 * pipes or deals with output accordingly
	 * Does so by looking for special characters
	 */
	static void parseJobs(std::vector<std::unique_ptr<Job>> &jobs, std::vector<std::string> &myCommands);
		
	
};


#endif

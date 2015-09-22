#ifndef SHELLCOMMAND_H
#define SHELLCOMMAND_H

#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "Shell.h"
#include <vector>

	/**
	 * Special built in commands for our shell 
	 * @Author Turner Strayhorn
	 */

class ShellCommand{
	
	friend class Shell;
	
public:
	
	/**
	 * Constructor
	 * Special built in commands for our shell 
	 */
	ShellCommand(std::vector<std::string> command);
	/**
	 * Dtor
	 */
	~ShellCommand();
	
	/**
	 * Demuxes the command and runs it accordingly
	 */
	void demuxCommand();
	
	/**
	 * shows help menu
	 */
	void showHelp();
	
private:
	std::vector<const char *> myCommand;
	Shell *shell;

};


#endif

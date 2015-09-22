
#include "ShellCommand.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>



ShellCommand::ShellCommand(std::vector<std::string> command):myCommand(Shell::stringToCharVector(command)), shell(Shell::getInstance()){

	demuxCommand();
}

ShellCommand::~ShellCommand(){
	
}

void ShellCommand::demuxCommand(){
	std::string searchStrng = myCommand.front();

	myCommand.erase(myCommand.begin());
	//changes directory
	if(searchStrng == "cd"){
		if(chdir(myCommand[0]) != 0){
			perror("cd");
		}
	}
	//shows directory of shell
	else if(searchStrng == "dirs"){
		std::cout<<"~"<<shell->getPath()<<std::endl;
	}
	//execs process and finishes
	else if(searchStrng == "exec"){
      	execvp(myCommand[0], const_cast<char **>(&myCommand[0]));
      	perror("exec");
	}
	//waits for process to finish
	else if(searchStrng == "wait"){
		
		int status = 0;
		pid_t jobPid_ = atoi(myCommand[0]);
		waitpid(jobPid_, &status,  0);
		std::cout<<"Exit Status: "<<status<<std::endl;
		
	}
	//shows help menu
	else if(searchStrng == "help"){
		showHelp();	
	}
}

/*Called if the user types help
 *Prints out help information for the user and lists the supported
 * commands
 * 
 */
void ShellCommand::showHelp(){
	std::cout<<"<~~~~~~Help Menu~~~~~~>"<<std::endl;

	std::cout<<"The following are valid commands:"<<std::endl;

	for(auto i : shell->getValidCommands()){
		std::cout<<i<<std::endl;
	}	
}




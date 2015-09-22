#include "Shell.h"
#include <sys/types.h>
#include <unistd.h>
#include <termios.h>
#include <signal.h>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>

Shell* Shell::instance_ = nullptr;
std::vector<std::string> Shell::validCommands_{"cd","dirs","exec","goto","jobs","repeat","wait", "help"};


Shell::Shell() : pid_(getpid()), pgid_(getpid()), terminal_(STDIN_FILENO){

    //ignore signals
    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);
    signal(SIGTTIN, SIG_IGN);
    signal(SIGTTOU, SIG_IGN);
    signal(SIGCHLD, SIG_IGN);

    //put shell in its own process grouop
    setpgid(pgid_, pgid_);

    //gain control of terminal
    tcsetpgrp (terminal_, pgid_);

    //set home as the working directory
    if(chdir("/") != 0)
    	perror("cd");
}

Shell::~Shell(){
}

Shell* Shell::getInstance(){
    if (!instance_)
        instance_ = new Shell();
    return instance_;
}

pid_t Shell::getPid(){
    return pid_;
}

pid_t Shell::getPgid(){
    return pgid_;
}

int Shell::getTerminal(){
	return terminal_;
}
std::string Shell::getPath(){
	char * cDir = get_current_dir_name();
	std::string dir(cDir);
	delete cDir; //get_current_dir_name allocates memory that I don't want
    return dir;
}

const std::vector<std::string> & Shell::getValidCommands(){
    return validCommands_;//built-in shell commands, that is
}

//because of so many c functions, our string vectors needed to be char * vectors
std::vector<const char *> Shell::stringToCharVector(const std::vector<std::string> &args){
	std::vector<const char *> charArgs;
	std::transform(args.begin(), args.end(), std::back_inserter(charArgs),
		[](const std::string & s){return s.c_str();});
	charArgs.push_back(nullptr); //args must end with null for exec
	return charArgs;
}







#include "Process.h"
#include "Shell.h"
#include <unistd.h>
#include <signal.h>
#include <iostream>
Process::Process(pid_t pgid, bool foreground, const std::vector<std::string>& argv)  : pid_(0),pgid_(pgid), foreground_(foreground),  in_(STDIN_FILENO), out_(STDOUT_FILENO), err_(STDERR_FILENO),argv_(Shell::stringToCharVector(argv)){
//empty
}

pid_t Process::launch(){
	pid_ = fork();
	if (pid_ == 0){//child

    	pid_ = getpid();
    	//if the Job has not set this Process's pgid, make it this pid
    	if (pgid_ == 0)
    		pgid_ = pid_;
    	setpgid (pid_, pgid_);

    	//grab the terminal if in foreground
    	//both Job and Process should do this to avoid race conditions
    	if(foreground_){
    		tcsetpgrp(Shell::getInstance()->getTerminal(), pgid_);
    	}
    	
    	//allow job control signals again
    	//these were set to ignore in Shell
    	signal (SIGINT, SIG_DFL);
    	signal (SIGQUIT, SIG_DFL);
      	signal (SIGTSTP, SIG_DFL);
      	signal (SIGTTIN, SIG_DFL);
      	signal (SIGTTOU, SIG_DFL);
      	signal (SIGCHLD, SIG_DFL);
      	
      	//change std in out of new process
      	if (in_ != STDIN_FILENO){
      		dup2(in_, STDIN_FILENO);
      		close(in_);
      	}
      	if(out_ != STDOUT_FILENO){
      		dup2(out_, STDOUT_FILENO);
      		close(out_);
      	}
      	if(err_ != STDERR_FILENO){
      		dup2(err_, STDERR_FILENO);
      		close(err_);
      	}

      	//exec based on this path. if the process is not in the current directory,
      	//exec looks in the PATH environment variable.
      	//&argv_[0] gives an array of arguments that ends with null
      	execvp(argv_[0], const_cast<char **>(&argv_[0]));

      	//we should not reach these two lines
      	perror("exec");
		exit(1);

    }else if (pid_ > 0){//parent
    }else{//error
    	perror("fork");
    }

	//only returned outside of child
	return pid_;
}

void Process::setFileDescriptors(int in, int out, int err){
	in_ = in;
	out_ = out;
	err_ = err;
}

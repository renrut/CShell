#include "Job.h"
#include "Shell.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

Job * Job::makeJob(){
	return new Job(STDIN_FILENO, STDOUT_FILENO, STDERR_FILENO, true);
}

Job::Job(int in, int out, int err, bool foreground) : pgid_(0), in_(in), out_(out), err_(err), foreground_(foreground){
}

Job::~Job(){//close these files so I can access them later
	if(in_ != STDIN_FILENO){
		close(in_);
	}
	if(out_ != STDOUT_FILENO){
		close(out_);
	}
	if(err_ != STDERR_FILENO){
		close(err_);
	}
}

void Job::addProcess( const std::vector<std::string> & process){
	Process p(pgid_, foreground_, process);
	processes_.push_back(p);

}

void Job::launch(){

	//setup pipes and redirection
	int pipefd[2];
	int in = in_, out, err = err_;
	for (size_t i = 0; i < processes_.size(); ++i){

		/*PIPING*/
		if(i < processes_.size() - 1){//if this is not the last process, pipe to the next process
			if(pipe(pipefd) < 0)//if pipe does not return 0 it fails
				perror("pipe");
			//	exit(EXIT_FAILURE);
				out = pipefd[1];
		}else out = out_;//the last process should output to the output of the job

		processes_[i].setFileDescriptors(in, out, err);

		pid_t pid = processes_[i].launch();// this will return the pid of the child process

		//if the pgid hasn't been set, set it to the first child
		if(!pgid_){
			pgid_ = pid;
			setChildrenPgid(pgid_);//no race conditions because no other processes have been launched
		}
		setpgid(pid, pgid_);//set the child process's group id to this job's id

		//close pipes
		if(in != in_){
			close(in);
		}
		if(out != out_){
			close(out);
		}
		in = pipefd[0];//allow next process to have the read end of pipe
	}
	if(foreground_)
		launchForeground();
	else launchBackground();
}

void Job::launchForeground(){
	//give terminal access to this process group
    tcsetpgrp (Shell::getInstance()->getTerminal(), pgid_);
    int status;
    waitpid(-1, &status,  0); // wait for all child processes
    //give the terminal back to the shell
	tcsetpgrp(Shell::getInstance()->getTerminal(), Shell::getInstance()->getPid());
}
void Job::launchBackground(){
	//empty
}

void Job::setChildrenPgid(pid_t pgid){
	for (Process p : processes_){
		p.pgid_ = pgid;
	}
}


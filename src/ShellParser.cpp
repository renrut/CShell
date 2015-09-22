#include "ShellParser.h"
#include "Job.h"
#include <unistd.h>
#include <memory>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <algorithm>


void ShellParser::parseCommand(std::string command){
	
	std::string parsingStr = command;
	if (command == "exit") return;
	size_t check = 0;
	std::vector<std::string> myCommands;


	//get a list of space-separated words
	while(check!=(std::string::npos)){
		//waits until npos is over
		check = parsingStr.find(" ");
		if(check == (std::string::npos)){
			if(parsingStr != "")
				myCommands.push_back(parsingStr);
		}else{
			if(parsingStr.substr(0, check) != "")
				myCommands.push_back(parsingStr.substr(0, check));
		}

		if(check != std::string::npos)
			parsingStr = parsingStr.substr(check+1);
	}


	std::vector<std::unique_ptr<Job>> jobs;
	parseJobs(jobs, myCommands);

	for(std::unique_ptr<Job>& j : jobs){

		j->launch();
	}

}
//checks to make sure it is a valid command
bool ShellParser::isValidCommand(std::vector<std::string> myCommands){
	std::vector<std::string>validCommands(Shell::getValidCommands());

	for(auto i : validCommands){
		if(myCommands.front() == i)
			return true;
	}
	return false;
}


//parses jobs, sets up pipes or input/output redirection
void ShellParser::parseJobs(std::vector<std::unique_ptr<Job>> &jobs, std::vector<std::string> &myCommands){
	if(!myCommands.empty() && isValidCommand(myCommands)){
		ShellCommand command(myCommands);
		return;
	}

	std::unique_ptr<Job> job(nullptr);

	std::vector<std::string> processArgs;

	for(size_t i = 0; i < myCommands.size(); ++i){
		if (job == nullptr)
			job.reset(Job::makeJob());

		std::string str(myCommands[i]);
		bool endOfCommand = false;

		if(str == "|"){//pipe
			std::copy(myCommands.begin(), myCommands.begin() + i, std::back_inserter(processArgs));
			myCommands.erase(myCommands.begin(), myCommands.begin() + i +1);
			i = -1;
			job->addProcess(processArgs);
			processArgs.clear();
		}
		else if(str == "<"){//input redirection
			if (i < myCommands.size() - 1){
				job->in_ = open(myCommands[i + 1].c_str(),O_RDWR);
				std::copy(myCommands.begin(), myCommands.begin() + i, std::back_inserter(processArgs));
				myCommands.erase(myCommands.begin(), myCommands.begin() + i + 2);
				i = -1;
			}
			endOfCommand = true;
		}
		else if(str == ">"){//output redirection

			if (i < myCommands.size() - 1){
				job->out_ = open(myCommands[i + 1].c_str(),O_RDWR | O_TRUNC | O_CREAT);
				std::copy(myCommands.begin(), myCommands.begin() + i, std::back_inserter(processArgs));
				myCommands.erase(myCommands.begin(), myCommands.begin() + i + 2);
				i = -1;
			}
			endOfCommand = true;
		}
		else if (str == ">>"){//output redirection for appending

			if (i < myCommands.size() - 1){
				job->out_ = open(myCommands[i + 1].c_str(),O_RDWR | O_APPEND);
				std::copy(myCommands.begin(), myCommands.begin() + i, std::back_inserter(processArgs));
				myCommands.erase(myCommands.begin(), myCommands.begin() + i + 2);
				i = -1;
			}
			endOfCommand = true;
		}
		else if(str == "&"){//background
			std::copy(myCommands.begin(), myCommands.begin() + i, std::back_inserter(processArgs));
			myCommands.erase(myCommands.begin(), myCommands.begin() + i +1);
			i = -1;
			job->foreground_ = false;
			endOfCommand = true;
		}
		//releases unq ptr if job is over
		if(endOfCommand){
			job->addProcess(processArgs);
			jobs.push_back(std::unique_ptr<Job>(job.release()));
			processArgs.clear();
		}
	}
	if (!myCommands.empty() && job != nullptr){//parsed without a special character, so add the job
		job->addProcess(myCommands);
		jobs.push_back(std::unique_ptr<Job>(job.release()));
	}
}

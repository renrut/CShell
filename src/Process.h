#ifndef PROCESS_H
#define PROCESS_H
#include <string>
#include <vector>

/**
 * Process is where the Shell finally forks and execs the commands
 * @author Mark Karle
 */

//forward declaration
class Job;

class Process{
	friend Job;
 public:

	/*
	 * Constructor
	 * pid_, in_, out_, and err_ are set to 0, 0, 1, and 2 respectively
	 */
    Process(pid_t pgid,bool foreground, const std::vector<std::string> & argv);

    /*
     * forks and execs based on argv
     */
    pid_t launch();

    /*
     * changes in, out, and err to the appropriate file descriptors
     */
    void setFileDescriptors(int in, int out, int err);

 private:

    pid_t pid_;
    pid_t pgid_;

    //if the process should run in the foreground, foreground_ is true
    bool foreground_;

    //file descriptors for I/O
    int in_, out_, err_;
    //arguments for exec
    std::vector<const char *> argv_;
};
#endif //PROCESS_H

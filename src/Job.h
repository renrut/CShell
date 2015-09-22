#ifndef JOB_H
#define JOB_H
#include <vector>
#include <string>
#include "Process.h"

/**
 * A job builds processes that are strung together for piping
 * A job doesn't care about its original input and output, but
 * 	it closes them if they are not STD
 * 	Also, job just passes these file descriptors to Process, which dups them
 * Job conventions come from:
 * http://www.gnu.org/software/libc/manual/html_node/Implementing-a-Shell.html
 * especially for the job control techniques
 * @author Mark Karle
 */

class ShellParser;//forward declaration

class Job{
	friend ShellParser;
public:
	/*
	 * Factory method to make a job with STD file descriptors and runs in foreground
	 */
	static Job * makeJob();

	/*
	 * Closes file descriptors
	 */
	~Job();

	/*
	 * registers a process to this job
	 */
	void addProcess( const std::vector<std::string> & process);

	/*
	 * launches all processes in this job
	 */
	void launch();

private:

	/**
	 * CTor is private because of the factory method
	 */
	Job(int in, int out, int err, bool foreground);

	/*
	 * list of processes to be launched
	 */
	std::vector<Process> processes_;

	/*
	 * process group id for all the processes that will be launced
	 * starts as 0, but becomes the pid of the first process launched
	 */
	pid_t pgid_;

	//file descriptors for input output and error
	int in_, out_, err_;

	//whether or not the shell will wait for completion of job
	bool foreground_;

	//possibility for completed and stopped, but I decided against it

	/*
	 * waits for the processes to complete before returning control to the shell
	 */
	void launchForeground();

	/*
	 * does nothing, but provided in case of allowing for stopped jobs later
	 */
	void launchBackground();

	/*
	 * sets the pgid of all child processes after the first process is launched
	 */
	void setChildrenPgid(pid_t);
};

#endif

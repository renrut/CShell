#ifndef SHELL_H
#define SHELL_H
#include <string>
#include <vector>
/**
 * Singleton representing the running shell
 * Shell conventions come from
 * http://www.gnu.org/software/libc/manual/html_node/Implementing-a-Shell.html
 * especially for the initialization of the shell in the constructor
 * and job control techniques
 * @author Mark Karle
 */

class Shell{

 public:
    /**
     * Constructs a shell in the foreground
     * The shell ignores job control signals
     * The shell is in its own process group
     */
    Shell();

    /**
       deletes the instance of the shell;
     */
    ~Shell();
    Shell(const Shell&) = delete;

    /*
     * returns single instance of shell
     */
    static Shell* getInstance();

    /*
     * returns pid of shell
     */
    pid_t getPid();

    /**
     * returns group id of shell
     */
    pid_t getPgid();

    /**
     * returns int representing the terminal, used for terminal access
     */
    int getTerminal();

    /**
       returns the path that this shell is operating in
    */
    std::string getPath();

    /**
       returns commands that are implemented by the shell, such as
       change directory
    */
    static const std::vector<std::string> & getValidCommands();

    /**
     * returns a char * vector to use as arguments to c methods
     */
    static std::vector<const char *> stringToCharVector(const std::vector<std::string>&);

 private:
    static Shell* instance_;

    pid_t pid_;//pid of the shell
    pid_t pgid_;//groud id of the shell

    //used for gaining terminal access
    int terminal_;

    //list of built-in shell commands
    static std::vector<std::string> validCommands_;
};

#endif //SHELL_H

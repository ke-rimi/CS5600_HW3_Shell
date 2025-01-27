#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <cstring>


using namespace std;   


void printPrompt() {
    cout << "nush$ ";
}


void executeCommand(vector<string> &args) {
    // Convert vector of strings to array of char*
    vector<char*> argv;
    for (auto& arg : args) {
        argv.push_back(&arg[0]);
    }
    argv.push_back(nullptr);

    // Fork a child process
    pid_t pid = fork();
    if (pid < 0) {
        //fork failed
        cerr << "nush error: fork failed " << endl;
    } else if (pid == 0) {
        //child process
        execvp(argv[0], argv.data());
        cerr << "nush error: Command not found " << argv[0] << endl;
        exit(1);
    } else {
        //parent process
        int status;
        waitpid(pid, &status, 0); // wait for the child process to finish

        if (WIFEXITED(status)) {
            // Print the status code of the child process
            cout << "nush status: " << WEXITSTATUS(status) << endl;
        }
    }

}


int main() {
    string input;
    while (true) {
        printPrompt();
        if (!getline(cin, input)) {
            break; // exit on EOF 
        }

        if (input == "exit"){
            break;
        }

        //Tokenize input into arguments 
        vector<string> args;
        char* token = strtok(&input[0], " ");

        while (token != nullptr) {
            args.push_back(token);
            token = strtok(nullptr, " ");
        }

        if (!args.empty()) {
            executeCommand(args);
        }
    }

    return 0;
}



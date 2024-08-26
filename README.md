
# Custom Shell Program in C

This C program implements a simple custom shell that can execute commands, manage a history of commands, and handle basic shell operations such as changing directories (cd), exiting, and forking processes.


![laptop-with-terminal-big](https://user-images.githubusercontent.com/84729141/164443291-d47193ea-700a-4204-a07c-35b727f4a357.jpg)


## Features

- **Command Execution**: The shell can execute commands using the `execvp` function. It supports any command that can be run in a standard Unix shell.
- **Command History**: The shell maintains a history of commands along with the process IDs (PIDs) of the child processes that executed them.
- **Change Directory (cd)**: The shell supports the `cd` command to change the current working directory.
- **Exit Command**: The shell can be exited using the `exit` command.
- **Fork and Execute**: The shell creates a child process using `fork` and executes commands in the child process.

## Files

- **main.c**: The main file containing the implementation of the shell.
- **Makefile** (optional): A makefile to compile the program.

## Compilation

To compile the program, use the following command:

```bash
gcc -o custom_shell main.c
```

This will create an executable file named `custom_shell`.

## Running the Program

To run the program, execute the following command:

```bash
./custom_shell
```

The shell will start and prompt for commands. You can enter any valid Unix command. The shell also supports the following special commands:

- **exit**: Exit the shell.
- **history**: Display the history of commands executed in the shell.
- **cd [directory]**: Change the current working directory.

## Command History

The shell maintains a history of commands executed along with the process IDs (PIDs) of the child processes that executed them. The history can be viewed using the `history` command.

## Error Handling

- **fork failed**: If the `fork` system call fails, the program will print an error message and exit.
- **exec failed**: If the `execvp` function fails to execute a command, the program will print an error message.
- **cd failed**: If the `cd` command fails, the program will print an error message.

## License

This project is licensed under the MIT License.

## Author

Sagiv Antebi



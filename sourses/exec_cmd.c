#include "header.h"

void	exec_command(char **cmd, char **envp)
{
	pid_t	pid;

	(void)envp;
	pid = fork();
	if (pid < 0)
		perror("fork failed");
	else if (pid == 0)
	{
		if (execvp(cmd[0], cmd) == -1)
		{
			perror("execve failed");
			exit(EXIT_FAILURE);
		}
	}
	else
		waitpid(pid, NULL, 0);
}

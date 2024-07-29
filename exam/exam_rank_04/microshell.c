#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

//Write error message
int	ft_puterr(char *str)
{
	while (*str != '\0')
	{
		write (2, str, 1);
		str++;
	}
	return (1);
}

//cd command
int	ft_cd(char **av, int i)
{
	if (i != 2)
		return (ft_puterr("error: cd: bad arguments"));
	if (chdir(av[1]) == -1)
		return (ft_puterr("Error: can't change directory to "), ft_puterr(av[1]), ft_puterr("\n"));
	return (0);
}

/*Executes the commands*/
int	ft_exec(char **av, int i, char **envp)
{
	int	fd[2];
	int	status;
	int	has_pipe = av[i] && !strcmp(av[i],"|"); //Check if the command has a pipe, using a flag;

	if (!has_pipe && !strcmp(*av, "cd")) //If there's no pipe and the command is cd, call the ft_cd function
	{
		return ft_cd(av, i);
	}
	if (has_pipe && pipe(fd) == -1) //The command has a pipe, but it fails in creating it.
		return (ft_puterr("Fatal error.\n"));

	int	pid = fork();
	if (!pid)
	{
		av[i] = 0;
		if (has_pipe && (dup2(fd[1], 1) == -1 || close(fd[0]) == -1 || close(fd[1]) == -1)) //if there's a pipe in the command and setting it up fails, return an error.
			return ft_puterr("Fatal error.\n");
		if (!strcmp(*av, "cd")) //If there's no pipe and the command is cd, call the ft_cd function
			return ft_cd(av, i);
		execve(*av, av, envp); //Execute the command.
		return (ft_puterr("error: cannot execute "), ft_puterr(*av), ft_puterr("\n"));
	}
	//wait for the child process to finish
	waitpid(pid, &status, 0);
	if (has_pipe && (dup2(fd[0], 0) == -1 || close(fd[0]) == -1 || close(fd[1]) == -1)) //if there's a pipe in the command and setting it up fails, return an error.
		return ft_puterr("Fatal error.\n");
	return WIFEXITED(status) && WEXITSTATUS(status); //returns the exit status of the child process
}

int	main(int ac, char **av, char **envp)
{
	int	i = 0;
	int	status = 0;

	if (ac > 1)
	{
		while (av[i] && av[++i])
		{
			av += i;
			i = 0;
			while (av[i] && strcmp(av[i], "|") && strcmp(av[i], ";"))
				i++;
			if (i)
				status = ft_exec(av, i, envp);
		}
	}
	return (status);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzhang2 <lzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 17:19:46 by lzhang2           #+#    #+#             */
/*   Updated: 2024/10/14 15:37:08 by lzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	init_struct_prog(t_prog *prog)
{
	prog->infile = -1;
	prog->outfile = -1;
	prog->pipe_fd[0] = -1;
	prog->pipe_fd[1] = -1;
	prog->pid1 = -1;
	prog->pid2 = -1;
	prog->exec.i = 0;
	prog->is_1st_cmd = false;
	prog->exec.path_env = NULL;
	prog->exec.paths = NULL;
	prog->exec.full_path = NULL;
	prog->exec.access_ok = NULL;
}

void	ft_check_param(t_prog *prog, int argc)
{
	if (argc < 5)
	{
		ft_putstr_fd("argc should be 5\n", 2);
		ft_putstr_fd("Ex: ./pipex <file1> <cmd1> <cmd2> <file2>", 2);
		ft_close(prog);
		exit(EXIT_FAILURE);
	}
}

void	ft_child_1(t_prog *prog, char **argv, char **envp)
{
	prog->pid1 = fork();
	if (prog->pid1 == 0)
	{
		check_infile(prog, argv[1]);
		if ((dup2(prog->infile, STDIN_FILENO) == -1)
			|| (dup2(prog->pipe_fd[1], STDOUT_FILENO) == -1))
		{
			ft_putstr_fd("Failed to duplicate infile to stdin", 2);
			ft_close(prog);
			exit(EXIT_FAILURE);
		}
		ft_close(prog);
		prog->is_1st_cmd = true;
		execute_command(prog, argv[2], envp);
	}
}

void	ft_child_2(t_prog *prog, char **argv, char **envp)
{
	prog->pid2 = fork();
	if (prog->pid2 == 0)
	{
		check_outfile(prog, argv[4]);
		if ((dup2(prog->pipe_fd[0], STDIN_FILENO) == -1)
			|| dup2(prog->outfile, STDOUT_FILENO) == -1)
		{
			ft_putstr_fd("Failed to duplicate infile to stdout", 2);
			ft_close(prog);
			exit(EXIT_FAILURE);
		}
		ft_close(prog);
		prog->is_1st_cmd = false;
		execute_command(prog, argv[3], envp);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_prog	prog;
	int		exit_status;

	init_struct_prog(&prog);
	ft_check_param(&prog, argc);
	if (pipe(prog.pipe_fd) == -1)
	{
		ft_putstr_fd("pipe failed", 2);
		ft_close(&prog);
		return (1);
	}
	ft_child_1(&prog, argv, envp);
	ft_child_2(&prog, argv, envp);
	ft_close(&prog);
	exit_status = wait_for_children(prog.pid1, prog.pid2);
	return (exit_status);
}

/*strace -e signal=SIGPIPE -o trace.log < input /usr/bin/cat
| /usr/bin/wcgd > output
*/
/*valgrind --leak-check=full --show-leak-kinds=all 
--trace-children=yes ./pipex input /usr/bin/cat dls output
*/
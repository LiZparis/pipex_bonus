/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzhang2 <lzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 15:14:34 by lzhang2           #+#    #+#             */
/*   Updated: 2024/10/14 14:11:26 by lzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	ft_free_split(char **split)
{
	int	i;

	i = 0;
	if (!split)
		return ;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
}

void	ft_close(t_prog *prog)
{
	if (prog->infile != -1)
		close(prog->infile);
	if (prog->outfile != -1)
		close(prog->outfile);
	if (prog->pipe_fd[0] != -1)
		close(prog->pipe_fd[0]);
	if (prog->pipe_fd[1] != -1)
		close(prog->pipe_fd[1]);
}

void	is_last_cmd(t_prog *prog)
{
	if (prog->is_last_cmd == false)
	{
		ft_close(prog);
		exit(0);
	}
	else if (prog->is_last_cmd == true)
	{
		ft_close(prog);
		exit(127);
	}
}
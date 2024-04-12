/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabarnou <gabarnou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 11:02:05 by gabarnou          #+#    #+#             */
/*   Updated: 2024/04/12 18:37:27 by gabarnou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	**path_extraction(char **envp)
{
	char	**paths;
	int		i;

	paths = NULL;
	i = 0;
	while (envp[i])
	{
		if (ft_strncmp("PATH=", envp[i], 5) == 0)
		{
			paths = ft_split(envp[i] + 5, ':');
		}
		i++;
	}
	if (!paths)
		paths = ft_split(". ", ' ');
	return (paths);
}

char	*ft_strjoin_triple(char const *s1, char const *s2, char const *s3)
{
	char	*result;
	size_t	s1_len;
	size_t	s2_len;
	size_t	s3_len;

	if (!s1 || !s2 || !s3)
		return (NULL);
	s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	s3_len = ft_strlen(s3);
	result = (char *)malloc(s1_len + s2_len + s3_len + 1);
	if (!result)
		return (NULL);
	ft_strlcpy(result, s1, s1_len + 1);
	ft_strlcat(result, s2, s1_len + s2_len + 1);
	ft_strlcat(result, s3, s1_len + s2_len + s3_len + 1);
	return (result);
}

void	ft_execve(t_pipex *pipex)
{
	char	*cmd_path;
	int		i;

	cmd_path = 0;
	i = 0;
	while (pipex->paths[i])
	{
		if (pipex->child_args[0][0] == '.' || pipex->child_args[0][0] == '/')
			execve(pipex->child_args[0], pipex->child_args, pipex->envp);
		cmd_path = ft_strjoin_triple(pipex->paths[i],
				"/", pipex->child_args[0]);
		if (!cmd_path)
		{
			send_error_msg("failed to build cmd_path\n");
			free_tab(pipex->paths);
			exit(EXIT_FAILURE);
		}
		execve(cmd_path, pipex->child_args, pipex->envp);
		i++;
		free(cmd_path);
	}
}

void	send_error_msg(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		write(STDERR_FILENO, &str[i], 1);
		i++;
	}
}

int	is_limiter(const char *s1,const char *s2)
{
	/*
	
	Pour trouver le delimiteur, mettre le \0 a \n pour comparer et trouver la string
	parametres = buff de gnl et cmds[2] de pipex
	questions = comment faire pour scanner chaque ligne
		reponses potentiels = integrer la verification a la boucle
	
	*/
	int i;
	unsigned char *u1;
	unsigned char *u2;

	i = ft_strlen(s2);
	u2 = s2;
	u2[i] = '\n';

	while (s1 || s2)
	{
		u1 = (unsigned char) s1;
		u2 = (unsigned char) s2;
		if (u1 != u2 || u1 == '\0' || u2 == '\0')
		{
			return ((int)(u1 - u2));
		}
		s1++;
		s2++;
	}
	return (0);
}

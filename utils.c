/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabarnou <gabarnou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 11:02:05 by gabarnou          #+#    #+#             */
/*   Updated: 2024/04/05 14:00:05 by gabarnou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char **path_extraction(char **envp)
{
	char	**paths;
	int		i;

	paths = NULL;
	i = 0;
	while(envp[i])
	{
		if (ft_strncmp("PATH=", envp[i], 5) == 0)
		{
			paths = ft_split(envp[i] + 5, ':');
			if(!paths)
				perror("pipex failed to extract paths for envp\n"),
					exit(EXIT_FAILURE);
		}
		i++;
	}
	if(!paths)
	{
		paths = ft_split(". ", ' ');
		if(!paths)
			perror("pipex failed to extract paths for envp\n"),
				exit(EXIT_FAILURE);
	}
	return(paths);
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
		cmd_path = ft_strjoin_triple(pipex->paths[i], "/", pipex->child_args[0]);
		if (!cmd_path)
		{
			perror("failed to build cmd_path");
			free_tab(pipex->paths);
			exit(EXIT_FAILURE);
		}
		execve(cmd_path, pipex->child_args, pipex->envp);
		i++;
		free(cmd_path);
	}
}

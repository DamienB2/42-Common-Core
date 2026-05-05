/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaise <dbaise@student.42belgium.be>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/29 14:17:05 by dbaise            #+#    #+#             */
/*   Updated: 2026/05/05 19:03:21 by dbaise           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*ft_strchr(const char *s, int c)
{
	while (1)
	{
		if (*s == (char)c)
			return ((char *) s);
		if (*s == '\0')
			break ;
		s++;
	}
	return (NULL);
}

char	*add_to_stash(char *buf, char *stash)
{
	char	*temp_stash;

	temp_stash = stash;
	stash = ft_strjoin(stash, buf);
	free(temp_stash);
	return (stash);
}

char	*read_file_and_stash(int fd, char *stash)
{
	char	*buf;
	ssize_t	nb;

	buf = malloc(sizeof(char) * BUFFER_SIZE + 1);
	if (!buf)
		return (NULL);
	nb = 1;
	if (!stash)
		stash = ft_strdup("");
	while ((!stash || !ft_strchr(stash, '\n')) && nb != 0)
	{
		nb = read(fd, buf, BUFFER_SIZE);
		if (nb == -1)
		{
			free(buf);
			free(stash);
			return (NULL);
		}
		buf[nb] = '\0';
		if (nb == 0)
			break ;
		stash = add_to_stash(buf, stash);
	}
	free(buf);
	return (stash);
}

char	*create_line(char *stash, char **stash_rest)
{
	char	*line;
	char	*temp;
	int		i;

	i = 0;
	if (!stash)
		return (NULL);
	while (stash[i] && stash[i] != '\n')
		i++;
	if (stash[i] == '\n')
	{
		line = ft_substr(stash, 0, i + 1);
		temp = ft_strdup(&stash[i + 1]);
		free(*stash_rest);
		*stash_rest = temp;
	}
	else
	{
		line = ft_strdup(stash);
		free(*stash_rest);
		*stash_rest = NULL;
	}
	return (line);
}

char	*get_next_line(int fd)
{
	char		*line;
	static char	*stash;
	char		*temp_stash;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	temp_stash = stash;
	stash = read_file_and_stash(fd, stash);
	if (!stash)
	{
		free(temp_stash);
		return (NULL);
	}
	if (ft_strlen(stash) == 0)
	{
		free(stash);
		stash = NULL;
		return (NULL);
	}
	line = create_line(stash, &stash);
	return (line);
}

#include <fcntl.h>
#include <stdio.h>

int	main(void)
{
	int		fd;
	char	*line;

	fd = open("file.txt", O_RDONLY);
	if (fd == -1)
		return (1);

	while ((line = get_next_line(fd)) != NULL)
	{
		printf("%s", line);
		free(line);
	}
	close(fd);
	return (0);
}
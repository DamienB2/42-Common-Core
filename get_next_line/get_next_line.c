/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaise <dbaise@student.42belgium.be>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/29 14:17:05 by dbaise            #+#    #+#             */
/*   Updated: 2026/05/04 15:13:28 by dbaise           ###   ########.fr       */
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
	if (!stash)
		stash = "";
	stash = ft_strjoin(stash, buf);
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
	while ((!stash || !ft_strchr(stash, '\n')) && nb != 0)
	{
		nb = read(fd, buf, BUFFER_SIZE);
		if (nb == -1)
		{
			free(buf);
			return (NULL);
		}
		buf[nb] = '\0';
		if (nb == 0)
			break ;
		stash = add_to_stash(buf, stash);
	}
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

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	stash = read_file_and_stash(fd, stash);
	if (!stash)
		return (NULL);
	line = create_line(stash, &stash);
	return (line);
}

#include <fcntl.h>
#include <stdio.h>

int main(void)
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

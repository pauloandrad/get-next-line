/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pahenriq <pahenriq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/05 12:29:21 by pahenriq          #+#    #+#             */
/*   Updated: 2026/08/02 13:51:49 by pahenriq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <stdio.h>
#include <unistd.h>

int	read_next_chunk(int fd, char *buffer, char delimiter, char **line)
{
	int	bytes_read;
	int	size;
	int	line_len;
	int	chunk_line_len;

	line_len = line_size(*line, '\0');
	bytes_read = read(fd, buffer, BUFFER_SIZE);
	if (bytes_read <= 0 && line_len > 0)
		return (line_len + 1);
	else if (bytes_read <= 0 && !buffer[0])
		return (-1);
	if (bytes_read > 0)
		buffer[bytes_read] = '\0';
	chunk_line_len = line_size(buffer, delimiter);
	if (buffer[chunk_line_len] == delimiter)
		chunk_line_len++;
	size = line_len + chunk_line_len + 1;
	*line = realloc_line(*line, size);
	if (!*line)
		return (-1);
	ft_strlcat(*line, buffer, size);
	return (size);
}

int	buffer_init_line(int fd, char *buffer, char delimiter, char **line)
{
	int	line_len;

	line_len = line_size(buffer, delimiter);
	if (buffer[line_len] == delimiter)
	{
		ft_strlcat(*line, buffer, line_len + 2);
		return (line_len + 1);
	}
	if (line_len > 0)
		ft_strlcat(*line, buffer, BUFFER_SIZE + 1);
	line_len = read_next_chunk(fd, buffer, delimiter, line);
	return (line_len);
}

void	prepare_next_call(char *buffer, int len, char delimiter)
{
	if (buffer[len] == delimiter)
		len++;
	ft_strlcpy(buffer, &buffer[len], BUFFER_SIZE + 1);
}

int	next_chunk_line(int fd, char *buffer, char delimiter, char **line)
{
	int	buff_line_len;
	int	bytes_read;
	int	size;

	buff_line_len = 0;
	bytes_read = read(fd, buffer, BUFFER_SIZE);
	if (bytes_read > 0)
	{
		buffer[bytes_read] = '\0';
		buff_line_len = line_size(buffer, delimiter);
		size = buff_line_len + line_size(*line, '\0');
		if (buffer[buff_line_len] == delimiter)
			size++;
		else if (buffer[buff_line_len] == '\0')
			size = bytes_read + line_size(*line, '\0');
		size++;
		*line = realloc_line(*line, size);
		if (!*line)
			return (-1);
		ft_strlcat(*line, buffer, size);
		if (buffer[buff_line_len] == '\0')
			return (next_chunk_line(fd, buffer, delimiter, line));
	}
	return (buff_line_len);
}

char	*get_next_line(int fd)
{
	static char	buffer[BUFFER_SIZE + 1];
	char		delimiter;
	char		*line;
	int			len;

	delimiter = '\n';
	if (fd < 0)
		return (NULL);
	line = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!line)
		return (NULL);
	line[0] = '\0';
	len = buffer_init_line(fd, buffer, delimiter, &line);
	if (len <= 0)
		return (free(line), NULL);
	if (line[line_size(line, delimiter)] == '\0' && buffer[0])
	{
		len = next_chunk_line(fd, buffer, delimiter, &line);
		if (len < 0)
			return (free(line), NULL);
	}
	len = line_size(buffer, delimiter);
	prepare_next_call(buffer, len, delimiter);
	return (line);
}

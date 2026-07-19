/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pahenriq <pahenriq@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/05 12:29:21 by pahenriq          #+#    #+#             */
/*   Updated: 2026/07/19 00:21:54 by pahenriq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>


char *realloc_line(char *line, size_t size)
{
	char *temp;

	temp = malloc(size);
	if (!temp)
		return NULL;
	ft_strlcpy(temp, line, size);
	free(line);
	line = temp;
	return line;
}

int line_len(char *line) {
    int		i;

    i = 0;
    while (line[i] != '\n' && line[i])
        i++;
    return i;
}

int buffer_init_line(int fd, char *buffer, char **line)
{
	int		bytes_read;
	int		size;
	int		len;

	len = line_len(buffer);
	if (len > 0)
		ft_strlcat(*line, buffer, BUFFER_SIZE + 1);
	bytes_read = read(fd, buffer, BUFFER_SIZE);
	if (bytes_read <= 0)
		return (-1);
	buffer[bytes_read] = '\0';
	size = len + bytes_read + 1;
	*line = realloc_line(*line, size);	
	if (!*line)
		return (-1);
	ft_strlcat(*line, buffer, size);
	return (size);
}

int next_chunk_line(int fd, char *buffer, char **line, int len)
{
	int		buff_line_len;
	int		bytes_read;
	int		size;

	buff_line_len = 0;
	bytes_read = read(fd, buffer, BUFFER_SIZE);
	if (bytes_read > 0)
	{
		buffer[bytes_read] = '\0';
		buff_line_len = line_len(buffer);
		size = buff_line_len + len;
		if (buffer[buff_line_len] == '\n')
			size++;
		*line = realloc_line(*line, size);
		if (!*line)
			return (-1);
		ft_strlcat(*line, buffer, size);
		if (buffer[buff_line_len] == '\0')
			return next_chunk_line(fd, buffer, line, size);
	}
	return (buff_line_len);
}

char *get_next_line(int fd) {
	static char	buffer[BUFFER_SIZE + 1];
	char	*line;
	int		len;

	line = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!line)
		return NULL;
	line[0] = '\0';
	len = buffer_init_line(fd, buffer, &line);
	if (len <= 0)
		return (free(line), NULL);
	if (line[len] == '\0')
	{
		len = next_chunk_line(fd, buffer, &line, len);
		if (len < 0)
			return (free(line), NULL);
	}
	if (buffer[len] == '\n')
		len++;
	ft_strlcpy(buffer, &buffer[len], BUFFER_SIZE + 1);
	return line;
}

int main(void) {
    char *line;
    int fd;

	fd = open("file.txt", O_RDONLY);
	if (fd == -1)
	{
		printf("Erro ao abrir arquivo\n");
		return (1);
	}

    while((line = get_next_line(fd)) != NULL)
    {
        printf("%s", line);
        free(line);
    }

	close(fd);
    return (0);
}

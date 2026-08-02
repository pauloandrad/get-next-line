#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 42

// Copy actual ft_strlcpy from utils
size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	size_t	i;
	size_t	src_len;

	src_len = 0;
	while (src[src_len])
		src_len++;
	if (dstsize == 0)
		return (src_len);
	i = 0;
	while (src[i] && i < dstsize - 1)
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (src_len);
}

int line_size(char *line, char delimiter)
{
	int i = 0;
	while (line[i] != delimiter && line[i])
		i++;
	return i;
}

void show_buffer(char *buffer, const char *label)
{
	printf("%s: [", label);
	for (int i = 0; i < BUFFER_SIZE && buffer[i]; i++)
	{
		if (buffer[i] == '\n')
			printf("\\n");
		else if (buffer[i] >= 32 && buffer[i] < 127)
			printf("%c", buffer[i]);
		else
			printf("?");
	}
	printf("] (%d bytes)\n", (int)strlen(buffer));
}

int main(void)
{
	int fd = open("test_file.txt", O_RDONLY);
	if (fd < 0) return 1;

	char buffer[BUFFER_SIZE + 1];
	int bytes_read;

	printf("===== CALL 1 =====\n");
	bytes_read = read(fd, buffer, BUFFER_SIZE);
	buffer[bytes_read] = '\0';
	printf("After read(1): bytes_read=%d, file_pos=%ld\n", bytes_read, lseek(fd, 0, SEEK_CUR));
	show_buffer(buffer, "  buffer");

	int len = line_size(buffer, '\n');
	printf("  len=%d, buffer[%d]='%c' (0x%02x)\n", len, len, buffer[len] ?: '?', (unsigned char)buffer[len]);
	if (buffer[len] == '\n')
		len++;
	printf("  After delimiter check: len=%d\n", len);

	printf("  Calling ft_strlcpy(buffer, &buffer[%d], 43)\n", len);
	ft_strlcpy(buffer, &buffer[len], BUFFER_SIZE + 1);
	show_buffer(buffer, "  buffer after strlcpy");

	printf("\n===== CALL 2 =====\n");
	printf("File position before read: %ld\n", lseek(fd, 0, SEEK_CUR));
	bytes_read = read(fd, buffer, BUFFER_SIZE);
	buffer[bytes_read] = '\0';
	printf("After read(2): bytes_read=%d, file_pos=%ld\n", bytes_read, lseek(fd, 0, SEEK_CUR));
	show_buffer(buffer, "  buffer");

	len = line_size(buffer, '\n');
	printf("  len=%d, buffer[%d]='%c' (0x%02x)\n", len, len, buffer[len] ?: '?', (unsigned char)buffer[len]);
	if (buffer[len] == '\n')
		len++;
	printf("  After delimiter check: len=%d\n", len);

	printf("  Calling ft_strlcpy(buffer, &buffer[%d], 43)\n", len);
	ft_strlcpy(buffer, &buffer[len], BUFFER_SIZE + 1);
	show_buffer(buffer, "  buffer after strlcpy");

	printf("\n===== CALL 3 =====\n");
	printf("File position before read: %ld\n", lseek(fd, 0, SEEK_CUR));
	bytes_read = read(fd, buffer, BUFFER_SIZE);
	buffer[bytes_read] = '\0';
	printf("After read(3): bytes_read=%d, file_pos=%ld\n", bytes_read, lseek(fd, 0, SEEK_CUR));
	show_buffer(buffer, "  buffer");

	close(fd);
	return 0;
}

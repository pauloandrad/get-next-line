#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 42

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
	char line[500];
	int bytes_read;

	printf("===== CALL 1 =====\n");
	bytes_read = read(fd, buffer, BUFFER_SIZE);
	buffer[bytes_read] = '\0';
	printf("After read(1): bytes_read=%d\n", bytes_read);
	show_buffer(buffer, "  buffer");

	int chunk_line_len = line_size(buffer, '\n');
	printf("  chunk_line_len=%d, buffer[%d]='%c' (0x%02x)\n", chunk_line_len, chunk_line_len, buffer[chunk_line_len] ?: '?', (unsigned char)buffer[chunk_line_len]);
	if (buffer[chunk_line_len] == '\n')
		chunk_line_len++;
	printf("  After delimiter check: chunk_line_len=%d\n", chunk_line_len);

	printf("  Simulating prepare_next_call: ft_strlcpy(buffer, &buffer[%d], 43)\n", chunk_line_len);
	char temp[BUFFER_SIZE + 1];
	strncpy(temp, &buffer[chunk_line_len], BUFFER_SIZE);
	temp[BUFFER_SIZE] = '\0';
	strcpy(buffer, temp);
	show_buffer(buffer, "  buffer after prepare");

	printf("\n===== CALL 2 =====\n");
	printf("File position before read: %ld\n", lseek(fd, 0, SEEK_CUR));
	bytes_read = read(fd, buffer, BUFFER_SIZE);
	buffer[bytes_read] = '\0';
	printf("After read(2): bytes_read=%d\n", bytes_read);
	show_buffer(buffer, "  buffer");

	chunk_line_len = line_size(buffer, '\n');
	printf("  chunk_line_len=%d, buffer[%d]='%c' (0x%02x)\n", chunk_line_len, chunk_line_len, buffer[chunk_line_len] ?: '?', (unsigned char)buffer[chunk_line_len]);
	if (buffer[chunk_line_len] == '\n')
		chunk_line_len++;
	printf("  After delimiter check: chunk_line_len=%d\n", chunk_line_len);

	printf("  Simulating prepare_next_call: ft_strlcpy(buffer, &buffer[%d], 43)\n", chunk_line_len);
	strncpy(temp, &buffer[chunk_line_len], BUFFER_SIZE);
	temp[BUFFER_SIZE] = '\0';
	strcpy(buffer, temp);
	show_buffer(buffer, "  buffer after prepare");

	printf("\n===== CALL 3 =====\n");
	printf("File position before read: %ld\n", lseek(fd, 0, SEEK_CUR));
	bytes_read = read(fd, buffer, BUFFER_SIZE);
	buffer[bytes_read] = '\0';
	printf("After read(3): bytes_read=%d\n", bytes_read);
	show_buffer(buffer, "  buffer");

	close(fd);
	return 0;
}

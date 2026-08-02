#include "../get_next_line.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern char *get_next_line(int fd);

int main(void)
{
	int fd;
	char *line;
	int count = 0;

	fd = open("test_file.txt", O_RDONLY);
	if (fd < 0)
	{
		perror("open");
		return 1;
	}

	while ((line = get_next_line(fd)) != NULL && count < 5)
	{
		count++;
		int len = strlen(line);
		printf("\n=== Line %d (len=%d) ===\n", count, len);
		printf("Content: [");
		for (int i = 0; i < len; i++)
		{
			if (line[i] == '\n')
				printf("\\n");
			else
				printf("%c", line[i]);
		}
		printf("]\n");
		printf("Hex: ");
		for (int i = 0; i < len; i++)
			printf("%02x ", (unsigned char)line[i]);
		printf("\n");

		free(line);
	}

	close(fd);
	return 0;
}

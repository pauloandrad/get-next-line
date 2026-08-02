#include "../get_next_line.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

	while ((line = get_next_line(fd)) != NULL)
	{
		count++;
		printf("Line %d (%zu chars): [%s]\n", count, strlen(line), line);

		// Show hex of first 5 and last 5 chars
		printf("  First 5: ");
		for (int i = 0; i < 5 && line[i]; i++)
			printf("%02x ", (unsigned char)line[i]);
		printf("\n");

		printf("  Last 5: ");
		for (int i = strlen(line) - 5; i < strlen(line); i++)
			if (i >= 0)
				printf("%02x ", (unsigned char)line[i]);
		printf("\n\n");

		free(line);
	}

	close(fd);
	return 0;
}

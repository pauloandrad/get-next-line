#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Copy of GNL functions with debug output
#define BUFFER_SIZE 42

char *ft_strlcat_dbg(char *dst, const char *src, size_t dstsize, const char *caller)
{
	size_t	i, j, dst_len, src_len;

	src_len = 0;
	while (src[src_len])
		src_len++;
	if (dstsize == 0)
		return dst;
	dst_len = 0;
	while (dst[dst_len] && dst_len < dstsize)
		dst_len++;
	printf("  [%s] strlcat: dst_len=%zu, src_len=%zu, dstsize=%zu, can_copy=%zu\n",
		caller, dst_len, src_len, dstsize, (dst_len < dstsize) ? (dstsize - 1 - dst_len) : 0);

	i = dst_len;
	j = 0;
	while (src[j] != '\0' && i < dstsize - 1)
	{
		dst[i] = src[j];
		i++;
		j++;
	}
	dst[i] = '\0';
	return dst;
}

int main(void)
{
	int fd;
	fd = open("test_file.txt", O_RDONLY);
	if (fd < 0)
		return 1;

	char buffer[BUFFER_SIZE + 1];
	char line[500];

	printf("=== Simulating Read 1 ===\n");
	int bytes = read(fd, buffer, BUFFER_SIZE);
	buffer[bytes] = '\0';
	printf("Read %d bytes: ", bytes);
	for (int i = 0; i < bytes; i++)
		printf("%02x ", (unsigned char)buffer[i]);
	printf("\n");

	printf("\n=== Simulating Read 2 ===\n");
	bytes = read(fd, buffer, BUFFER_SIZE);
	buffer[bytes] = '\0';
	printf("Read %d bytes: ", bytes);
	for (int i = 0; i < bytes; i++)
		printf("%02x ", (unsigned char)buffer[i]);
	printf("\n");

	printf("\n=== Simulating Read 3 ===\n");
	line[0] = '\0';
	bytes = read(fd, buffer, BUFFER_SIZE);
	buffer[bytes] = '\0';
	printf("Read %d bytes: ", bytes);
	for (int i = 0; i < bytes; i++)
		printf("%02x ", (unsigned char)buffer[i]);
	printf("\n");

	printf("Calling ft_strlcat(line='', buffer, 43)\n");
	ft_strlcat_dbg(line, buffer, 43, "rd3");
	printf("Line now has %zu bytes\n", strlen(line));

	close(fd);
	return 0;
}

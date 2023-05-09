#include "main.h"
#include <stdlib.h>


/**
 * read_textfile - reads a text file
 * @filename: string
 * @letters: size_t
 *
 * Return: ssize_t
 */

ssize_t read_textfile(const char *filename, size_t letters)
{
	char *b;
	ssize_t op, rd, wr;

	if (filename == NULL)
		return (0);

	b = malloc(sizeof(char) * letters);

	if (b == NULL)
		return (0);

	op = open(filename, O_RDONLY);
	rd = read(op, b, letters);
	wr = write(STDOUT_FILENO, b, rd);

	if (op == -1 || rd == -1 || wr == -1  || wr != rd)
	{
		free(b);
		return (0);
	}

	free(b);
	close(op);

	return (wr);


}


#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

void display_usage_error(void);
void display_read_error(const char *filename);
void display_write_error(const char *filename);
void display_close_error(int fd);
void copy_file(const char *src_file, const char *dest_file);

/**
 * display_usage_error - Displays an error message for incorrect usage.
 */
void display_usage_error(void)
{
	dprintf(STDERR_FILENO, "Usage: cp file_from file_to\n");
	exit(97);
}

/**
 * display_read_error - Displays an error message for file read failure.
 * @filename: The file to be read.
 */
void display_read_error(const char *filename)
{
	dprintf(STDERR_FILENO, "Error: Can't read from file %s\n", filename);
	exit(98);
}

/**
 * display_write_error - Displays an error message for file write failure.
 * @filename: The file to be written.
 */
void display_write_error(const char *filename)
{
	dprintf(STDERR_FILENO, "Error: Can't write to %s\n", filename);
	exit(99);
}

/**
 * display_close_error - Displays an error message for file descriptor close.
 * @fd: The file descriptor.
 */
void display_close_error(int fd)
{
	dprintf(STDERR_FILENO, "Error: Can't close fd %d\n", fd);
	exit(100);
}

/**
 * copy_file - Copies the content of one file to another file.
 * @src_file: The source file.
 * @dest_file: The destination file.
 */
void copy_file(const char *src_file, const char *dest_file)
{
	int src_fd, dest_fd;
	ssize_t read_bytes, write_bytes;
	char buffer[1024];

	src_fd = open(src_file, O_RDONLY);

	if (src_fd == -1)
		display_read_error(src_file);

	dest_fd = open(dest_file, O_CREAT | O_WRONLY | O_TRUNC, 0664);

	if (dest_fd == -1)
	{
		close(src_fd);
		display_write_error(dest_file);
	}

	while ((read_bytes = read(src_fd, buffer, sizeof(buffer))) > 0)
	{
		write_bytes = write(dest_fd, buffer, read_bytes);
		if (write_bytes == -1 || write_bytes != read_bytes)
		{
			close(src_fd);
			close(dest_fd);
			display_write_error(dest_file);
		}
	}

	if (read_bytes == -1)
	{
		close(src_fd);
		close(dest_fd);
		display_read_error(src_file);
	}

	if (close(src_fd) == -1)
		display_close_error(src_fd);

	if (close(dest_fd) == -1)
		display_close_error(dest_fd);
}

/**
 * main - Entry point of the program.
 * @argc: Number of command-line arguments.
 * @argv: Array of pointers to the command-line arguments.
 * Return: 0 on success.
 */
int main(int argc, char *argv[])
{
	if (argc != 3)
		display_usage_error();

	copy_file(argv[1], argv[2]);

	return (0);
}

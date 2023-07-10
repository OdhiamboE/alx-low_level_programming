#include <unistd.h>
#include <fcntl.h>
#include <string.h>

/**
 * create_file - Creates a file and writes text content to it.
 * @filename: The name of the file to create.
 * @text_content: The text content to write to the file.
 *
 * Return: 1 on success, -1 on failure.
 */
int create_file(const char *filename, char *text_content)
{
	int file_descriptor, bytes_written;
	mode_t mode = S_IRUSR | S_IWUSR;

	if (!filename)
		return (-1);

	file_descriptor = open(filename, O_WRONLY | O_CREAT | O_TRUNC, mode);
	if (file_descriptor == -1)
		return (-1);

	if (text_content)
	{
		bytes_written = write(file_descriptor, text_content, strlen(text_content));
		if (bytes_written == -1)
		{
			close(file_descriptor);
			return (-1);
		}
	}

	close(file_descriptor);
	return (1);
}

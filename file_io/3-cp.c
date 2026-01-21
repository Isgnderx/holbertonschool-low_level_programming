#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define BUFFER_SIZE 1024

/**
 * close_fd - closes file descriptor with error checking
 * @fd: file descriptor to close
 *
 * Return: 0 on success, 1 on failure
 */
int close_fd(int fd)
{
	if (close(fd) == -1)
		return (1);
	return (0);
}

/**
 * handle_error - handles error cases
 * @code: error code
 * @msg: error message
 * @arg: argument for message
 */
void handle_error(int code, char *msg, char *arg)
{
	dprintf(STDERR_FILENO, msg, arg);
	exit(code);
}

/**
 * open_files - opens source and destination files
 * @argv: argument vector
 * @fd_from: pointer to store source fd
 * @fd_to: pointer to store destination fd
 */
void open_files(char *argv[], int *fd_from, int *fd_to)
{
	mode_t perm = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH;

	*fd_from = open(argv[1], O_RDONLY);
	if (*fd_from == -1)
		handle_error(98, "Error: Can't read from file %s\n", argv[1]);

	*fd_to = open(argv[2], O_CREAT | O_WRONLY | O_TRUNC, perm);
	if (*fd_to == -1)
	{
		close_fd(*fd_from);
		handle_error(99, "Error: Can't write to %s\n", argv[2]);
	}
}

/**
 * copy_content - copies content from source to destination
 * @fd_from: source file descriptor
 * @fd_to: destination file descriptor
 * @argv: argument vector
 */
void copy_content(int fd_from, int fd_to, char *argv[])
{
	char buffer[BUFFER_SIZE];
	int read_bytes, write_bytes;

	while ((read_bytes = read(fd_from, buffer, BUFFER_SIZE)) > 0)
	{
		write_bytes = write(fd_to, buffer, read_bytes);
		if (write_bytes == -1 || write_bytes != read_bytes)
		{
			close_fd(fd_from);
			close_fd(fd_to);
			handle_error(99, "Error: Can't write to %s\n", argv[2]);
		}
	}

	if (read_bytes == -1)
	{
		close_fd(fd_from);
		close_fd(fd_to);
		handle_error(98, "Error: Can't read from file %s\n", argv[1]);
	}
}

/**
 * main - copies content of a file to another file
 * @argc: argument count
 * @argv: argument vector
 *
 * Return: 0 on success, error code on failure
 */
int main(int argc, char *argv[])
{
	int fd_from, fd_to;

	if (argc != 3)
		handle_error(97, "Usage: cp file_from file_to\n", NULL);

	open_files(argv, &fd_from, &fd_to);
	copy_content(fd_from, fd_to, argv);

	if (close_fd(fd_from))
	{
		close_fd(fd_to);
		dprintf(STDERR_FILENO, "Error: Can't close fd %d\n", fd_from);
		exit(100);
	}
	if (close_fd(fd_to))
	{
		dprintf(STDERR_FILENO, "Error: Can't close fd %d\n", fd_to);
		exit(100);
	}

	return (0);
}

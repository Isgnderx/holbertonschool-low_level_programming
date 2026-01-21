#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "main.h"

/**
 * cleanup_resources - frees buffer and closes file descriptor
 * @buffer: pointer to allocated memory
 * @fd: file descriptor to close
 */
void cleanup_resources(char *buffer, int fd)
{
	if (buffer != NULL)
		free(buffer);
	if (fd != -1)
		close(fd);
}

/**
 * read_file - reads from file descriptor into buffer
 * @fd: file descriptor
 * @buffer: buffer to store data
 * @letters: number of letters to read
 *
 * Return: number of bytes read, -1 on error
 */
ssize_t read_file(int fd, char *buffer, size_t letters)
{
	ssize_t n_read;

	n_read = read(fd, buffer, letters);
	return (n_read);
}

/**
 * write_to_stdout - writes buffer to standard output
 * @buffer: buffer containing data
 * @n_read: number of bytes to write
 *
 * Return: number of bytes written, -1 on error
 */
ssize_t write_to_stdout(char *buffer, ssize_t n_read)
{
	ssize_t n_written;

	n_written = write(STDOUT_FILENO, buffer, n_read);
	return (n_written);
}

/**
 * read_textfile - reads a text file and prints to POSIX standard output
 * @filename: name of the file to read
 * @letters: number of letters it should read and print
 *
 * Return: actual number of letters read and printed, 0 on failure
 */
ssize_t read_textfile(const char *filename, size_t letters)
{
	int fd;
	ssize_t n_read, n_written;
	char *buffer;

	if (filename == NULL)
		return (0);

	buffer = malloc(sizeof(char) * letters);
	if (buffer == NULL)
		return (0);

	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		cleanup_resources(buffer, -1);
		return (0);
	}

	n_read = read_file(fd, buffer, letters);
	if (n_read == -1)
	{
		cleanup_resources(buffer, fd);
		return (0);
	}

	n_written = write_to_stdout(buffer, n_read);
	if (n_written == -1 || n_written != n_read)
	{
		cleanup_resources(buffer, fd);
		return (0);
	}

	cleanup_resources(buffer, fd);
	return (n_written);
}

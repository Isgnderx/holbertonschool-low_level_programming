#include <elf.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * print_error - Prints error message and exits with code 98
 * @msg: Error message
 * @file: File name
 */
static void print_error(const char *msg, const char *file)
{
	if (file)
		dprintf(STDERR_FILENO, "Error: %s %s\n", msg, file);
	else
		dprintf(STDERR_FILENO, "Error: %s\n", msg);
	exit(98);
}

/**
 * print_magic - Prints ELF magic bytes
 * @e_ident: ELF identification bytes
 */
static void print_magic(unsigned char *e_ident)
{
	int i;

	printf("  Magic:   ");
	for (i = 0; i < EI_NIDENT; i++)
	{
		printf("%02x", e_ident[i]);
		if (i != EI_NIDENT - 1)
			printf(" ");
	}
	printf("\n");
}

/**
 * print_class - Prints ELF class
 * @e_ident: ELF identification bytes
 */
static void print_class(unsigned char *e_ident)
{
	printf("  Class:                             ");
	if (e_ident[EI_CLASS] == ELFCLASS32)
		printf("ELF32\n");
	else if (e_ident[EI_CLASS] == ELFCLASS64)
		printf("ELF64\n");
	else
		printf("<unknown: %x>\n", e_ident[EI_CLASS]);
}

/**
 * print_data - Prints ELF data encoding
 * @e_ident: ELF identification bytes
 */
static void print_data(unsigned char *e_ident)
{
	printf("  Data:                              ");
	if (e_ident[EI_DATA] == ELFDATA2LSB)
		printf("2's complement, little endian\n");
	else if (e_ident[EI_DATA] == ELFDATA2MSB)
		printf("2's complement, big endian\n");
	else
		printf("<unknown: %x>\n", e_ident[EI_DATA]);
}

/**
 * print_version - Prints ELF version
 * @e_ident: ELF identification bytes
 */
static void print_version(unsigned char *e_ident)
{
	printf("  Version:                           ");
	if (e_ident[EI_VERSION] == EV_CURRENT)
		printf("%d (current)\n", e_ident[EI_VERSION]);
	else
		printf("%d\n", e_ident[EI_VERSION]);
}

/**
 * print_osabi - Prints OS/ABI
 * @e_ident: ELF identification bytes
 */
static void print_osabi(unsigned char *e_ident)
{
	printf("  OS/ABI:                            ");
	switch (e_ident[EI_OSABI])
	{
	case ELFOSABI_SYSV:
		printf("UNIX - System V\n");
		break;
	case ELFOSABI_NETBSD:
		printf("UNIX - NetBSD\n");
		break;
	case ELFOSABI_SOLARIS:
		printf("UNIX - Solaris\n");
		break;
	default:
		printf("<unknown: %x>\n", e_ident[EI_OSABI]);
	}
}

/**
 * print_abiversion - Prints ABI version
 * @e_ident: ELF identification bytes
 */
static void print_abiversion(unsigned char *e_ident)
{
	printf("  ABI Version:                       %d\n",
		e_ident[EI_ABIVERSION]);
}

/**
 * swap16 - Swap endianness of 16-bit value
 * @x: Value
 * Return: Swapped value
 */
static unsigned int swap16(unsigned int x)
{
	return ((x & 0x00ff) << 8) | ((x & 0xff00) >> 8);
}

/**
 * swap32 - Swap endianness of 32-bit value
 * @x: Value
 * Return: Swapped value
 */
static unsigned int swap32(unsigned int x)
{
	return ((x & 0x000000ff) << 24) |
		((x & 0x0000ff00) << 8) |
		((x & 0x00ff0000) >> 8) |
		((x & 0xff000000) >> 24);
}

/**
 * print_type - Prints ELF file type
 * @type: ELF type
 * @is_be: Big endian flag
 */
static void print_type(unsigned int type, int is_be)
{
	printf("  Type:                              ");
	if (is_be)
		type = swap16(type);

	if (type == ET_EXEC)
		printf("EXEC (Executable file)\n");
	else if (type == ET_DYN)
		printf("DYN (Shared object file)\n");
	else
		printf("<unknown: %x>\n", type);
}

/**
 * print_entry - Prints entry point address
 * @buf: ELF header buffer
 * @e_ident: ELF identification bytes
 */
static void print_entry(unsigned char *buf, unsigned char *e_ident)
{
	unsigned long entry;
	int is_be;

	is_be = (e_ident[EI_DATA] == ELFDATA2MSB);
	printf("  Entry point address:               ");

	if (e_ident[EI_CLASS] == ELFCLASS32)
	{
		entry = ((Elf32_Ehdr *)buf)->e_entry;
		if (is_be)
			entry = swap32(entry);
		printf("0x%lx\n", entry);
		return;
	}

	entry = (unsigned long)((Elf64_Ehdr *)buf)->e_entry;
	printf("0x%lx\n", entry);
}

/**
 * main - Displays ELF header information
 * @argc: Argument count
 * @argv: Argument vector
 * Return: 0 on success, exits 98 on error
 */
int main(int argc, char **argv)
{
	int fd;
	ssize_t r;
	unsigned char buf[64];
	unsigned char *id;

	if (argc != 2)
		print_error("Usage:", "elf_header elf_filename");

	fd = open(argv[1], O_RDONLY);
	if (fd == -1)
		print_error("Can't open file", argv[1]);

	r = read(fd, buf, sizeof(buf));
	if (r != (ssize_t)sizeof(buf))
		print_error("Can't read file", argv[1]);

	close(fd);
	id = buf;

	if (id[EI_MAG0] != ELFMAG0 || id[EI_MAG1] != ELFMAG1 ||
		id[EI_MAG2] != ELFMAG2 || id[EI_MAG3] != ELFMAG3)
		print_error("Not an ELF file", argv[1]);

	printf("ELF Header:\n");
	print_magic(id);
	print_class(id);
	print_data(id);
	print_version(id);
	print_osabi(id);
	print_abiversion(id);
	print_type(((Elf32_Ehdr *)buf)->e_type,
		id[EI_DATA] == ELFDATA2MSB);
	print_entry(buf, id);

	return (0);
}


#include <elf.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * print_error - Prints error message and exits with code 98
 * @msg: Error message
 * @file: File name (can be NULL)
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
	case ELFOSABI_HPUX:
		printf("UNIX - HP-UX\n");
		break;
	case ELFOSABI_NETBSD:
		printf("UNIX - NetBSD\n");
		break;
	case ELFOSABI_LINUX:
		printf("UNIX - Linux\n");
		break;
	case ELFOSABI_SOLARIS:
		printf("UNIX - Solaris\n");
		break;
	case ELFOSABI_IRIX:
		printf("UNIX - IRIX\n");
		break;
	case ELFOSABI_FREEBSD:
		printf("UNIX - FreeBSD\n");
		break;
	case ELFOSABI_TRU64:
		printf("UNIX - TRU64\n");
		break;
	case ELFOSABI_ARM:
		printf("ARM\n");
		break;
	case ELFOSABI_STANDALONE:
		printf("Standalone App\n");
		break;
	default:
		printf("<unknown: %x>\n", e_ident[EI_OSABI]);
		break;
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
 * swap16 - Swap endianness of a 16-bit value
 * @x: Value
 * Return: Swapped value
 */
static unsigned int swap16(unsigned int x)
{
	return (((x & 0x00ffU) << 8) | ((x & 0xff00U) >> 8));
}

/**
 * swap32 - Swap endianness of a 32-bit value
 * @x: Value
 * Return: Swapped value
 */
static unsigned int swap32(unsigned int x)
{
	return (((x & 0x000000ffU) << 24) |
		((x & 0x0000ff00U) << 8) |
		((x & 0x00ff0000U) >> 8) |
		((x & 0xff000000U) >> 24));
}

/**
 * type_str - Convert e_type to string like readelf
 * @t: Type value (host endian)
 * Return: Pointer to static string, or NULL if unknown
 */
static const char *type_str(unsigned int t)
{
	switch (t)
	{
	case ET_NONE:
		return ("NONE (None)");
	case ET_REL:
		return ("REL (Relocatable file)");
	case ET_EXEC:
		return ("EXEC (Executable file)");
	case ET_DYN:
		return ("DYN (Shared object file)");
	case ET_CORE:
		return ("CORE (Core file)");
	default:
		return (NULL);
	}
}

/**
 * print_type - Prints ELF file type
 * @type_raw: Raw e_type from header
 * @is_be: 1 if big-endian, 0 if little-endian
 */
static void print_type(unsigned int type_raw, int is_be)
{
	unsigned int t;
	const char *s;

	t = type_raw;
	if (is_be)
		t = swap16(t);

	printf("  Type:                              ");
	s = type_str(t);
	if (s)
		printf("%s\n", s);
	else
		printf("<unknown: %x>\n", t);
}

/**
 * swap64_ul - Swap 64-bit value using unsigned long (no long long)
 * @v: Value
 * Return: Swapped value
 */
static unsigned long swap64_ul(unsigned long v)
{
	unsigned int hi, lo;
	unsigned long res;

	hi = (unsigned int)((v >> 32) & 0xffffffffUL);
	lo = (unsigned int)(v & 0xffffffffUL);

	res = ((unsigned long)swap32(lo) << 32) | (unsigned long)swap32(hi);
	return (res);
}

/**
 * print_entry - Prints entry point address
 * @buf: ELF header buffer
 * @e_ident: ELF identification bytes
 */
static void print_entry(unsigned char *buf, unsigned char *e_ident)
{
	int is_be;

	is_be = (e_ident[EI_DATA] == ELFDATA2MSB);
	printf("  Entry point address:               ");

	if (e_ident[EI_CLASS] == ELFCLASS32)
	{
		Elf32_Addr e32;

		e32 = ((Elf32_Ehdr *)buf)->e_entry;
		if (is_be)
			e32 = (Elf32_Addr)swap32((unsigned int)e32);

		printf("0x%lx\n", (unsigned long)e32);
		return;
	}

	if (e_ident[EI_CLASS] == ELFCLASS64)
	{
		Elf64_Addr e64;
		unsigned long entry;

		e64 = ((Elf64_Ehdr *)buf)->e_entry;
		entry = (unsigned long)e64;

		if (is_be)
			entry = swap64_ul(entry);

		printf("0x%lx\n", entry);
		return;
	}

	printf("0x0\n");
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
	int is_be;

	if (argc != 2)
		print_error("Usage:", "elf_header elf_filename");

	fd = open(argv[1], O_RDONLY);
	if (fd == -1)
		print_error("Can't open file", argv[1]);

	r = read(fd, buf, sizeof(buf));
	if (r != (ssize_t)sizeof(buf))
	{
		close(fd);
		print_error("Can't read file", argv[1]);
	}
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

	is_be = (id[EI_DATA] == ELFDATA2MSB);
	print_type((unsigned int)((Elf32_Ehdr *)buf)->e_type, is_be);
	print_entry(buf, id);

	return (0);
}


#include <elf.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * print_error - print error message to stderr and exit 98
 * @msg: message
 * @file: filename (can be NULL)
 */
static void print_error(const char *msg, const char *file)
{
	if (file)
		dprintf(STDERR_FILENO, "Error: %s %s\n", msg, file);
	else
		dprintf(STDERR_FILENO, "Error: %s\n", msg);
	exit(98);
}

static void print_magic(const unsigned char *e_ident)
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

static void print_class(const unsigned char *e_ident)
{
	printf("  Class:                             ");
	switch (e_ident[EI_CLASS])
	{
	case ELFCLASS32:
		printf("ELF32\n");
		break;
	case ELFCLASS64:
		printf("ELF64\n");
		break;
	default:
		printf("<unknown: %x>\n", e_ident[EI_CLASS]);
		break;
	}
}

static void print_data(const unsigned char *e_ident)
{
	printf("  Data:                              ");
	switch (e_ident[EI_DATA])
	{
	case ELFDATA2LSB:
		printf("2's complement, little endian\n");
		break;
	case ELFDATA2MSB:
		printf("2's complement, big endian\n");
		break;
	default:
		printf("<unknown: %x>\n", e_ident[EI_DATA]);
		break;
	}
}

static void print_version(const unsigned char *e_ident)
{
	printf("  Version:                           ");
	if (e_ident[EI_VERSION] == EV_CURRENT)
		printf("%d (current)\n", e_ident[EI_VERSION]);
	else
		printf("%d\n", e_ident[EI_VERSION]);
}

static void print_osabi(const unsigned char *e_ident)
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
		/* readelf prints “<unknown: xx>” */
		printf("<unknown: %x>\n", e_ident[EI_OSABI]);
		break;
	}
}

static void print_abiversion(const unsigned char *e_ident)
{
	printf("  ABI Version:                       %d\n", e_ident[EI_ABIVERSION]);
}

static const char *type_to_str(unsigned int e_type)
{
	switch (e_type)
	{
	case ET_NONE:
		return "NONE (None)";
	case ET_REL:
		return "REL (Relocatable file)";
	case ET_EXEC:
		return "EXEC (Executable file)";
	case ET_DYN:
		return "DYN (Shared object file)";
	case ET_CORE:
		return "CORE (Core file)";
	default:
		return NULL;
	}
}

static void print_type(unsigned int e_type, int is_be)
{
	const char *s;

	/* if big endian, the 16-bit fields need swapping */
	if (is_be)
		e_type = ((e_type & 0x00ff) << 8) | ((e_type & 0xff00) >> 8);

	printf("  Type:                              ");
	s = type_to_str(e_type);
	if (s)
		printf("%s\n", s);
	else
		printf("<unknown: %x>\n", e_type);
}

static unsigned long long read_entry_elf32(const unsigned char *buf, int is_be)
{
	const Elf32_Ehdr *h = (const Elf32_Ehdr *)buf;
	Elf32_Addr e = h->e_entry;

	if (!is_be)
		return (unsigned long long)e;

	/* swap 32-bit */
	e = ((e & 0x000000ffU) << 24) |
	    ((e & 0x0000ff00U) << 8)  |
	    ((e & 0x00ff0000U) >> 8)  |
	    ((e & 0xff000000U) >> 24);
	return (unsigned long long)e;
}

static unsigned long long read_entry_elf64(const unsigned char *buf, int is_be)
{
	const Elf64_Ehdr *h = (const Elf64_Ehdr *)buf;
	Elf64_Addr e = h->e_entry;

	if (!is_be)
		return (unsigned long long)e;

	/* swap 64-bit */
	e = ((e & 0x00000000000000ffULL) << 56) |
	    ((e & 0x000000000000ff00ULL) << 40) |
	    ((e & 0x0000000000ff0000ULL) << 24) |
	    ((e & 0x00000000ff000000ULL) << 8)  |
	    ((e & 0x000000ff00000000ULL) >> 8)  |
	    ((e & 0x0000ff0000000000ULL) >> 24) |
	    ((e & 0x00ff000000000000ULL) >> 40) |
	    ((e & 0xff00000000000000ULL) >> 56);
	return (unsigned long long)e;
}

static void print_entry(const unsigned char *buf, const unsigned char *e_ident)
{
	int is_be = (e_ident[EI_DATA] == ELFDATA2MSB);
	unsigned long long entry;

	printf("  Entry point address:               ");

	if (e_ident[EI_CLASS] == ELFCLASS32)
	{
		entry = read_entry_elf32(buf, is_be);
		printf("0x%lx\n", (unsigned long)entry);
	}
	else if (e_ident[EI_CLASS] == ELFCLASS64)
	{
		entry = read_entry_elf64(buf, is_be);
		printf("0x%lx\n", (unsigned long)entry);
	}
	else
	{
		/* unknown class: best effort */
		printf("0x0\n");
	}
}

int main(int argc, char **argv)
{
	int fd;
	ssize_t r;
	unsigned char buf[64];
	unsigned char *id;
	int is_be;
	unsigned int e_type_raw;

	if (argc != 2)
		print_error("Usage: elf_header elf_filename", NULL);

	fd = open(argv[1], O_RDONLY);
	if (fd == -1)
		print_error("Can't open file", argv[1]);

	/* One read only: grab max ELF header size */
	r = read(fd, buf, sizeof(buf));
	if (r != (ssize_t)sizeof(buf))
	{
		close(fd);
		print_error("Can't read from file", argv[1]);
	}

	close(fd);

	id = buf;

	/* validate ELF magic */
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

	/* e_type is at same offset for Elf32_Ehdr and Elf64_Ehdr (16) */
	e_type_raw = ((const Elf32_Ehdr *)buf)->e_type;
	print_type(e_type_raw, is_be);

	print_entry(buf, id);

	return (0);
}


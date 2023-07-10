#include <elf.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

void validate_elf(unsigned char *ident);
void print_magic_numbers(unsigned char *ident);
void print_header_info(Elf64_Ehdr *header);

void close_file(int fd);

/**
 * validate_elf - Validates whether the file is an ELF file or not.
 * @ident: A pointer to an array containing the ELF magic numbers.
 *
 * Description: If the file is not an ELF file, exit with code 98.
 */
void validate_elf(unsigned char *ident)
{
	int i;

	for (i = 0; i < 4; i++)
	{
		if (ident[i] != 127 &&
		    ident[i] != 'E' &&
		    ident[i] != 'L' &&
		    ident[i] != 'F')
		{
			dprintf(STDERR_FILENO, "Error: Not an ELF file\n");
			exit(98);
		}
	}
}

/**
 * print_magic_numbers - Prints the magic numbers of an ELF header.
 * @ident: A pointer to an array containing the ELF magic numbers.
 *
 * Description: Magic numbers are separated by spaces.
 */
void print_magic_numbers(unsigned char *ident)
{
	int i;

	printf("  Magic:   ");

	for (i = 0; i < EI_NIDENT; i++)
	{
		printf("%02x", ident[i]);

		if (i == EI_NIDENT - 1)
			printf("\n");
		else
			printf(" ");
	}
}

/**
 * print_header_info - Prints the information contained in the ELF header.
 * @header: A pointer to the ELF header structure.
 */
void print_header_info(Elf64_Ehdr *header)
{
	printf("ELF Header:\n");
	print_magic_numbers(header->e_ident);

	printf("  Class:                             ");
	switch (header->e_ident[EI_CLASS])
	{
	case ELFCLASSNONE:
		printf("none\n");
		break;
	case ELFCLASS32:
		printf("ELF32\n");
		break;
	case ELFCLASS64:
		printf("ELF64\n");
		break;
	default:
		printf("<unknown: %x>\n", header->e_ident[EI_CLASS]);
	}

	printf("  Data:                              ");
	switch (header->e_ident[EI_DATA])
	{
	case ELFDATANONE:
		printf("none\n");
		break;
	case ELFDATA2LSB:
		printf("2's complement, little endian\n");
		break;
	case ELFDATA2MSB:
		printf("2's complement, big endian\n");
		break;
	default:
		printf("<unknown: %x>\n", header->e_ident[EI_DATA]);
	}

	printf("  Version:                           %d", header->e_ident[EI_VERSION]);
	switch (header->e_ident[EI_VERSION])
	{
	case EV_CURRENT:
		printf(" (current)\n");
		break;
	default:
		printf("\n");
		break;
	}

	printf("  OS/ABI:                            ");
	switch (header->e_ident[EI_OSABI])
	{
	case ELFOSABI_NONE:
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
		printf("<unknown: %x>\n", header->e_ident[EI_OSABI]);
	}

	printf("  ABI Version:                       %d\n", header->e_ident[EI_ABIVERSION]);

	if (header->e_ident[EI_DATA] == ELFDATA2MSB)
		header->e_type >>= 8;

	printf("  Type:                              ");
	switch (header->e_type)
	{
	case ET_NONE:
		printf("NONE (None)\n");
		break;
	case ET_REL:
		printf("REL (Relocatable file)\n");
		break;
	case ET_EXEC:
		printf("EXEC (Executable file)\n");
		break;
	case ET_DYN:
		printf("DYN (Shared object file)\n");
		break;
	case ET_CORE:
		printf("CORE (Core file)\n");
		break;
	default:
		printf("<unknown: %x>\n", header->e_type);
	}

	printf("  Entry point address:               ");
	if (header->e_ident[EI_DATA] == ELFDATA2MSB)
	{
		header->e_entry = ((header->e_entry << 8) & 0xFF00FF00) |
			((header->e_entry >> 8) & 0xFF00FF);
		header->e_entry = (header->e_entry << 16) | (header->e_entry >> 16);
	}

	if (header->e_ident[EI_CLASS] == ELFCLASS32)
		printf("%#x\n", (unsigned int)header->e_entry);
	else
		printf("%#lx\n", header->e_entry);
}

/**
 * close_file - Closes a file.
 * @fd: The file descriptor of the file to be closed.
 *
 * Description: If the file cannot be closed, exit with code 98.
 */
void close_file(int fd)
{
	if (close(fd) == -1)
	{
		dprintf(STDERR_FILENO, "Error: Can't close file descriptor %d\n", fd);
		exit(98);
	}
}

/**
 * main - Displays the information contained in the ELF header
 *        at the start of an ELF file.
 * @argc: The number of arguments supplied to the program.
 * @argv: An array of pointers to the arguments.
 *
 * Return: 0 on success.
 *
 * Description: If the file is not an ELF file or the function fails,
 *              exit with code 98.
 */
int main(int argc, char *argv[])
{
	int fd;
	Elf64_Ehdr header;
	ssize_t bytes_read;

	if (argc != 2)
	{
		dprintf(STDERR_FILENO, "Usage: %s <ELF-file>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	fd = open(argv[1], O_RDONLY);
	if (fd == -1)
	{
		perror("open");
		exit(EXIT_FAILURE);
	}

	bytes_read = read(fd, &header, sizeof(Elf64_Ehdr));
	if (bytes_read == -1)
	{
		perror("read");
		exit(EXIT_FAILURE);
	}

	validate_elf(header.e_ident);
	print_header_info(&header);

	close_file(fd);
	return (0);
}

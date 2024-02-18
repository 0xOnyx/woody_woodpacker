#include "utils.h"

off_t file_size(int fd)
{
	struct stat st;

	if (fstat(fd, &st) == -1)
	{
		fprintf(stderr, "Failed to get file size\n");
		return (0);
	}
	return (st.st_size);
}

int load_map(t_map_file *file, const char *filename, int option )
{
	int fd;
	void *data;
	size_t size;

	if ((fd = open(filename, option)) < 0)
	{
		fprintf(stderr, "Failed to open file %s\n", filename);
		return (1);
	}

	size = file_size(fd);

	if (!(data = (void *)mmap(
			NULL,
			size,
			PROT_READ | PROT_WRITE,
			MAP_SHARED,
			fd, 0)))
	{
		close(fd);
		fprintf(stderr, "Failed to map file\n");
	}
	file->fd = fd;
	file->size = size;
	file->data = data;
	return (0);
}

int unload_map(t_map_file *file){
	if (file->data && munmap(file->data, file->size) == -1)
	{
		fprintf(stderr, "Failed to unmap file\n");
		return (1);
	}
	close(file->fd);
	return (0);
}

int find_type(Elf64_Phdr *elf64Phdr, int start, int phnum, Elf64_Word type)
{
	for (int i = start; i < phnum; i++)
	{
		if (elf64Phdr[i].p_type == type)
			return (i);
	}
	return (-1);
}

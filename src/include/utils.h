#ifndef WOODY_WOODPACKER_UTILS_H
# define WOODY_WOODPACKER_UTILS_H
# include <stdio.h>
# include <fcntl.h>
# include <sys/mman.h>
# include <unistd.h>
# include <sys/stat.h>
# include <string.h>
# include <elf.h>
# include <stdlib.h>
# include <errno.h>

#define OUTPUT_FILE "WOODY"

#define IS_ELF64(p,s) (s > sizeof(Elf64_Ehdr) && !memcmp(ELFMAG, p, SELFMAG) && p[EI_CLASS] == ELFCLASS64)


#define ABORT(...) do{\
		fprintf(stderr, __VA_ARGS__);\
		if(errno) fprintf(stderr, " : %s", strerror(errno));\
		fprintf(stderr, "\nABORT!\n");\
		exit(0);\
	}while(0)

typedef struct
{
	int				fd;
	size_t 			size;
	unsigned char	*data;
} t_map_file;

off_t	file_size(int fd);
int		load_map(t_map_file *file, const char *filename, int option);
int 	unload_map(t_map_file *file);
int		inject_code(t_map_file *elf, t_map_file *patch);

#endif //WOODY_WOODPACKER_UTILS_H

#include "utils.h"
#define NAME_OUTPUT "woody"

static void clear_heap(t_map_file *elf, t_map_file *patch){
	if (elf->data != NULL)
		unload_map(elf);
	else if (patch->data != NULL)
		unload_map(patch);
}

static void copy_file(t_map_file *in, t_map_file *out){
	write(out->fd, in->data, in->size);
}

int	main(int argc, char **argv)
{
	t_map_file elf = {.data = NULL};
	t_map_file patch = {.data = NULL};

	if (argc < 2)
		ABORT("No input ELF file use ./woody_woodpacker <input_file>");

	if (load_map(&elf, argv[1], O_RDONLY, 0)
    	|| load_map(&patch, NAME_OUTPUT, O_RDWR, elf.size)){
		clear_heap(&elf, &patch);
		ABORT("Failed to load file");
	}

	if (!IS_ELF64(elf.data, elf.size)) {
		clear_heap(&elf, &patch);
		ABORT("Not a ELF 64 executable");
	}

	copy_file(&elf, &patch);
	inject_code(&patch);
	clear_heap(&elf, &patch);
	return (0);
}
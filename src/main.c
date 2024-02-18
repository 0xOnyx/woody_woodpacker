#include "utils.h"

void clear_heap(t_map_file *elf, t_map_file *patch){
	if (elf->data != NULL)
		unload_map(elf);
	else if (patch->data != NULL)
		unload_map(patch);
}

int	main(int argc, char **argv)
{
	t_map_file elf = {.data = NULL};
	t_map_file patch = {.data = NULL};

	if (argc < 2)
		ABORT("No input ELF file use ./woody_woodpacker <input_file>");

	if (load_map(&elf, argv[1], O_RDWR)) {
		clear_heap(&elf, &patch);
		ABORT("Failed to load file");
	}

	if (!IS_ELF64(elf.data, elf.size)) {
		clear_heap(&elf, &patch);
		ABORT("Not a ELF 64 executable");
	}

	inject_code(&elf, &patch);
	clear_heap(&elf, &patch);
	return (0);
}
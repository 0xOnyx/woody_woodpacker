#include "utils.h"

#define CODE_SIZE sizeof(CODE_STUB)
#define JMP_INDEX 51
#define INDEX_TEXT_SIZE 65
#define INDEX_TEXT 59


unsigned char CODE_STUB[] = {
        0x9c,0x50,0x57,0x56,0x54,0x52,0x51,0x41,0x50,0x41,0x51,
        0xe9,0xf2,0x00,0x00,0x00,0xbf,0x01,0x00,0x00,0x00,
        0x5e,0xba,0x0e,0x00,0x00,0x00,0xb8,0x01,0x00,0x00,
        0x00,0x0f,0x05,0xe9,0xee,0x00,0x00,0x00,0x41,0x59,
        0x41,0x58,0x59,0x5a,0x5c,0x5e,0x5f,0x58,0x9d,0xe9,
        0xfb,0xff,0xff,0xff,0x5e,0x48,0x8d,0x3d,0xf8,0xff,
        0xff,0xff,0x41,0xbb,0xff,0xff,0xff,0x00,0x55,0x48,
        0x89,0xe5,0x48,0x81,0xec,0x00,0x01,0x00,0x00,0x48,
        0x89,0xe0,0x48,0x31,0xc9,0x66,0x89,0x0c,0x08,0x48,
        0x83,0xc1,0x01,0x48,0x81,0xf9,0x00,0x01,0x00,0x00,
        0x7c,0xef,0x48,0x31,0xc9,0x48,0x31,0xd2,0x48,0x03,
        0x14,0x08,0x49,0x89,0xc8,0x49,0x83,0xe0,0x0f,0x4a,
        0x03,0x14,0x06,0x48,0x81,0xe2,0xff,0x00,0x00,0x00,
        0x44,0x8a,0x04,0x08,0x44,0x8a,0x0c,0x10,0x44,0x88,
        0x0c,0x08,0x44,0x88,0x04,0x10,0x48,0x83,0xc1,0x01,
        0x48,0x81,0xf9,0x00,0x01,0x00,0x00,0x7c,0xcd,0x48,
        0x31,0xc9,0x48,0x31,0xd2,0x4d,0x31,0xd2,0xe8,0x1a,
        0x00,0x00,0x00,0x46,0x8a,0x04,0x17,0x45,0x30,0xc1,
        0x46,0x88,0x0c,0x17,0x49,0x83,0xc2,0x01,0x4d,0x39,
        0xda,0x7c,0xe7,0xc9,0xe9,0x5f,0xff,0xff,0xff,0x48,
        0x83,0xc1,0x01,0x48,0x81,0xe1,0xff,0x00,0x00,0x00,
        0x48,0x03,0x14,0x08,0x48,0x81,0xe2,0xff,0x00,0x00,
        0x00,0x44,0x8a,0x04,0x08,0x44,0x8a,0x0c,0x10,0x44,
        0x88,0x0c,0x08,0x44,0x88,0x04,0x10,0x4c,0x8b,0x04,
        0x08,0x4c,0x03,0x04,0x10,0x49,0x81,0xe0,0xff,0x00,
        0x00,0x00,0x46,0x8a,0x0c,0x00,0xc3,0xe8,0x09,0xff,
        0xff,0xff,0x2e,0x2e,0x2e,0x2e,0x57,0x4f,0x4f,0x44,
        0x59,0x2e,0x2e,0x2e,0x2e,0x0a,0xe8,0x1d,0xff,0xff,
        0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00
};


Elf64_Phdr *find_cave(Elf64_Ehdr *ehdr, Elf64_Phdr *phdr)
{
	for(Elf64_Half i = 0; i < ehdr->e_phnum; i++)
	{
		if (phdr[i].p_type == PT_LOAD)
		{
			Elf64_Xword offset_last_data = phdr[i].p_offset + phdr[i].p_memsz;
			Elf64_Xword size = phdr[i+1].p_offset - offset_last_data;
			if (size > CODE_SIZE && phdr[i].p_flags & PF_X)
				return &phdr[i];
		}
	}
	return NULL;
}

static void insert_code(unsigned char *data, unsigned char key[KEY_SIZE],
     Elf64_Addr last_entry, Elf64_Addr new_entry, Elf64_Phdr *pt_load_x)
{
	int32_t	rel_entry;
	int32_t text_entry;
	uint32_t text_size;

	rel_entry =  last_entry - (new_entry + JMP_INDEX + sizeof(int32_t));
	printf("[x] new rel entry for jump => %x\n", rel_entry);
	memcpy(CODE_STUB + JMP_INDEX, &rel_entry, sizeof(int32_t));

	text_entry = pt_load_x->p_vaddr - (new_entry + INDEX_TEXT + sizeof(int32_t));
    text_size = pt_load_x->p_memsz;

	memcpy(CODE_STUB + INDEX_TEXT_SIZE, &text_size, sizeof(uint32_t));
    memcpy(CODE_STUB + INDEX_TEXT, &text_entry, sizeof(int32_t));
    memcpy(CODE_STUB + CODE_SIZE - KEY_SIZE, key, KEY_SIZE);

    memcpy(data, CODE_STUB, CODE_SIZE);
}

int inject_code(t_map_file *elf)
{
    unsigned char   key[KEY_SIZE];
	Elf64_Ehdr		*ehdr;
	Elf64_Phdr		*phdr;
	Elf64_Phdr 		*cave;

	ehdr = (Elf64_Ehdr *)elf->data;
	phdr = (Elf64_Phdr *)(elf->data + ehdr->e_phoff);

	if ((cave = find_cave(ehdr, phdr)) == NULL)
        ABORT("[x]\tNo found cave for inject\n");

	printf("[+]\tCave found with space => %lu\n", cave->p_memsz - CODE_SIZE);

    if (gen_key(key) < KEY_SIZE)
        ABORT("[x]\tNot able to generate key\n");


    encrypt_rc4(elf->data + cave->p_offset, cave->p_memsz, key);

	insert_code(elf->data + cave->p_offset + cave->p_memsz, key,
				ehdr->e_entry, cave->p_vaddr + cave->p_memsz,
                cave);

	ehdr->e_entry = cave->p_offset + cave->p_memsz;
	cave->p_memsz += CODE_SIZE;
	cave->p_filesz += CODE_SIZE;

	printf("[+]\tentry point => %lx\n", ehdr->e_entry);
    printf("[+]\tkey =>\n");
    for (size_t i = 0; i < KEY_SIZE; i++){
        printf("0x%02x, ", key[i]);
    }

    cave->p_flags |= PF_W;

    printf("\n[++++INJECT SUCESSFUL++++] \n");
	return (0);
}
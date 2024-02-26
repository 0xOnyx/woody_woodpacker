#include "utils.h"

#define CODE_SIZE sizeof(CODE_STUB)
#define JMP_INDEX 43
#define INDEX_CODE_SIZE
#define INDEX_TEXT



unsigned char CODE_STUB[] = {
        0x9c,0x50,0x57,0x56,0x54,0x52,0x51,0x41,0x50,0x41,0x51,0xeb,0x22,0xbf,0x01,0x00,0x00,0x00,0x5e,0xba,0x09,0x00,0x00,0x00,0xb8,0x01,0x00,0x00,0x00,0x0f,0x05,0x41,0x59,0x41,0x58,0x59,0x5a,0x5c,0x5e,0x5f,0x58,0x9d,0xe9,0xfb,0xff,0xff,0xff,0xe8,0xd9,0xff,0xff,0xff,0x5f,0x5f,0x57,0x4f,0x4f,0x44,0x59,0x5f,0x5f,0x00
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

void insert_code(unsigned char *data, unsigned char *key,
     Elf64_Addr last_entry, Elf64_Addr new_entry)
{
	int32_t	rel_entry;
	int32_t text_entry;
	uint32_t code_size;

	rel_entry =  last_entry - (new_entry + JMP_INDEX + sizeof(int32_t));
	printf("[x] new rel entry for jump => %x\n", rel_entry);
	memcpy(CODE_STUB + JMP_INDEX, &rel_entry, sizeof(int32_t));
    memcpy(CODE_STUB + CODE_SIZE - KEY_SIZE, &key, KEY_SIZE);

	text_entry =

	rel_text = get_uint64(((Elf64_Phdr *)file->text)->p_vaddr, file->endian)
			   - (entry_addr + payload->i_text + sizeof(int32_t));


	code_size = CODE_SIZE;

	memcpy(CODE_STUB + INDEX_CODE_SIZE, CODE_SIZE, sizeof(int32_t));
	memcpy()

	memcpy(data, CODE_STUB, CODE_SIZE);
}

int inject_code(t_map_file *elf, t_map_file *patch)
{
    unsigned char   key[KEY_SIZE];
	Elf64_Ehdr		*ehdr;
	Elf64_Phdr		*phdr;
	Elf64_Phdr 		*cave;
//	Elf64_Addr		jmp_adr;
	(void)patch;

	ehdr = (Elf64_Ehdr *)elf->data;
	phdr = (Elf64_Phdr *)(elf->data + ehdr->e_phoff);

	if ((cave = find_cave(ehdr, phdr)) == NULL)
        ABORT("[x]\tNo found cave for inject\n");

	printf("[+]\tCave found with space => %lu\n", cave->p_memsz - CODE_SIZE);

    if (gen_key(key) < KEY_SIZE)
        ABORT("[x]\tNot able to generate key\n");


    encrypt_rc4(elf->data + cave->p_offset, cave->p_memsz, key);

	insert_code(elf->data + cave->p_offset + cave->p_memsz,
                key,
				ehdr->e_entry, cave->p_offset + cave->p_memsz);



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





//	Elf64_Shdr *shdr = (Elf64_Shdr *)(elf->data + ehdr->e_shoff);
//	for (int j = 0; j < ehdr->e_shnum; j++)
//	{
//		if (cave->p_offset == shdr[j].sh_offset) {
////			ehdr->e_entry = shdr[j].sh_addr + shdr[j].sh_size;
////			ehdr->e_entry = cave->p_offset + cave->p_memsz;
//
//			printf("sh_addr =>  %lx and p_offset => %lx p_vaddr => %lx p_paddr => %lx\n",  shdr[j].sh_addr, cave->p_offset, cave->p_vaddr, cave->p_paddr);
//			printf("sh_size => %lx and p_filesz => %lx p_memsz => %lx  p_align => %lx\n", shdr[j].sh_size, cave->p_filesz, cave->p_memsz, cave->p_align);
//
//			printf("addr => %ld\n", shdr[j].sh_addr + shdr[j].sh_size);
////			insert_code(elf->data + shdr[j].sh_addr + shdr[j].sh_size);
//
//
//			printf("[+] Update segment size\n");
//
////			shdr[j].sh_size += CODE_SIZE;
//			return (0);
//		}
//	}
//	printf("NO section in segment\n");
	//	ehdr->e_entry = phdr->p_offset + phdr->p_memsz;
	return (0);
}
# woody_woodpacker
The "woody_woodpacker" project encrypts 64-bit ELF binaries, creating a self-decrypting executable named "woody" that runs identically to the original program while enhancing security

# More
The project involves creating a tool named "woody_woodpacker" that manipulates ELF (Executable and Linkable Format) files, specifically targeting 64-bit binaries. The primary function of this tool is to encrypt a given binary file, producing a modified version named "woody". When executed, "woody" will first decrypt itself in memory before running the original program's code, ensuring that the execution is identical to the unmodified binary.

The encryption process is designed to obfuscate the binary's content, making it difficult for antivirus programs to analyze the code before it's executed. This technique can be useful for protecting proprietary code or adding an additional layer of security. The tool allows users to select the encryption algorithm of their choice. In cases where an encryption key is required, the tool generates it as randomly as possible and displays it when the main program is run. Additionally, when the encrypted program ("woody") is executed, it displays a specific string ("....WOODY....") to indicate successful decryption before proceeding with the original program's execution flow.

This project requires a deep understanding of the ELF file format, encryption algorithms, and low-level programming in C and potentially assembly language for performance-critical sections. The final deliverable must include a Makefile with compilation instructions and adhere to best coding practices for security and efficiency.

# useful
```bash
debian@ov-74088f:/tmp$ echo -e  '#include <stdio.h>\n int main() {printf("hello world");}' | gcc  -x c - -c  -o hello.o
debian@ov-74088f:/tmp$ objcopy --add-section .mydata=mydata --set-section-flags .mydata=noload,readonly hello.o hello2.o
debian@ov-74088f:/tmp$ objdump -sj .mydata hello

hello:     file format elf64-x86-64

Contents of section .mydata:
 0000 64617461 20616464 65642074 6f206865  data added to he
 0010 6c6c6f0a                             llo.            
debian@ov-74088f:/tmp$ 
```


# link
https://sourceware.org/legacy-ml/binutils/2008-06/msg00216.html
https://stackoverflow.com/questions/233358/elf-file-headers
https://github.com/thorkill/eresi
https://sourceforge.net/p/elftoolchain/wiki/Home/

http://dbp-consulting.com/tutorials/debugging/linuxProgramStartup.html

https://netspooky.medium.com/elf-binary-mangling-part-1-concepts-e00cb1352301
http://phrack.org/issues/58/5.html#article
https://violentbinary.github.io/posts/1-elf-static-injection-to-load-malicious-dynamic-link-library/
https://fasterthanli.me/series/making-our-own-executable-packer

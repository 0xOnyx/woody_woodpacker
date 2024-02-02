# woody_woodpacker
The "woody_woodpacker" project encrypts 64-bit ELF binaries, creating a self-decrypting executable named "woody" that runs identically to the original program while enhancing security

# More
The project involves creating a tool named "woody_woodpacker" that manipulates ELF (Executable and Linkable Format) files, specifically targeting 64-bit binaries. The primary function of this tool is to encrypt a given binary file, producing a modified version named "woody". When executed, "woody" will first decrypt itself in memory before running the original program's code, ensuring that the execution is identical to the unmodified binary.

The encryption process is designed to obfuscate the binary's content, making it difficult for antivirus programs to analyze the code before it's executed. This technique can be useful for protecting proprietary code or adding an additional layer of security. The tool allows users to select the encryption algorithm of their choice. In cases where an encryption key is required, the tool generates it as randomly as possible and displays it when the main program is run. Additionally, when the encrypted program ("woody") is executed, it displays a specific string ("....WOODY....") to indicate successful decryption before proceeding with the original program's execution flow.

This project requires a deep understanding of the ELF file format, encryption algorithms, and low-level programming in C and potentially assembly language for performance-critical sections. The final deliverable must include a Makefile with compilation instructions and adhere to best coding practices for security and efficiency.

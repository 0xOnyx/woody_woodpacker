#include "utils.h"

//int main(){
//    unsigned char key[17] = "1234567890123456";
//    unsigned char sbuff[257];
//    unsigned char code[CODE_LEN] = "my very secret message is Hello World!";
//    init_rc4(key, sbuff);
//
//    decrypt(code, sbuff, CODE_LEN);
//
//    write(1, code, 38);
//}


int gen_key(unsigned char key[KEY_SIZE])
{
    int fd = open(RAND_FILE, O_RDONLY);
    if (fd <= 0)
        return (0);
    return (int)read(fd, key, KEY_SIZE);
}

static void init_rc4(unsigned char key[KEY_SIZE], unsigned char sbuff[257]){

    sbuff[256] = 0;

    for (int i = 0; i < 256; i++) {
        sbuff[i] = i;
    }
    int j = 0;
    for (int i = 0; i < 256; i++) {
        j = (j + sbuff[i] + key[i & 15]) & 255;

        unsigned char t = sbuff[i];
        sbuff[i] = sbuff[j];
        sbuff[j] = t;
    }
}

static unsigned char get_byte(unsigned char sbuff[256]){
    static int i = 0;
    static int j = 0;

    i = (i + 1) & 255;
    j = (j + sbuff[i]) & 255;
    
    char t = sbuff[i];
    sbuff[i] = sbuff[j];
    sbuff[j] = t;

    int l = (sbuff[i] + sbuff[j]) & 255;
    char k = sbuff[l];
    return k;
}

static void decrypt(unsigned char *code, unsigned char sbuff[256], unsigned long long len){
    for (unsigned long long i = 0; i < len; i++){
        unsigned char byte = get_byte(sbuff);
        code[i] = code[i] ^ byte;
    }
}

void encrypt_rc4(unsigned char *code, Elf64_Xword len, unsigned char key[KEY_SIZE])
{
    unsigned char buff[257];

    init_rc4(key, buff);

    decrypt(code, buff, len);
}

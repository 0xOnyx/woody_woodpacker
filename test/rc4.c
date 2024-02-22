#include <unistd.h>
#include <stdio.h>

#define CODE_LEN 39

void init_rc4(unsigned char key[17], unsigned char sbuff[256]);
void decrypt(unsigned char *code, unsigned char sbuff[256], int len);

int main(){
    unsigned char key[17] = "1234567890123456";
    unsigned char sbuff[257];
    unsigned char code[CODE_LEN] = "my very secret message is Hello World!";
    init_rc4(key, sbuff);

    decrypt(code, sbuff, CODE_LEN);

    write(1, code, 38);
}

void init_rc4(unsigned char key[17], unsigned char sbuff[257]){

    sbuff[256] = 0;

    for (int i = 0; i < 256; i++) {
        sbuff[i] = i;
    }
    int j = 0;
    for (int i = 0; i < 256; i++) {
        j = (j + sbuff[i] + key[i & 15]) & 255;

        char t = sbuff[i];
        sbuff[i] = sbuff[j];
        sbuff[j] = t;
    }
}

unsigned char get_byte(unsigned char sbuff[256]){
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

void decrypt(unsigned char *code, unsigned char sbuff[256], int len){
    for (int i = 0; i < len; i++){
        unsigned char byte = get_byte(sbuff);
        code[i] = code[i] ^ byte;
    }
}

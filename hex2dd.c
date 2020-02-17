#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void hex2dd(long addressNum){
    long addrSections[4];

    // Split up into sections
    for (int i = 0; i < 4; i++){
        long sec = addressNum & 0xff;
        addressNum >>= 8;
        addrSections[i] = sec;
    }

    // Print in reverse order since we started with low bytes
    for (int i = 3; i >= 0; i--){
        printf("%ld.", addrSections[i]);
    }
    printf("\n");

}

void dd2hex(const char * stringAddr){
    char * start = stringAddr;
    char * end;
    char subString[4];
    long hexNum;

    printf("0x");
    while ((end = strstr(start, "."))){
        //printf("%s ", start);
        strncpy(subString, start, end - start);
        hexNum = atoi(subString);
        printf("%02lx", hexNum);
        start = end + 1;
        memset(subString, '\0', 4);
    }

    hexNum = atoi(start);
    printf("%02lx ", hexNum);

    printf("\n");
}

int main(){

    //hex2dd(0x8002c2f2);
    dd2hex("128.2.194.242");

    return 0;
}

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CHANNEL_COUNT 3

void printUsage();
void printVersion();
void printInfo();
void invertInt8(uint8_t *x);
void hexStrToInt8(char x[2], uint8_t *dest);
void int8ToHexStr(uint8_t x, char dest[2]);
void invertHexStr(char x[2]);

uint8_t channels[CHANNEL_COUNT] = {};

char hexCharLUT[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };

int main(int argc, char **argv)
{
#ifdef DEBUG
    printf("Color inverter debug version. argc is %i.\n", argc);
#endif
    if(argc < 2){
        fputs("Error: Argument count is below 1.\n", stderr);
        printUsage();
        return EXIT_FAILURE;
    } else if(argc == 2){
        char str[8] = "#000000";

#ifdef DEBUG
        puts("Argc equals 2. Assuming input hexStr or --help\nChecking for '#' at the beggining and strlen(argv[1] >= 7)...");
#endif
        if(strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0){
            printUsage();
            printInfo();
            return EXIT_SUCCESS;
        } else if(strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--version") == 0){
            printVersion();
            return EXIT_SUCCESS;
        } else if(argv[1][0] != '#'){
            fputs("Error: '#' not found at the beginning of the first argument.\n", stderr);
            printUsage();
            return EXIT_FAILURE;
        } else if(strlen(argv[1]) < 7){
            fputs("Error: length of the first argument is below 7.\n", stderr);
            printUsage();
            return EXIT_FAILURE;
        } else if(strlen(argv[1]) > 7){
            fputs("Warning: length of the first argument is greater than 7. Characters beyond no. 7 will be ignored.\n", stderr);
        }
#ifdef DEBUG
        puts("OK\n");
#endif

        for(int i = 1; i < CHANNEL_COUNT * 2; i += 2){
            char buf[2] = { argv[1][i], argv[1][i + 1] };
#ifdef DEBUG
            printf("Loop index is %i. buf[0] is argv[1][%i], AKA '%c', and buf[1] is argv[1][%i + 1], AKA '%c'.\n", (i - 1) / 2, i, buf[0], i, buf[1]); 
#endif
            invertHexStr(buf);
#ifdef DEBUG
            printf("Copying \"%c%c\" to str...\t", buf[0], buf[1]);
#endif
            strncpy(&str[i], buf, 2);
#ifdef DEBUG
            printf("OK. Now str is %s.\n", str);
#endif
        }
        fprintf(stdout, "%s\n", str);
    } else if(argc > CHANNEL_COUNT){
        for(int i = 0; i < CHANNEL_COUNT; i++){
            uint8_t value = atoi(argv[i + 1]);
            if(value < 0 || value > 255){
                printUsage();
                return EXIT_FAILURE;
            } else {
                invertInt8(&value);
                channels[i] = value;
            }
        }
        printf("%i %i %i\n", channels[0], channels[1], channels[2]);
    } else {
        fputs("Error: invalid number of arguments\n", stderr);
        printUsage();
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}

void printUsage()
{
    printf("Usage:\n\tcolor-inverter <R> <G> <B>\n\t\tWhere R, G and B are values between 0 and 255\nOR\n\tcolor-inverter '#xxxxxx'\n\t\tWhere x is a hex digit (0-f), case insensitive\nOR\n\tcolor-inverter -h|--help to display this message\n\tcolor-inverter -v|--version to display the version\n");
}

void printVersion(){
    printf("RGB inverter v1.0\nMade by Jan Sołtan on Friday October 10th 2023 at 21:36.\n");
}

void printInfo(){
    printf("This is a simple program which will return an inverted version of a given RGB color value (subtract each channel from 255).\n");
}

void invertInt8(uint8_t *x)
{
#ifdef DEBUG
    printf("\n\t---- BEGIN invertInt8 function with input %i. ----\n", *x);
#endif
    *x = 255 - *x;
#ifdef DEBUG
    printf("\t---- END invertInt8 function with output %i. ----\n\n", *x);
#endif
}

void hexStrToInt8(char x[2], uint8_t *dest)
{
    *dest = 0;
#ifdef DEBUG
    printf("\n\t---- BEGIN hexStrToInt8 function with input \"%c%c\". ----\n", x[0], x[1]);
#endif
    for(int i = 0; i < 2; i++){
        int shift = 4 - (i * 4);
        uint8_t value = 0;
#ifdef DEBUG
        printf("\t\tLoop index is %i, shift is %i.\n", i, shift);
#endif
        if(x[i] >= 48 && x[i] <= 57){
#ifdef DEBUG
            printf("\t\t\tinput[%i] is '%c' (digit), converted to %i.\n", i, x[i], x[i] - 48);
#endif
            value = x[i] - 48;
        } else if(x[i] >= 65 && x[i] <= 70){
#ifdef DEBUG
            printf("\t\t\tinput[%i] is '%c' (uppercase char), converted to %i.\n", i, x[i], x[i] - 65 + 10);
#endif
            value = x[i] - 65 + 10;
        } else if(x[i] >= 97 && x[i] <= 102){
#ifdef DEBUG
            printf("\t\t\tinput[%i] is '%c' (lowercase char), converted to %i.\n", i, x[i], x[i] - 97 + 10);
#endif
            value = x[i] - 97 + 10;
        }
        value <<= shift;
#ifdef DEBUG
        printf("\n\t\tValue after shift is %i (binary %b). Adding to dest...\n", value, value);
#endif
        *dest += value;
    }
#ifdef DEBUG
    printf("\t---- END hexStrToInt8 function with output %i. ----\n\n", *dest);
#endif
}

void int8ToHexStr(uint8_t x, char dest[2])
{
    memset(dest, 0, 2);
#ifdef DEBUG
    printf("\n\t---- BEGIN int8ToHexStr function with input %i. ----\n", x);
#endif
    for(int i = 0; i < 2; i++){
        int shift = 4 - (i * 4);
        uint8_t mask = 15 << shift;
#ifdef DEBUG
        printf("\t\tLoop index is %i, shift is %i, mask is %b.\n", i, shift, mask);
#endif
        dest[i] = hexCharLUT[(x & mask) >> shift];
#ifdef DEBUG
        printf("\t\t\tValue of dest[%i] was changed to be that of lookupTable[%i], AKA '%c'.\n", i, (x & mask) >> shift, hexCharLUT[(x & mask) >> shift]);
#endif
    }
#ifdef DEBUG
    printf("\n\t---- END int8ToHexStr function with output \"%c%c\". ----\n\n", dest[0], dest[1]);
#endif
}

void invertHexStr(char s[2])
{
#ifdef DEBUG
    printf("\n---- BEGIN invertHexStr function with input value \"%c%c\". ----\n\n", s[0], s[1]);
#endif
    uint8_t val;
    hexStrToInt8(s, &val);
    invertInt8(&val);
    int8ToHexStr(val, s);
#ifdef DEBUG
    printf("---- END invertHexStr function with output value \"%c%c\". ----\n\n", s[0], s[1]);
#endif
}

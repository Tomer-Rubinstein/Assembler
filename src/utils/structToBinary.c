#include <stdlib.h>
#include <string.h>
#include "essentials.h"

/*
    BinaryConversionR() converts a R command values to binary
    params:
        - int opcode
        - int rs
        - int rt
        - int rd
        - int funct
    returns:
        (char *) the binary representation of the command's struct's fields
*/
char *BinaryConversionR(int opcode, int rs, int rt, int rd, int funct){
    char * ptr = calloc( 33 ,sizeof(char));
    strcat(ptr, decimalToBinary(opcode, 6));
    strcat(ptr, decimalToBinary(rs, 5));
    strcat(ptr, decimalToBinary(rt, 5));
    strcat(ptr, decimalToBinary(rd, 5));
    strcat(ptr, decimalToBinary(funct, 5));
    strcat(ptr, decimalToBinary(0, 6)); /* not in use bits */
    return ptr;
}

/*
    BinaryConversionI() converts an I command values to binary
    params:
        - int opcode
        - int rs
        - int rt
        - int operandIsLabel, the second pass should fill the encoded binary of the label's value
    returns:
        (char *) the binary representation of the command's struct's fields
*/
char *BinaryConversionI(int opcode, int rs, int rt, int immed, int operandIsLabel){
    char * ptr = calloc( 33 ,sizeof(char));
    strcat(ptr, decimalToBinary(opcode, 6));
    strcat(ptr, decimalToBinary(rs, 5));
    strcat(ptr, decimalToBinary(rt, 5));
    if(!operandIsLabel)
        strcat(ptr,decimalToBinary(immed, 16));
    
    return ptr;
}

/*
    BinaryConversionJ() converts a J command values to binary
    params:
        - int opcode
        - int rs
        - int address
        - int operandIsLabel, the second pass should fill the encoded binary of the label's value
    returns:
        (char *) the binary representation of the command's struct's fields
*/
char *BinaryConversionJ(int opcode, int reg, int address, int operandIsLabel){
    char * ptr = calloc( 33 ,sizeof(char));
    strcat(ptr, decimalToBinary(opcode, 6));
    strcat(ptr, decimalToBinary(reg, 1));
    if(!operandIsLabel)
        strcat(ptr, decimalToBinary(address, 25));
    
    return ptr;
}

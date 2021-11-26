#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
  getCommentIndicatorIndex() returns the index of the char ';' in a given string
  params:
    - char *line, the line to search for the ';' character
  return:
    - (-1) if ';' was not found
    - (int) returns the index of ';' in line
*/
int getCommentIndicatorIndex(char *line){
  int i;
  for(i=0; i < strlen(line); i++)
    if(line[i]==';')
      return i;
  
  return -1;
}

/*
  decimalToBinary() converts decimal number to it's binary form (as string)
  params:
    - int decimal, the number to convert to binary
    - int size_ptr, how many bits should the binary be
  return:
    - (NULL) if program couldn't allocate enough memory
    - (char *) the binary representation of the given decimal number
*/
char *decimalToBinary(int decimal, int size_str){
  int i;
  char *binary = calloc(size_str + 2, sizeof(char));
  if(binary == NULL){
    printf("Program couldn't allocate enough memory");
    return NULL;
  }
  for(i = size_str - 1; i >= 0; i--){
    *(binary + i) = (decimal & 1) + '0';
    decimal = decimal >> 1;
  }
  *(binary + size_str + 1) = '\0';
  return binary;
}


/*
  binaryToDecimal() converts binary string to a decimal number
  params:
    - char *, the binary string to convert to decimal
  return:
    - (int) the decimal number converted from the given binary string
*/
int binaryToDecimal(char *binary){
  long currPower = 1;
  long result = 0;
  int i;

  for(i=strlen(binary)-1; i >= 0; i--){
    if(binary[i] == '1')
      result += currPower;
    
    currPower *= 2;
    
  }

  return result;
}

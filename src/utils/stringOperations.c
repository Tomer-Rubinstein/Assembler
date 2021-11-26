#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* extractNumFromStr() removes comments from a string and whitespaces
  params:
   - char *str, the string to strip
  returns:
   - (char *) the stripped string
*/
char *strip(char *str){
  int i;
  char *newStr = (char *) malloc((strlen(str)+1) * sizeof(char));
  memset(newStr, '\0', strlen(str)+1);

  for(i=0; i < strlen(str); i++){
    if(str[i] == ';')
      break;
    
    if(str[i] != '\n' && str[i] != '\t' && str[i] != ' '){
      strncat(newStr, &str[i], 1);
    }
  }

  return newStr;
}

/* firstWord() returns the first sequence of characters until a space
  params:
   - char *str, the string to get it's first seperated sequence of characters
  returns:
   - (char *) the sequence of characters seperated by a space
*/
char *firstWord(char *str){
  char *first = (char *)calloc(strlen(str) + 1, sizeof(char));
  int i;
  int isReadingName = 0;
  
  for(i=0; i < strlen(str); i++){
    if((str[i] == ' ' || str[i] == '\t') && isReadingName)
      break;
    if(str[i] == ':'){
      first[0] = '\0';
      isReadingName = 0;
      continue;
    }

    
    if(str[i] != ' ' && str[i] != '\t'){
      isReadingName = 1;
      strncat(first, &str[i], 1);
    }
      
  }
  
  return first;
}

/* extractNumFromStr() extracts the first sequence of digits in a given string(sequence could be broken by a non-digit value)
  params:
   - char *str, the string to extract the first sequence of digits from
   - int allowNegs(zero, or any other), to allow negative numbers(don't mark the '-' char as invalid)
  returns:
   - (int) -1 if invalid character placement, or the extracted number from the string as integer
*/
int extractNumFromStr(char *str, int allowNegs){
  char *newStr = (char *)calloc(strlen(str) + 1, sizeof(char));
  int isNum = 0;
  int i, num;

  for(i=0; i < strlen(str); i++){
    if(str[i] == '-' && !isNum && !allowNegs)
        return -1;
    if(('0' <= str[i] && str[i] <= '9') || (!isNum && str[i] == '-')){
      isNum = 1;
      strncat(newStr, &str[i], 1);
    }else{
      if(isNum)
        break;
    }
  }
  num = atoi(newStr);
  free(newStr);

  return num;
}

/* getNumLength() returns how many digits are in a given integer
  params:
   - int num, the int to check it's digit length
  returns:
   - (int) how many digits are in the given int num
*/
int getNumLength(int num){
  int numLength = 0;

  while(num > 0){
    numLength++;
    num /= 10;
  }

  return numLength;
}


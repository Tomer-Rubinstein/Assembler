#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "./commandTypes.h"
#include "stringOperations.h"
#include "symbolTable.h"
#include "../instructions.h"

#define SYMBOL_LENGTH 31

int validateRegister(char *str);
int missingComma(char *cmd, int expectedCommas);
int validateOperands(char *cmd, int expectedParams, int cmdType, int lineNum, char *filename);
int validateDataInstruction(char *instruction, int lineNum, int minValue, int maxValue, char *filename);

/*
  validateLine() is the function that checks the syntax of a given line,
  params:
    - char *line, the line to check it's syntax
    - int lineNum, the current line (used for the error message)
  return:
    - (1) if an error was found
    - (0) if no errors were found in the line
*/
int validateLine(char *line, int lineNum, char *filename){
  int params = 0;
  int commas = 0;
  int i;

  int isSymbol = 0;
  int isDataInstruction = 0;
  int isChar = 0;

  int minValue = 0;
  int maxValue = 0;

  GlobalCommand *currCmd = NULL;
  GlobalCommand *tempNode = CommandsHeadNode;

  /* finding a symbol definition, if there is */
  char *currSymbol = (char *) calloc(SYMBOL_LENGTH+1, sizeof(char));
  char *symbolCommand = (char *) calloc(strlen(line)+1, sizeof(char));

  char *cmdName;

  /* clearing comments from the line */
  for(i=0; i < strlen(line); i++){
    if(line[i] == ';'){
      line[i] = '\0';
      break;
    }    
  }

  /* getting the name of a symbol and it's value */  
  for(i=0; i < strlen(line) && strstr(line, ":"); i++){
    if(line[i] == ':'){
      isSymbol = 1;
    }else if(!isSymbol && line[i] != ' '){
      strncat(currSymbol, &line[i], 1); 
    }else if(isSymbol){
      if(line[i] != ' ')
        isChar = 1;
      if(isChar)
        strncat(symbolCommand, &line[i], 1);
    }
  }

  /* skip empty lines */
  if(line == NULL || strlen(strip(line)) == 0){
    return 0;
  }

  /* symbol validation */
  if(isSymbol){
    if(strlen(currSymbol) >= 31){
      fprintf(stdout, "(%s) In line %d: symbol name length out of range\n", filename, lineNum);
      return 1;
    }

    /* symbol start with alpha */
    if(!isalpha(currSymbol[0]) && isSymbol){
      fprintf(stdout, "(%s) In line %d: symbols must start with an alphabetic character\n", filename, lineNum);
      return 1;
    }

    /* symbol uniqueness */
    /* defining new symbol and it already exists */
    if(currSymbol != NULL && getSymbol(currSymbol) != NULL && isSymbol){
      fprintf(stdout, "(%s) In line %d: multiple definitions for %s\n", filename, lineNum, currSymbol);
      return 1;
    }

    /* for example "label:" */
    if(strlen(symbolCommand) == 0){
      fprintf(stdout, "(%s) In line %d: invalid label definition, unspecified value\n", filename, lineNum);
      return 1;
    }

    for(i=0; i < strlen(currSymbol); i++){
      if(currSymbol[i] == ' ' || isprint(currSymbol[i]) == 0){
        fprintf(stdout, "(%s) In line %d: unexpected character in label name\n", filename, lineNum);
        return 1;
      }
    }

    /* if currSymbol is a saved keyword */
    for(i=0; i < KEYWORDS_COUNT; i++){
      if(strstr(keywords[i], currSymbol)){
        fprintf(stdout, "(%s) In line %d: invalid label name\n", filename, lineNum);
        return 1;
      }
    }

    line = symbolCommand;
  }
  cmdName = firstWord(line);

  while(tempNode != NULL){
    if(strcmp(tempNode->cmdName, strip(cmdName)) == 0){
	    currCmd = tempNode;
      break;
    }
    tempNode = tempNode->nextCmd;
  }

  /* unknown instruction */
  if(currCmd != NULL){
    params = (currCmd->params < 0) ? 0 : currCmd->params;
    commas = (params-1 < 0) ? 0 : (currCmd->params)-1;
  }else{
    isDataInstruction = 0;
    for(i=0; i < DATA_INSTRUCTION_COUNT; i++){
      if(strcmp(cmdName, Data_Instructions[i]) == 0){
        isDataInstruction = 1;
        break;
      }
    }
    if(isDataInstruction){
      if(i <= 3){
        minValue = DataInstructionRangeValues[i][0];
        maxValue = DataInstructionRangeValues[i][1];
      }
      return validateDataInstruction(line, lineNum, minValue, maxValue, filename);
    }else if(strstr(line, ".entry") != NULL || strstr(line, ".extern") != NULL){
      return 0;
    }else{
      fprintf(stdout, "(%s) In line %d: undefined instruction\n", filename, lineNum);
      return 1;
    }
    return 0;
  }

  /* comma validation(missing & extraneous) */
  switch(missingComma(line, commas)){
    case -1:
      fprintf(stdout, "(%s) In line %d: missing comma(s)\n", filename, lineNum);
      return 1;
    case 1:
      fprintf(stdout, "(%s) In line %d: extraneous comma(s)\n", filename, lineNum);
      return 1;
    default:
      break;
  }


  /* params validation(missing & extraneous) */
  if(validateOperands(line, params, currCmd->commandType, lineNum, filename))
    return 1;
  free(symbolCommand);
	free(currSymbol);
  free(cmdName);
  return 0;
}


/* validateRegister() returns if a register is valid(in range from 0 to 31, not null, not an alphabetic value)
  params:
   - char *str, the given string that contains a single register (if not, checking the one that appears first)
  returns:
   - int (0 or 1), 1 if register is valid, 0 if not
*/
int validateRegister(char *str){
  int i;
  int isReg = 0;
  char *reg = (char *) malloc(3 * sizeof(char));
	memset(reg, '\0', 3);
  for(i=0; i < strlen(str); i++){
    if(str[i] == '$'){
      isReg = 1;
      continue;
    }
    if(!('0' <= str[i] && str[i] <= '9') && isReg)
      return 0;
    
    if(isReg){
      if(strlen(reg) > 2)
        return 0;
      strncat(reg, &str[i], 1);
    }else if((str[i] == ' ' || str[i] == '\n' || str[i] == '\t') && isReg){
      break;
    }
  }

  if(!(0 <= atoi(reg) && atoi(reg) <= 31) || strlen(reg) == 0)
    return 0;  
  free(reg);
  return 1;
}


/* missingComma() returns if a given string as just the amount of commas as expected by the expectedCommas paramter
  params:
   - char *cmd, the string to check it's commas count
   - int expectedCommas, how much commas should be in the string
  returns:
   - int (0, 1 or -1), 1 if there more commas than expected, -1 if there are less commas than expected, 0 if just the right amount
*/
int missingComma(char *cmd, int expectedCommas){
  int i;
  int commas = 0;
  for(i=0; i < strlen(cmd); i++)
    if(cmd[i] == ',')
      commas++;
  
  if(commas > expectedCommas)
    return 1; /* extraneous comma(s) */
  else if(commas < expectedCommas)
    return -1; /* missing comma(s) */
  return 0;
}


/* validateOperands() checks for missing/extraneous/invalid/out of range operand(s) in a given command string
  params:
   - char *cmd, the string to check it's parameters
   - int expectedParams, how much params should be
   - int cmdType, the type of the given command
   - int lineNum, the current line number
  returns:
   - int (0, 1), 1 if an error was occurred, 0 if not
*/
int validateOperands(char *cmd, int expectedParams, int cmdType, int lineNum, char *filename){
  char *token;
  int params = 0;
  char *copy = calloc(strlen(cmd) + 1,  sizeof(char));
  char *cmdName = firstWord(cmd);

  strncpy(copy, cmd, strlen(cmd));

  token = strtok(copy, " ");
  token = strtok(NULL, ","); /* not counting the command name */

  while(token != NULL){
    token = strip(token);
    if(cmdType == R_Type && !validateRegister(token)){
      fprintf(stdout, "(%s) In line %d: invalid operand\n", filename, lineNum);
      return 1;
    }else if(cmdType == I_Type){
      if(strcmp(cmdName, "beq")==0 || strcmp(cmdName, "bne")==0 || strcmp(cmdName, "blt")==0 || strcmp(cmdName, "bgt")==0){
        if((params == 0 || params == 1) && !validateRegister(token)){
          fprintf(stdout, "(%s) In line %d: invalid operand\n", filename, lineNum);
          return 1;
        }
      }else if((params == 0 || params == 2) && !validateRegister(token)){
        fprintf(stdout, "(%s) In line %d: invalid operand\n", filename, lineNum);
        return 1;
      }else if(params == 1){
        int num = atoi(token);
        if(!(-32768 <= num && num <= 32767) || (strlen(token) > 5 && !strstr(token, "-")) || (strlen(token) > 6 && strstr(token, "-"))){
          fprintf(stdout, "(%s) In line %d: value out of range\n", filename, lineNum);
          return 1;
        }

        if(atoi(token) == 0 && strcmp(strip(token), "0")){
          fprintf(stdout, "(%s) In line %d: invalid operand\n", filename, lineNum);
          return 1;
        }
      }
    }else if(cmdType == J_Type && strstr(token, "$") && !validateRegister(token)){
      fprintf(stdout, "(%s) In line %d: invalid operand\n", filename, lineNum);
      return 1;
    }
    params++;
    
    token = strtok(NULL, ",");
  }
  if(params > expectedParams)
    fprintf(stdout, "(%s) In line %d: extraneous operand(s)\n", filename, lineNum); /* extraneous operand(s) */
  else if(params < expectedParams)
    fprintf(stdout, "(%s) In line %d: missing operand(s)\n", filename, lineNum); /* missing operand(s) */
  free(copy);
  free(cmdName);
	return 0;
}


/*
  validateDataInstruction() validates data instructions(.db, .dh, .dw, .asciz, .entry, .extern),
  it writes an error message and returns a status code (0 or 1) if an error was found.
  params:
    - char *instruction : the instruction to check it's syntax
    - int lineNum       : the current line (used for the error message)
    - ine minValue      : if a command accepts integer values(.db, .dh, .dw) minValue is the smallest number that can be accepted
    - int maxValue      : if a command accepts integer values(.db, .dh, .dw) maxValue is the biggest number that can be accepted
  return:
    - (1) if an error was found
    - (0) if no errors were found in the line
*/
int validateDataInstruction(char *instruction, int lineNum, int minValue, int maxValue, char *filename){
  char *operands = (char *) malloc(sizeof(char) * (strlen(instruction)-strlen(firstWord(instruction))));
  char *currNum = (char *) malloc(sizeof(char) * 11);

  int num;
  int i;

  int spaceFlag = 0;
  int quoteFlag = 0;
  int encounteredName = 0;

	memset(operands,'\0', strlen(instruction)-strlen(firstWord(instruction)));
	memset(currNum,'\0', 11); 

  /* since it's guaranteed that there is a space between the keyword and the operands
  and the keyword is valid (checked before the function call),
  we can remove it from the string and leave only the operands */
  for(i=0; i < strlen(instruction); i++){
    if(instruction[i] != ' ' && instruction[i] != '\t')
      encounteredName = 1;
    if(instruction[i] == ' ' && !spaceFlag && encounteredName)
      spaceFlag = 1;
    else if(spaceFlag)
      strncat(operands, &instruction[i], 1);
  }

  /* validate operands for null safety */
  if(strlen(operands) == 0){
    fprintf(stdout, "(%s) In line %d: no operands\n", filename, lineNum);
    return 1;
  }

  /* check for errors in the .asciz command */
  if(strstr(instruction, ".asciz")){
    for(i=0; i < strlen(operands); i++){
      if(operands[i] == '\"' && !quoteFlag)
        quoteFlag = 1;
      else if(operands[i] == '\"' && quoteFlag){
        quoteFlag = 0;
        if(i+1 != strlen(operands) && !(i >= strlen(strip(operands))-1)){
          fprintf(stdout, "(%s) In line %d: extraneous data after end of string\n", filename, lineNum);
          return 1;
        }
        return 0;
      }

      if(quoteFlag){
        if(!isprint(operands[i])){
          fprintf(stdout, "(%s) In line %d: unprintable character\n", filename, lineNum);
          return 1;
        }
      }else{
        if(operands[i] != ' ' && operands[i] != '\t' && !quoteFlag){
          fprintf(stdout, "(%s) In line %d: invalid string definition\n", filename, lineNum);
          return 1;
        }
      }
    }

    if(quoteFlag){
      fprintf(stdout, "(%s) In line %d: invalid string ending\n", filename, lineNum);
      return 1;
    }
    return 0;
  }

  if(operands[0] == ',' || (operands[strlen(operands)-1] == ',')){
    fprintf(stdout, "(%s) In line %d: invalid comma\n", filename, lineNum);
    return 1;
  }


  /* do operations of currNum to validate value range, missing comma and invalid operand */
  for(i=0; i < strlen(operands); i++){
    if(operands[i] == '-'){
      if((i+1 < strlen(operands) && !isdigit(operands[i+1])) || (i-1 >= 0 && isdigit(operands[i-1]))){
        fprintf(stdout, "(%s) In line %d: unexpected character\n", filename, lineNum);
        return 1;
      }
      strncat(currNum, &operands[i], 1);

    }else if(operands[i] == '+'){
      if((i+1 < strlen(operands) && !isdigit(operands[i+1])) || (i-1 >= 0 && isdigit(operands[i-1]))){
        fprintf(stdout, "(%s) In line %d: unexpected character\n", filename, lineNum);
        return 1;
      }
    }else if(isdigit(operands[i])){
      strncat(currNum, &operands[i], 1);
      num = atoi(currNum);

      if(!(minValue <= num && num <= maxValue)){
        fprintf(stdout, "(%s) In line %d: value out of range\n", filename, lineNum);
        return 1;
      }
    }else if(operands[i] == ',' || i >= strlen(strip(operands))-1){
      currNum[0] = '\0';
    }else if((operands[i] == ' ' || operands[i] == '\t') && strlen(currNum) != 0){
      fprintf(stdout, "(%s) In line %d: missing comma(s)\n", filename, lineNum);
      return 1;
    }else{
      if(!isdigit(operands[i]) && operands[i] != ' ' && operands[i] != '\t'){
        fprintf(stdout, "(%s) In line %d: 4 unexpected character\n", filename, lineNum);
        return 1;
      }
    }

  }

  operands = strip(operands);
  for(i=0; i < strlen(operands)-1; i++){
    if(operands[i] == ',' && operands[i+1] == ','){
      fprintf(stdout, "(%s) In line %d: extraneous comma(s)\n", filename, lineNum);
      return 1;
    }
  }
  free(operands);
  free(currNum);
  return 0;
}

#include "MemNode.h"
#include "./utils/structToBinary.h"
#include "./dataList.h"
#include "./utils/stringOperations.h"
#include "LabelNode.h"
#include "./utils/essentials.h"
#include "./utils/symbolTable.h"
#include "./utils/stringOperations.h"

#include "first_pass.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int ICF;
int DCF;

/* binaryToHex() returns as string the hex value of a given binary string 
  params:
   - char *binaryRepresentation, the binary string to convert to hex
   - int isDataInstruction, the hex value of data instruction binary isn't being reversed
  returns:
   - char *, (string) the hex value of the given binary string
*/
char *binaryToHex(char *binaryRepresentation){
  char *currBinary = (char *) malloc((sizeof(char) * 4)+1);
  char *hex = (char *) malloc((sizeof(char) * 8)+1);
  char *currHexValue = (char *) malloc((sizeof(char) * 2)+1);
  char *result;

  char c;
  int i;
  int currDecimal = 0;

  memset(currBinary, '\0', 5);
  memset(hex, '\0', 9);
	memset(currHexValue,'\0', 3);

  /* first part: converting the binary to hex (will result in reversed hex values) */
  for(i=0; i < strlen(binaryRepresentation); i++){
    strncat(currBinary, &binaryRepresentation[i], 1);
    if((i+1)%4 == 0){
      currDecimal = binaryToDecimal(currBinary);

      if(0 <= currDecimal && currDecimal <= 9){
        c = 48 + currDecimal;
        strncat(hex, &c, 1);
      }else if(10 <= currDecimal && currDecimal <= 15){
        c = 64 + currDecimal-9; /* according to the ascii table */
        strncat(hex, &c, 1);
      }

      memset(currBinary, '\0', 5);
    }
  }

  result = (char *) malloc((sizeof(char) * strlen(hex))+1);
	memset(result, '\0', strlen(hex)+1);

  for(i=strlen(hex)-1; i >= 0; i--){
    strncat(currHexValue, &hex[i], 1);

    if(strlen(currHexValue) == 2){
      c = currHexValue[0];
      currHexValue[0] = currHexValue[1];
      currHexValue[1] = c;

      strncat(result, currHexValue, 2);
      currHexValue[0] = '\0';
    }

  }


  free(currHexValue);
  free(hex);
  free(currBinary);
  return result;
}


/* binaryToHex() returns as string the hex value of a given memory node (struct)
   params:
    - struct memNode *node, the command to translate
   returns:
    - char *, (string) representing the hex value of a given command.
 */
char *memoryToHex(struct memNode *node){
  char *binaryRepresentation;



  /* converting the node values to it's binary form */
  switch(node->commandType){
    case R_COMMAND:
      binaryRepresentation = BinaryConversionR(
        (node->command).commandR.opcode,
        (node->command).commandR.rs,
        (node->command).commandR.rt,
        (node->command).commandR.rd,
        (node->command).commandR.funct
      );
      break;
    case I_COMMAND:
      binaryRepresentation = BinaryConversionI(
        (node->command).commandI.opcode,
        (node->command).commandI.rs,
        (node->command).commandI.rt,
        (node->command).commandI.immed,
        0 
      );
      break;
    case J_COMMAND:
      binaryRepresentation = BinaryConversionJ(
        (node->command).commandJ.opcode,
        (node->command).commandJ.reg,
        (node->command).commandJ.address,
        0 
      );
      break;

    default:
      return NULL;
  }
  
  return binaryToHex(binaryRepresentation);
}


/* dataToHex() returns as string the hex value of a given data instruction (struct) 
   params:
    - struct dataList *node, the data instruction to encode
   returns:
    - char *, (string) representing the hex value of the given data instruction. 
*/
char *dataToHex(struct dataNode *node){
  int bitfieldSize = 0;
  char *binaryRep;

  switch(node->dataType){
    case DB:
      bitfieldSize = DB_BITFIELD_SIZE;
      binaryRep = decimalToBinary((node->dataCommand).dbCommand.data, bitfieldSize);
      break;
    case DH:
      bitfieldSize = DH_BITFIELD_SIZE;
      binaryRep = decimalToBinary((node->dataCommand).dhCommand.data, bitfieldSize);
      break;
    case DW:
      bitfieldSize = DW_BITFIELD_SIZE;
      binaryRep = decimalToBinary((node->dataCommand).dwCommand.data, bitfieldSize);
      break;

    default:
      break;
  }

  if(binaryRep != NULL && strlen(binaryRep) > 0){
    return binaryToHex(binaryRep);
  }
  return NULL;
}


/* outputObjectFile() creates a file named by outputFileName(param) with the encoded binary as hex
   params:
    - struct memNode *memHead, the start of the memory code
    - struct dataList *dataHead, the start of the data code
    - char *outputFileName, the name of the object file(assuming the ".ob" file extension is included in the name)
   returns:
    nothing, void function
 */
void outputObjectFile(struct memNode *memHead, struct dataNode *dataHead, char *outputFileName){
  struct memNode *currMemNode = memHead;
  struct dataNode *currDataNode = dataHead;
  char *hex;
  char *currHexValue = (char *) malloc((sizeof(char) * 3)+1);

  FILE *fp = fopen(outputFileName, "a");

  int i;
  int hexisCount = 0;
  int instructionCounter = 100;

	memset(currHexValue, '\0', 4);


  if(memHead == NULL || dataHead == NULL) /* no code to convert to hex */
    return;

  fprintf(fp, "\t%d %d\n", ICF-100, DCF); /* writing the title (first line): ICF and DCF */

  /* encoding the memory list to hex and writing to the object file line by line */
  while(currMemNode != NULL){
    hex = memoryToHex(currMemNode);


    for(i=0; i < strlen(hex); i++){
      strncat(currHexValue, &hex[i], 1);

      /* writing the address (IC) for each line */
      if(i%8 == 0){
        fprintf(fp, "0%d\t", instructionCounter);
        instructionCounter += 4;
      }

      /* writing 2 hexis + a space to the object file  */
      if((i+1)%2 == 0){
        strncat(currHexValue, " ", 1);
        fputs(currHexValue, fp);

        hexisCount++;
        currHexValue[0] = '\0';
      }

      /* new line after 4 hexis are in line */
      if(hexisCount%4 == 0 && hexisCount != 0){
        fputs("\n", fp);
        hexisCount = 0;
      }

    }
  
    currMemNode = currMemNode->next;
  }


  while(currDataNode != NULL){

    hex = dataToHex(currDataNode);

    for(i=0; i < strlen(hex); i++){
      if(hexisCount == 0){
        fprintf(fp, "0%d", instructionCounter);
        fputs("\t", fp);
        instructionCounter += 4;
      }

      strncat(currHexValue, &hex[i], 1);
      hexisCount++;

      if((i+1)%2 == 0){
        strncat(currHexValue, " ", 1);
        fputs(currHexValue, fp);
        currHexValue[0] = '\0';
      }

      if(hexisCount%8 == 0 && hexisCount != 0){
        fputs("\n", fp);
        hexisCount = 0;
      }
      
    }



    currDataNode = currDataNode->next;
  }

  if(hexisCount%4 != 0 && hexisCount != 0)
    fputs("\n", fp);
  

  free(currHexValue);
  fclose(fp);
}


/* outputExternFile() creates a file named by outputFileName(param) with the occurrences of uses of extern labels in the code
   output format: `ADDRESS LABEL_NAME`
   params:
    - struct NodeList *externals, the linked list that contains all the occurrences of extern labels in the code
    - char *outputFileName, the name of the externs file(assuming the ".ext" file extension is included in the name)
   returns:
    nothing, void function
 */
void outputExternFile(struct NodeList *externals, char *outputFilename){
  FILE *fp = fopen(outputFilename, "a+");
  struct NodeList *currExternal = externals;

  if(externals == NULL) /* no externals were specified in the code */
    return;


  while(currExternal != NULL){
    fprintf(fp, "%s 0%d\n", currExternal->label, currExternal->address);
    currExternal = currExternal->next;
  }
}


/* outputExternFile() creates a file named by outputFileName(param) with the occurrences of definitions of entry labels in the code
   output format: `ADDRESS LABEL_NAME`
   params:
    - struct symbolNode *symbolTableHead, the linked list that contains all of the symbol definitions
    - char *outputFileName, the name of the entries file(assuming the ".ent" file extension is included in the name)
   returns:
    nothing, void function
 */
void outputEntryFile(struct symbolNode *symbolTableHead, char *outputFileName){
struct symbolNode *currSymbol = symbolTableHead;
  FILE *fp = fopen(outputFileName, "a+");
  int isListEmpty = 1;

  while(symbolTableHead != NULL && currSymbol != NULL){
    if(currSymbol->isEntry){
      fprintf(fp, "%s 0%d\n", currSymbol->symbol, currSymbol->value);
      isListEmpty = 0;
    }
    
    currSymbol = currSymbol->next;
  }

  /* if there are no entries in the code, remove the created file */
  if(isListEmpty)
    remove(outputFileName);
}

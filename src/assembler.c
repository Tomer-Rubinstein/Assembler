#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "first_pass.h"
#include "second_pass.h"
#include "outputFiles.h"


char *seperateExtension(char *str, int getFilename){
    char *filename;
    char *extensionName;

    int i;
    int dotLocation = -1;

    for(i=strlen(str)-1; i >= 0; i--){
      if(str[i] == '.'){
        dotLocation = i;
        break;
      }
    }
    
    if(dotLocation == -1){
      fprintf(stdout, "No extension to file %s\n", str);
      return NULL;
    }
    
    extensionName = (char *) calloc(strlen(str), sizeof(char));
    filename = (char *) calloc(strlen(str), sizeof(char));

    memset(extensionName, '\0', strlen(extensionName));
    memset(filename, '\0', strlen(filename));
    
    for(i=dotLocation+1; i < strlen(str); i++)
      strncat(extensionName, &str[i], 1);
    
    
    for(i=0; i < dotLocation; i++)
      strncat(filename, &str[i], 1);

    return (getFilename) ? filename : extensionName;
}

int main(int argc, char *argv[]){
    int i;
    FILE *fptr;
    int isError = 0;

    char *filename;
    char *currFilename;

    for(i=1; i < argc; i++){
        if(strcmp(seperateExtension(argv[i], 0), "as") != 0){
            fprintf(stdout, "File %s is not a .as file\n", argv[i]);
            continue;
        }

        fptr = fopen(argv[i], "r");
        if(fptr == NULL){
            fprintf(stdout, "File %s was not found\n", argv[i]);
    		    fclose(fptr);
            continue;
        }


        if(firstScan(fptr, argv[i]))
            isError = 1;
        
        if(!isError && secondScan(fptr, argv[i]))
            isError = 1;
        
        if(!isError){
            filename = seperateExtension(argv[i], 1);
            currFilename = (char *) calloc(strlen(filename) + 30 ,sizeof(char));

            sprintf(currFilename, "%s.ob", filename);
            outputObjectFile(MemList, dataList, currFilename);
            currFilename[0] = '\0';

            sprintf(currFilename, "%s.ext", filename);
            outputExternFile(externals, currFilename);
            currFilename[0] = '\0';

            sprintf(currFilename, "%s.ent", filename);
            outputEntryFile(symbolTable, currFilename);
            memset(filename, '\0', strlen(filename));
            memset(currFilename, '\0', strlen(currFilename));
            free(currFilename);
        }

        fclose(fptr);


		freeMemList();
		freeExternals();
		freeDataList();
		freeSymbolTable();

        externals = NULL;
        MemList = NULL;
        symbolTable = NULL;
        dataList = NULL;
        
        isError = 0;
    }

    return 0;
}


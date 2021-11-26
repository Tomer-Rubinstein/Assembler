#include "./utils/scanUtils.h"
#include "./utils/scanUtilsProps.h"

struct NodeList * externals;
struct memNode * MemList;
struct symbolNode * symbolTable;

int secondScan(FILE *fptr, char *filename){

    struct NodeList * ptrExternals;
    struct memNode * ptrMemList = MemList;

    char *currCmd;
    char input[LINE_SIZE];

    int lineNum = 0;
    int commentIndicatorIndex = -1;
    int error_code = 0;

    int currCommandType = -1;
    char *commandName;

    rewind(fptr);

    loop:
    while(fgets(input, LINE_SIZE, fptr) != NULL){
        lineNum++;

        /*if the line is empty we should skip it*/
        if(strlen(strip(input)) == 0)
            goto loop;

        /* get index of comment, if there is, cut the comment out of the string */
        commentIndicatorIndex = getCommentIndicatorIndex(input);
        if(commentIndicatorIndex > 0){
            input[commentIndicatorIndex] = '\0';
        }else if(commentIndicatorIndex == 0){
            /* checking if 0 individually because if a line */
            /* is blank, the assembler skips the line. */
            goto loop;
        }

        currCmd = strip(input);
        commandName = firstWord(input);

        currCommandType = getCommandType(commandName);


        /* check if the first field in line is a symbol, if it is a symbol we should skip it */
        if(isSymbol(currCmd, lineNum)){
            while((*currCmd != ':') && (*currCmd != '\0') ){
                currCmd++;
            }
            currCmd++;
        }

        /*if the command of the line is storage command or .extern command we should skip it and read the next line */
        if(isStorageCommand(currCmd) || strstr(currCmd, ".extern")){
            goto loop;
        }else{
            if(strstr(currCmd, ".entry")){
                currCmd += strlen(".entry");
                if (currCmd == NULL) {
                    fprintf(stdout, "(%s) In line %d: .entry command should include a label name\n", filename, lineNum);
                    error_code = 1;
                }
                if(getSymbol(currCmd) != NULL){
                    /* if the symbol defined as external - report an error */
                    if(getSymbol(currCmd)->isExternal){
                        fprintf(stdout, "(%s) In line: %d the symbol %s cannot be both entry and external\n", filename, lineNum, currCmd);
                        error_code = 1;
                    }else{
                        /* no error found - we can apply the entry attribute for this symbol*/
                        getSymbol(currCmd)->isEntry = 1;
                    }
                }else{
                    fprintf(stdout, "(%s) In line %d: the symbol %s used for .entry is undefined\n", filename, lineNum, currCmd);
                    error_code = 1;
                }
                goto loop;

            }else{
                /* we are in an instruction line */
                if(currCommandType == J_Type){
                    if(!strcmp(currCmd, "stop")){ /* stop command had been already handled in first pass */
                        if(ptrMemList != NULL){
                            ptrMemList = ptrMemList->next;
                        }
                        goto loop;
                    }
                    if(!ptrMemList->command.commandJ.reg){
                        
                        char *label = currCmd;
                        if(strstr(currCmd, "call")){
                            /* it is a call command*/
                            label +=4;
                        }else if(strstr(currCmd, "jmp")){
                            /* it is a jmp command*/

                            label +=3;
                        }else if(strstr(currCmd, "la")){
                            /* it is a la command*/

                            label +=2;
                        }
                        if(getSymbol(label) != NULL){
                            (ptrMemList->command.commandJ).address = getSymbol(label)->value;
                            if(getSymbol(label)->isExternal){
                                if(externals == NULL){
                                    externals = calloc(1, sizeof(struct NodeList));
                                    externals->address = ptrMemList->address;
                                    externals->label = (char *) malloc(sizeof(char) * 32);
                                    memset(externals->label, '\0', 32);
                                    strcat(externals->label, label);
                                    ptrExternals = externals;
                                } else{
                                    addExternal(ptrExternals, ptrMemList->address, label);
                                    ptrExternals = ptrExternals->next;
                                }
                            }
                            ptrMemList->command.commandJ.address = getSymbol(label)->value;
                        }else{
                            /* wanted symbol isn't in the symbol table - report an error */
                            fprintf(stdout, "(%s) In line %d: symbol %s was not found\n", filename, lineNum, label);
                            error_code = 1;
                        }
                    }


                } else{
                    /* for the following I commands: beq, bne, blt, bgt (the others had been already handled in first pass) */
                    if(!strcmp(commandName, "beq") || !strcmp(commandName, "bne") || !strcmp(commandName, "blt") || !strcmp(commandName, "bgt") ){
                        char * label = currCmd;
                        label += 3; /*TODO: define it - the len of those commands: beq, bne, blt, bgt */
                        while((!isalpha(*label))||(*label == '$')){
                            label++;
                        }

                        /* if the symbol's attribute of this I command is external - report an error*/
                        if(getSymbol(label) != NULL){
                            if(getSymbol(label)->isExternal){
                                fprintf(stdout, "(%s) In line %d: the symbol %s can't be external because its is in one of those commands: beq, bne, blt, bgt \n", filename, lineNum, label);
                                error_code = 1;
                            }else{
                                ptrMemList->command.commandI.immed = getSymbol(label)->value - ptrMemList->address;
                            }
                        }else{
                            /* the wanted symbol isn't in symbol table - report an error */
                            fprintf(stdout, "(%s) In line %d: symbol %s was not found\n", filename, lineNum, label);
                            error_code = 1;
                        }
                    }
                }
            }

            ptrMemList = ptrMemList->next;
            currCmd[0]='\0';
        }
    }
    
    return error_code;
}

#include "scanUtils.h"

typedef enum {
    false,
    true
} boolean;

/*
    isSymbol() returns 1 if the given string is symbol in the first field of the line otherwise returns 0
    params:
        - char *str, the given string that we check if is symbol
        - int lineNum, the current line number in the given file
    returns:
        1 if the given string is symbol in the first field of the line otherwise returns 0
*/
int isSymbol(char *str, int lineNum){
    char *token;
    if(!strstr(str,":")){
        return 0;
    }
    token = strtok(str, ":");
    if(token){

        if(!isalpha(token[0])){
            fprintf(stdout, "In line: %d, symbols must start with an alphabetic character!", lineNum);
            return 0;
        }
        if(strlen(token) > 31){
            fprintf(stdout, "In line: %d, symbols' length should be at least 31 characters long", lineNum);
            return 0;
        }
        return 1;
    }
    return 0;
}

/*
    isSymbolDef() returns  NULL if the first field of the given string is not symbol otherwise the symbol in the first field of the line
    params:
        - char *str, the given string that we check if is symbol
    returns:
        NULL if the first field of the given string is not symbol otherwise the symbol in the first field of the line
*/
char *isSymbolDef(char *str){
    char *name = (char *) malloc((sizeof(char) * strlen(str)) + 1);
    int i;
    
	memset(name,'\0', strlen(str)+1);
    if(!strstr(str,":")){
	    free(name);
        return NULL;
	}

    for(i=0; i < strlen(str); i++){
        if(str[i] == ':')
            break;
        strncat(name, &str[i], 1);
    }

    return name;
}



int isStorageCommand(char *str){
    return (strstr(str, ".db") || strstr(str, ".dw") || strstr(str, ".dh") || strstr(str, ".asciz")) ? 1 : 0;
}

int isJCommand(char *str){
    return (strstr(str, "jmp") || strstr(str, "la") || strstr(str, "stop") || strstr(str, "call")) ? 1 : 0;
}

int isRCommand(char *str){
    return (strstr(str, "add") || strstr(str, "sub") || strstr(str, "and") || strstr(str, "or") || strstr(str, "nor") || strstr(str, "move") || strstr(str, "mvhi") || strstr(str, "mvlo")) ? 1 : 0;
}

/* is one of the following I commands: beq, bne, blt, bgt */
int isICommand(char *str){
    return (strstr(str, "beq") || strstr(str, "bne") || strstr(str, "blt") || strstr(str, "bgt")) ? 1 : 0;
}


/*
    firstWordLen() returns the first word encountered length
    params:
        - char *p, the string in which we search the first word encounterd and return its length
    returns:
        the first word encountered length
*/
int firstWordLen(char * p) {
    int i, count = 0;

    for (i = 0; p[i] != '\0' && !isspace(p[i]) && p[i] != ','; i++) {
        count++; /* we count how many characters does the first word has */
    }
    return count;
}


/*
    handleJCommand() returns the struct of the J command with the proper opcode,address and reg values
    params:
        - char * str, the given string that we need to take the values opcode,address and reg
    returns:
        the struct of the J command with the proper opcode,address and reg values
*/
commandJ handleJCommand(char * str){
    commandJ command;

    if(strstr(str, "stop")){
        command.opcode = 63;
        command.address = 0;
        command.reg = 0;
    }
    else {

        if (strstr(str, "jmp")) {
            command.opcode = 30;
            if (strstr(str, "$")) {
                /* jump to register */
                char *ptr;
                int counter = 1;
                command.reg = 1;
                for (ptr = str; *ptr != '\0' && *ptr != '$'; ptr++) {
                    counter++;
                }
                command.address = atoi(str + counter);
            } else {
                command.reg = 0;
            }
        } else {
            if (strstr(str, "la")) {
                command.opcode = 31;
                command.reg = 0;
            } else { /* it is a call command*/
                command.opcode = 32;
                command.reg = 0;
            }
        }
    }
    return command;
}


/*
    handleRCommand() returns the struct of the R command with the proper funct,opcode,rs and rd values
    params:
        - char * str, the given string that we need to take the values funct, opcode, rs and rd
    returns:
        the struct of the R command with the proper funct, opcode, rs and rd values
*/
commandR handleRCommand(char * str){
    commandR command;
    int commandLen;
    char *token;
    char *ptr;
    char delim[] = ",$";

    command.unused = 0;

    /* arithmetic and logic commands*/
    if( strstr(str, "add") || strstr(str, "sub") || strstr(str, "and") || strstr(str, "or") || strstr(str, "nor")){
        command.opcode = 0;
        commandLen = 3;
        if(strstr(str, "add"))
            command.funct = 1;
        if(strstr(str, "sub"))
            command.funct = 2;
        if(strstr(str, "and"))
            command.funct = 3;
        if(strstr(str, "nor"))
            command.funct = 5;
        if(strstr(str, "or")){
            commandLen = 2;
            command.funct = 4;
        }
    }
    else{ /* copying command*/
        command.opcode = 1;
        commandLen = 4;
        if(strstr(str, "move"))
            command.funct = 1;
        if(strstr(str, "mvhi"))
            command.funct = 2;
        if(strstr(str, "mvlo"))
            command.funct = 3;
    }
    
    ptr = (char *) malloc(sizeof(char) * (strlen(str)-commandLen));
    strcpy(ptr, str + commandLen +1);

    token = strtok(ptr, delim);
    command.rs = atoi(token);
    token = strtok(NULL, delim);
    
    if(!command.opcode){
        command.rt = atoi( token);
        token = strtok(NULL, delim);
        command.rd = atoi( token);
    }
    else{ /* the coping command require only 2 operands*/
        command.rd = atoi( token);
        command.rt = 0;
    }

	free(ptr);
    return command;
}

/*
    handleICommand() returns the struct of the I command with the proper opcode, rs, rt and immed values
    params:
        - char * str, the given string that we need to take the values opcode, rs, rt and immed
    returns:
        the struct of the I command with the proper opcode, rs, rt and immed values
*/
commandI handleICommand(char * str){
    commandI command;
    char *token ,*ptrLine;
    char delim[] = ",";
    char ptr[LINE_SIZE];

    ptrLine = str;

    if( strstr(str, "beq") || strstr(str, "bne") || strstr(str, "blt") || strstr(str, "bgt")){
        if(strstr(str, "bne"))
            command.opcode = 15;
        if(strstr(str, "beq"))
            command.opcode = 16;
        if(strstr(str, "blt"))
            command.opcode = 17;
        if(strstr(str, "bgt"))
            command.opcode = 18;
    }
    else{ /* arithmetic and logic commands and loading and saving in memory commands*/
        if(strstr(str, "addi") || strstr(str, "subi") || strstr(str, "andi") || strstr(str, "nori")){
            if(strstr(str, "addi"))
                command.opcode = 10;
            if(strstr(str, "subi"))
                command.opcode = 11;
            if(strstr(str, "andi"))
                command.opcode = 12;
            if(strstr(str, "nori"))
                command.opcode = 14;
        } else{
            if(strstr(str, "ori")){
                command.opcode = 13;
            }
            if(strstr(str, "lb"))
                command.opcode = 19;
            if(strstr(str, "sb"))
                command.opcode = 20;
            if(strstr(str, "lw"))
                command.opcode = 21;
            if(strstr(str, "sw"))
                command.opcode = 22;
            if(strstr(str, "lh"))
                command.opcode = 23;
            if(strstr(str, "sh"))
                command.opcode = 24;
        }
    }
    while(*ptrLine != '$'){
        ptrLine++;
    }
    ptrLine++;
    strcpy(ptr, ptrLine);
    token = strtok(ptr, delim);
    command.rs = atoi(token);
    if (!(strstr(str, "beq") || strstr(str, "bne") || strstr(str, "blt") || strstr(str, "bgt"))) {
        token = strtok(NULL, delim);
        command.immed = atoi(token);

    }
    token = strtok(NULL, delim);
    command.rt = atoi( token + 1); /* we dont want the first char of the last token which is - '$'*/

    return command;
}

/*
    getCommandType() returns the value type of the command 
    params:
        - char * str, the given string that has the command
    returns:
        the value type of the command either R_Type / I_Type / J_Type
*/
int getCommandType(char *str){
    str = strip(str);
    if(!strcmp(str, "add") || !strcmp(str, "sub") || !strcmp(str, "and") || !strcmp(str, "or") || !strcmp(str, "nor") || !strcmp(str, "move") || !strcmp(str, "mvhi") || !strcmp(str, "mvlo"))
        return R_Type;
    
    if(!strcmp(str, "addi") || !strcmp(str, "subi") || !strcmp(str, "andi") || !strcmp(str, "ori") || !strcmp(str, "nori")
        || !strcmp(str, "bne") || !strcmp(str, "beq") || !strcmp(str, "blt") || !strcmp(str, "bgt") || !strcmp(str, "lb") 
        || !strcmp(str, "sb") || !strcmp(str, "lw") || !strcmp(str, "sw") || !strcmp(str, "lh") || !strcmp(str, "sh") )
            return I_Type;

    if(!strcmp(str, "jmp") || !strcmp(str, "la") || !strcmp(str, "stop") || !strcmp(str, "call"))
        return J_Type;
    return -1;
}

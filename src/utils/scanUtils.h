#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "stringOperations.h"
#include "essentials.h"
#include "structToBinary.h"
#include "symbolTable.h"
#include "errorHandling.h"

#include "../LabelNode.h"
#include "../MemNode.h"
#include "../dataList.h"

#define LINE_SIZE 80

#define R_Type 1
#define I_Type 2
#define J_Type 3

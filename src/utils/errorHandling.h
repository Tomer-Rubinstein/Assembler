int validateRegister(char *str);
int missingComma(char *cmd, int expectedCommas);
int validateOperands(char *cmd, int expectedParams, int cmdType, int lineNum, char *filename);
int validateDataInstruction(char *instruction, int lineNum, int minValue, int maxValue, char *filename);
int validateLine(char *line, int lineNum, char *filename);

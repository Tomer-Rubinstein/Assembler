/*  commandTypes.h builds the linked list about every command with the needed information about it,
    it is defined as a struct called GlobalCommand and it's fields are:
      * commandType, which type of command is it? (R, I or J)
      * params, how many operands does a command need
      * cmdName, the name of the current command
      * nextCmd, the next command in the list
*/

#define R_Type 1
#define I_Type 2
#define J_Type 3

typedef struct GlobalCmd {
    int commandType;
    int params;
    char *cmdName;
    struct GlobalCmd *nextCmd;
} GlobalCommand;


/* R Commands */
GlobalCommand add_cmd = {
  R_Type, /* commandType */
  3, /* params */
  "add", /* cmdName */
};

GlobalCommand sub_cmd = {
  R_Type, /* commandType */
  3, /* params */
  "sub", /* cmdName */
  &add_cmd
};

GlobalCommand and_cmd = {
  R_Type, /* commandType */
  3, /* params */
  "and", /* cmdName */
  &sub_cmd
};

GlobalCommand or_cmd = {
  R_Type, /* commandType */
  3, /* params */
  "or", /* cmdName */
  &and_cmd
};

GlobalCommand nor_cmd = {
  R_Type, /* commandType */
  3, /* params */
  "nor", /* cmdName */
  &or_cmd
};

GlobalCommand move_cmd = {
  R_Type, /* commandType */
  2, /* params */
  "move", /* cmdName */
  &nor_cmd
};

GlobalCommand mvhi_cmd = {
  R_Type, /* commandType */
  2, /* params */
  "mvhi", /* cmdName */
  &move_cmd
};

GlobalCommand mvlo_cmd = {
  R_Type, /* commandType */
  2, /* params */
  "mvlo", /* cmdName */
  &mvhi_cmd,
};

/* I Commands */

GlobalCommand addi_cmd = {
  I_Type, /* commandType */
  3, /* params */
  "addi", /* cmdName */
  &mvlo_cmd
};

GlobalCommand subi_cmd = {
  I_Type, /* commandType */
  3, /* params */
  "subi", /* cmdName */
  &addi_cmd
};

GlobalCommand andi_cmd = {
  I_Type, /* commandType */
  3, /* params */
  "andi", /* cmdName */
  &subi_cmd
};

GlobalCommand ori_cmd = {
  I_Type, /* commandType */
  3, /* params */
  "ori", /* cmdName */
  &andi_cmd
};

GlobalCommand nori_cmd = {
  I_Type, /* commandType */
  3, /* params */
  "nori", /* cmdName */
  &ori_cmd
};

GlobalCommand bne_cmd = {
  I_Type, /* commandType */
  3, /* params */
  "bne", /* cmdName */
  &nori_cmd
};

GlobalCommand beq_cmd = {
  I_Type, /* commandType */
  3, /* params */
  "beq", /* cmdName */
  &bne_cmd
};

GlobalCommand blt_cmd = {
  I_Type, /* commandType */
  3, /* params */
  "blt", /* cmdName */
  &beq_cmd,
};

GlobalCommand bgt_cmd = {
  I_Type, /* commandType */
  3, /* params */
  "bgt", /* cmdName */
  &blt_cmd,
};

GlobalCommand lb_cmd = {
  I_Type, /* commandType */
  3, /* params */
  "lb", /* cmdName */
  &bgt_cmd
};

GlobalCommand sb_cmd = {
  I_Type, /* commandType */
  3, /* params */
  "sb", /* cmdName */
  &lb_cmd,
};

GlobalCommand lw_cmd = {
  I_Type, /* commandType */
  3, /* params */
  "lw", /* cmdName */
  &sb_cmd,
};

GlobalCommand sw_cmd = {
  I_Type, /* commandType */
  3, /* params */
  "sw", /* cmdName */
  &lw_cmd,
};

GlobalCommand lh_cmd = {
  I_Type, /* commandType */
  3, /* params */
  "lh", /* cmdName */
  &sw_cmd,
};

GlobalCommand sh_cmd = {
  I_Type, /* commandType */
  3, /* params */
  "sh", /* cmdName */
  &lh_cmd,
};

/* J Commands */

GlobalCommand jmp_cmd = {
  J_Type, /* commandType */
  1, /* params */
  "jmp", /* cmdName */
  &sh_cmd,
};

GlobalCommand la_cmd = {
  J_Type, /* commandType */
  1, /* params */
  "la", /* cmdName */
  &jmp_cmd
};

GlobalCommand call_cmd = {
  J_Type, /* commandType */
  1, /* params */
  "call", /* cmdName */
  &la_cmd,
};

GlobalCommand stop_cmd = {
  J_Type, /* commandType */
  0, /* params */
  "stop", /* cmdName */
  &call_cmd,
};

GlobalCommand *CommandsHeadNode = &stop_cmd;


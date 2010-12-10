#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

/* Constants for types of variables to check */
#define TYPE_SRC    0
#define TYPE_DES    1
#define TYPE_REG    2
#define TYPE_CONST8 3
#define TYPE_CONST4 4

void execute (unsigned short int ins);

#endif // INSTRUCTIONS_H

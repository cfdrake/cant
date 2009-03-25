#define TYPE_SRC    0
#define TYPE_DES    1
#define TYPE_REG    2
#define TYPE_CONST8 3
#define TYPE_CONST4 4

/*
 * Determines if the specified arg is of a type (des, reg, etc.)
 */
void
check (int arg, int type, int param)
{
  /* Determine what type to check for */
  switch (type)
    {
      case TYPE_SRC:
        if (!(arg >= 0 && arg < REGISTERS_LEN))
          {
            printf ("Error: expected SRC for param %i.\n", param);
            sys_dump (0x0);
            sys_halt (0x0);
          }
      case TYPE_DES:
        if (!(arg >= 2 && arg < REGISTERS_LEN))
          {
            printf ("Error: expected DES for param %i.\n", param);
            sys_dump (0x0);
            sys_halt (0x0);
          }
      case TYPE_REG:
        if (!(arg >= 0 && arg < REGISTERS_LEN))
          {
            printf ("Error: expected REG for param %i.\n", param);
            sys_dump (0x0);
            sys_halt (0x0);
          }
      case TYPE_CONST8:
        break;
      case TYPE_CONST4:
        if (!(arg >= 0 && arg <= 15))
          {
            printf ("Error: expected CONST4 for param %i.\n", param);
            sys_dump (0x0);
            sys_halt (0x0);
          }
    }
}

void
_add (int des, int src1, int src2)
{
  check (des, TYPE_DES, 1);
  check (src1, TYPE_SRC, 2);
  check (src2, TYPE_SRC, 3);
  printf ("add\n");
}

void
_sub (int des, int src1, int src2)
{
  check (des, TYPE_DES, 1);
  check (src1, TYPE_SRC, 2);
  check (src2, TYPE_SRC, 3);
  printf ("sub\n");
}

void
_mul (int des, int src1, int src2)
{
  check (des, TYPE_DES, 1);
  check (src1, TYPE_SRC, 2);
  check (src2, TYPE_SRC, 3);
  printf ("mul\n");
}

void
_div (int des, int src1, int src2)
{
  check (des, TYPE_DES, 1);
  check (src1, TYPE_SRC, 2);
  check (src2, TYPE_SRC, 3);
  printf ("div\n");
}

void
_beq (int reg1, int reg2, int reg3)
{
  check (reg1, TYPE_REG, 1);
  check (reg2, TYPE_REG, 2);
  check (reg3, TYPE_REG, 3);
  printf ("beq\n");
}

void
_bgt (int reg1, int reg2, int reg3)
{
  check (reg1, TYPE_REG, 1);
  check (reg2, TYPE_REG, 2);
  check (reg3, TYPE_REG, 3);
  printf ("bgt\n");
}

void
_ld (int des, int src1, int const4)
{
  check (des, TYPE_DES, 1);
  check (src1, TYPE_SRC, 2);
  check (const4, TYPE_CONST4, 3);
  printf ("ld\n"); 
}

void
_st (int reg, int src1, int const4)
{
  check (reg, TYPE_REG, 1);
  check (src1, TYPE_SRC, 2);
  check (const4, TYPE_CONST4, 3);
  printf ("st\n");
}

void
_lc (int des, int const8)
{
  check (des, TYPE_DES, 1);
  check (const8, TYPE_CONST8, 2);
  printf ("lc\n");
}

void
_jmp (int reg, int const8)
{
  check (reg, TYPE_REG, 1);
  check (const8, TYPE_CONST8, 2);
  printf ("jmp\n");
}

void
_inc (int des, int const8)
{
  check (des, TYPE_DES, 1);
  check (const8, TYPE_CONST8, 2);
  printf ("inc\n");
}

void
_sys (int reg, int const8)
{
  check (reg, TYPE_REG, 1);
  check (const8, TYPE_CONST8, 2);
  sys_call (reg, const8);
}

void
execute (unsigned short int ins)
{
  /* seperate instruction into parts */
  short int a0 = (ins >> 12) % 16;
  short int a1 = (ins >> 8) % 16;
  short int a2 = (ins >> 4) % 16;
  short int a3 = (ins >> 0) % 16;

  /* Run associated instruction */
  switch(a0)
    {
      case 0x0:
        _add (a1, a2, a3);
        break;
      case 0x1:
        _sub (a1, a2, a3);
        break;
      case 0x2:
        _mul (a1, a2, a3);
        break;
      case 0x3:
        _div (a1, a2, a3);
        break;
      case 0x6:
        _beq (a1, a2, a3);
        break;
      case 0x7:
        _bgt (a1, a2, a3);
        break;
      case 0x8:
        _ld (a1, a2, a3);
        break;
      case 0x9:
        _st (a1, a2, a3);
        break;
      case 0xa:
        _lc (a1, a3);
        break;
      case 0xb:
        _jmp (a1, a3);
        break;
      case 0xc:
        _inc (a1, a3);
        break;
      case 0xf:
        _sys (a1, a3);
        break;
      default:
        printf ("Error: invalid opcode %#x.\n", a0);
        sys_dump (0x0);
        sys_halt (0x0);
    }
}

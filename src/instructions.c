void
_add (int reg1, int reg2, int reg3)
{
  printf ("add\n");
}

void
_sub (int reg1, int reg2, int reg3)
{
  printf ("sub\n");
}

void
_mul (int reg1, int reg2, int reg3)
{
  printf ("mul\n");
}

void
_div (int reg1, int reg2, int reg3)
{
  printf ("div\n");
}

void
_beq (int reg1, int reg2, int reg3)
{
  printf ("beq\n");
}

void
_bgt (int reg1, int reg2, int reg3)
{
  printf ("bgt\n");
}

void
_ld (int reg, int src, int pos)
{
  printf ("ld\n"); 
}

void
_st (int reg, int src, int val)
{
  printf ("st\n");
}

void
_lc (int reg, int val)
{
  printf ("lc\n");
}

void
_jmp (int reg, int newpc)
{
  printf ("jmp\n");
}

void
_inc (int reg, int val)
{
  printf ("inc\n");
}

void
_sys (int reg, int code)
{
  printf ("sys\n");
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
        _lc (a1, a2);
        break;
      case 0xb:
        _jmp (a1, a2);
        break;
      case 0xc:
        _inc (a1, a2);
        break;
      case 0xf:
        _sys (a1, a2);
        break;
      default:
        printf ("Error: invalid opcode %#x.\n", a0);
        sys_dump (0x0);
        sys_halt (0x0);
    }
}

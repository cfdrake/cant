#ifndef SYS_H
#define SYS_H

void sys_halt(int reg);
void sys_dump(int reg);
void sys_put_int(int reg);
void sys_put_char(int reg);
void sys_put_str(int reg);
void sys_get_int(int reg);
void sys_get_char(int reg);
void sys_call(int reg, int code);

#endif // SYS_H

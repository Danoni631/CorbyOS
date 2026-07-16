struct InterruptRegisters;

void init_keyboard();
void keyboard_handler(struct InterruptRegisters *regs);
void handle_char(int scan);
void kb_state(int state);

extern const char* KeyMap[][2];
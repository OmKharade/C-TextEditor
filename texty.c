#include <ctype.h>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>

struct termios default_termios;

void disableRawMode() {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &default_termios);
}

void enableRawMode() {
    tcgetattr(STDIN_FILENO, &default_termios);
    atexit(disableRawMode);

    struct termios raw = default_termios;
    raw.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);
    raw.c_iflag &= ~(ICRNL | IXON);
    raw.c_oflag &= ~(OPOST);
    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    raw.c_cflag |= (CS8);

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
  }

int main(){
    enableRawMode();

    char c;
    while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q'){
      if (iscntrl(c)) {
        printf("%d\r\n", c);
      } else {
        printf("%d ('%c')\r\n", c, c);
      }
    }
    return 5;
}
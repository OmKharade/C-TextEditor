/* includes */

#include <ctype.h>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <errno.h>

#define CTRL(k) ((k) & 0x1f)

/* data */

struct termios default_termios;

/* terminal */

void die(const char *s){
    write(STDOUT_FILENO, "\x1b[2J", 4);
    write(STDOUT_FILENO, "\x1b[H", 3);

    perror(s);
    exit(1);
}

void disableRawMode() {
    if(tcsetattr(STDIN_FILENO, TCSAFLUSH, &default_termios) == -1)
      die("tcsetattr");
}

void enableRawMode() {
    if(tcgetattr(STDIN_FILENO, &default_termios) == -1)
      die("tcgetattr");
    atexit(disableRawMode);

    struct termios raw = default_termios;
    raw.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);
    raw.c_iflag &= ~(ICRNL | IXON);
    raw.c_oflag &= ~(OPOST);
    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    raw.c_cflag |= (CS8);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 1;

    if(tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
      die("tcgetattr");
  }

char editorReadKey(){
  int nread;
  char c;
  while((nread = read(STDIN_FILENO, &c, 1)) != 1){
    if(nread == -1 && errno != EAGAIN)
      die("read");
  }
  return c;
}

/* input */

void editorProcessKeypresses(){
  char c = editorReadKey();

  switch (c){
    case CTRL('q'):
      write(STDOUT_FILENO, "\x1b[2J", 4);
      write(STDOUT_FILENO, "\x1b[H", 3);
      exit(0);
      break;
  }
}


/* output */

void editorRefreshScreen(){
  write(STDOUT_FILENO, "\x1b[2J", 4);
  write(STDOUT_FILENO, "\x1b[H", 3);
}

/* init */

int main(){
    enableRawMode();

    while (1) {
      editorRefreshScreen();
      editorProcessKeypresses();
    }
    return 0;
}
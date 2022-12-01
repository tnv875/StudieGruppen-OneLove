#include <stdio.h>

int main(int argc, char* argv[]) {
  if (argc == 2){
    for (int i = 0; i < argc; i++) {
      puts(argv[1]);
    }
  } else {
    printf("Wrong number of arguments\n");
  }
}

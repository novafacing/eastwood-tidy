#include <other.h>

int globalval;

#define H 1

#define goodbye ("goodbye")

/* foo */

int foo () {
    int looooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooong;
  int a = 5;
  if (a == 5){
    a+=3;
  }
  for (int i = 0;i<a;i++) { 
    if (i + 1 != a + 1) {
      break;
    }
  }
  goto end;

  end:
  return 0;

} /* foo() */

/*
 * bar
 */

int bar(char * a,
       char *b) {
  do {
    *a = *b;
  }
  while (*(a++) != '\0'); // Bad comment
}
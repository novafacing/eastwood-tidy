int a(int a,
      int b) {
  printf("This is ok\n");
}

int main() {
  if (1 &&
    2) {
    printf("Good indentation\n");
  }
  else {
    printf("Good indentation\n");
  }
  while (1 &&
    2) {
    printf("Good indentation\n");
  }
  do {
    printf("Good indentation\n");
  } while (1);

  for (int i = 0;
    i < 10; i++) {
    printf("Good indentation\n");
  }
  // BAD INDENTATION -- BRACE LINES
  if (1 &&
    2) 
    {
    printf("Good indentation\n");
  }
  else 
  {
    printf("Good indentation\n");
  }
  while (1 &&
    2) 
    {
    printf("Good indentation\n");
  }
  do {
    printf("Good indentation\n");
  } while (1);

  for (int i = 0;
    i < 10; i++) {
    printf("Good indentation\n");

  }
  // BAD INDENTATION -- Incorrect indent amount
  if (1 &&
  2) {
     printf("Good indentation\n");
  }
  else {
      printf("Good indentation\n");
  }
  while (1 &&
  2) {
  printf("Good indentation\n");
  }
  do {
      printf("Good indentation\n");
  } while (1);

  for (int i = 0; (i > 10)
  || (i < 10); i++) {
     printf("Good indentation\n");

  }
  if (1&&2||3) {
    printf("Good identation but fails before/after ws\n");
  }
}
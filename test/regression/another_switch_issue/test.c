int main() {
  int x = 10;
  switch (x) {
    case 10:
      printf("x is 10\n");
      break;
    case 20:
      printf("x is 20\n");
      break;
    case 30: {
      printf("x is 30\n");
      break;
    }
    case 40: {
      printf("x is 40\n");
    }
      break;
    default:
      printf("x is neither 10 nor 20\n");
      break;
  }
}
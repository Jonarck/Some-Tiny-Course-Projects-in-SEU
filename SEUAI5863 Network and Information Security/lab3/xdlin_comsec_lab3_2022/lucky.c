#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>

char goodPassword() {
  int good='N';
  char Password[100]; // Memory storage for the password
  gets(Password); // Get input from keyboard

  return (char)good;
}

int main() {
  struct timeval t;

  printf("Enter your password:");

  if (goodPassword() == 'Y') {
    gettimeofday(&t, 0);
    srand((unsigned int) t.tv_usec);
    printf("Your lucky number today is %d!\n", rand()%100);
  }
  else {
    printf("No lucky numbers for you today.\n");
    exit(-1);
  }

  return 0;
}


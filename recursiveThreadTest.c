#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/time.h>
#include "thpool.h"


void levels(int level) {
  level--;
  for (int i = 0; i < level; i++) {
    levels(level);
  }
}

int main() {
  int level = 14;
  //time stuff
  struct timeval t1, t2;
  double elapsedTime1, elapsedTime2;

  printf("Starting none threaded\n");

  // start timer
  gettimeofday(&t1, NULL);

  levels(level);

  // stop timer
  gettimeofday(&t2, NULL);
  elapsedTime1 = (t2.tv_sec - t1.tv_sec);



  threadpool thpool = thpool_init(2);

  // start timer
  gettimeofday(&t1, NULL);
  level--;
  for (int i = 0; i < level; i++) {
    thpool_add_work(thpool, (void*)levels, level);
  }

  thpool_wait(thpool);

  // stop timer
  gettimeofday(&t2, NULL);
  elapsedTime2 = (t2.tv_sec - t1.tv_sec);

  printf("done\n");
  printf("None threaded seconds: %f\n", elapsedTime1);
  printf("Threaded seconds: %f\n", elapsedTime2);
}

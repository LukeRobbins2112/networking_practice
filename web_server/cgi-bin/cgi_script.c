#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLINE 1024

int main(){

  char* buf, *p;
  char content[MAXLINE];

  char arg1[MAXLINE], arg2[MAXLINE];
  int n1, n2;

  if ((buf = getenv("QUERY_STRING")) != NULL){
    p = strchr(buf, '&');
    *p = '\0';

    strcpy(arg1, buf);
    strcpy(arg2, p+1);
    n1 = atoi(arg1);
    n2 = atoi(arg2);
  } else {
    printf("Err getting query string");
  }

  // Build response
  sprintf(content, "Welcome to add.com: ");
  sprintf(content, "%sThe answer is: %d + %d = %d\r\n<p>", content, n1, n2, n1 + n2);

  printf("Content-length: %d\r\n", (int)strlen(content));
  printf("Content-type: text/html\r\n\r\n");
  printf("%s", content);
  fflush(stdout);
  exit(0);

  return 0;

}

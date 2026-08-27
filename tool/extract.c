/*
** Extract a range of bytes from a file.
**
** Usage:
**
**    extract FILENAME OFFSET AMOUNT
**
** The bytes are written to standard output.
*/
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>

int main(int argc, char **argv){
  FILE *f;
  char *zBuf;
  int ofst;
  int n;
  size_t got;
  char *endptr;
  long ofst_long;
  long n_long;

  if( argc!=4 ){
    fprintf(stderr, "Usage: %s FILENAME OFFSET AMOUNT\n", *argv);
    return 1;
  }
  f = fopen(argv[1], "rb");
  if( f==0 ){
    fprintf(stderr, "cannot open \"%s\"\n", argv[1]);
    return 1;
  }
  errno = 0;
  ofst_long = strtol(argv[2], &endptr, 10);
  if( errno==ERANGE || *endptr!='\0' || ofst_long<0 || ofst_long>INT_MAX ){
    fprintf(stderr, "invalid offset: %s\n", argv[2]);
    return 1;
  }
  ofst = (int)ofst_long;
  errno = 0;
  n_long = strtol(argv[3], &endptr, 10);
  if( errno==ERANGE || *endptr!='\0' || n_long<0 || n_long>INT_MAX ){
    fprintf(stderr, "invalid amount: %s\n", argv[3]);
    return 1;
  }
  n = (int)n_long;
  zBuf = malloc( n );
  if( zBuf==0 ){
    fprintf(stderr, "out of memory\n");
    return 1;
  }
  fseek(f, ofst, SEEK_SET);
  got = fread(zBuf, 1, n, f);
  fclose(f);
  if( got<n ){
    fprintf(stderr, "got only %d of %d bytes\n", got, n);
    return 1;
  }else{
    fwrite(zBuf, 1, n, stdout);
  }
  return 0;
}

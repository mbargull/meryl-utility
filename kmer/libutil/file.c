#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include "file.h"


int
fileExists(char *path) {
  struct stat s;

  return(stat(path, &s) == 0);
}


off_t
sizeOfFile(char *path) {
  struct stat s;

  errno = 0;
  if (stat(path, &s) != 0) {
    fprintf(stderr, "Couldn't stat '%s': %s\n", path, strerror(errno));
    exit(1);
  }

  return(s.st_size);
}


int
isHuman(FILE *F) {
  return(isatty(fileno(F)));
}


#ifdef __alpha
unsigned long __sbrk_override = 1;  //  See malloc(3) for details.

#define MMAPFLAGS    (MAP_FILE | MAP_VARIABLE | MAP_SHARED)
#endif

#ifdef __linux
#define MMAPFLAGS    (MAP_FILE | MAP_SHARED)
#endif

#ifdef __FreeBSD__
#define MMAPFLAGS    (MAP_FILE | MAP_SHARED)
#endif

#ifdef __sun
#define MMAPFLAGS    (MAP_SHARED)
#endif


//  Not everyone has O_LARGEFILE
#ifndef O_LARGEFILE
#define O_LARGEFILE 0
#endif


void*
mapFile(char *filename, size_t *length, char mode) {
  void        *ptr = 0L;
  struct stat  sb;
  int          f;
  int          openMode = O_RDONLY | O_LARGEFILE;
  int          mapMode  = O_RDWR   | O_LARGEFILE;

  switch (mode) {
    case 'r':
      openMode = O_RDONLY | O_LARGEFILE;
      mapMode  = PROT_READ;
      break;
    case 'w':
      openMode = O_RDWR   | O_LARGEFILE;
      mapMode  = PROT_READ | PROT_WRITE;
      break;
    default:
      fprintf(stderr, "Invalid mode to mapFile; must be 'r' or 'w'\n");
      exit(1);
      break;
  }

  errno = 0;
  f = open(filename, openMode);
  if (errno) {
    fprintf(stderr, "Couldn't open() '%s'\n%s\n", filename, strerror(errno));
    exit(1);
  }

  fstat(f, &sb);
  if (errno) {
    fprintf(stderr, "Couldn't fstat() '%s'\n%s\n", filename, strerror(errno));
    exit(1);
  }

  *length = sb.st_size;

  ptr = mmap(0L, *length, mapMode, MMAPFLAGS, f, 0);
  if (errno) {
    fprintf(stderr, "Couldn't mmap() '%s'\n%s\n", filename, strerror(errno));
    exit(1);
  }

  close(f);

  return(ptr);
}



void
unmapFile(void *addr, size_t length) {
#ifdef __sun
  //  This might work in general, but sun definitely needs the cast.
  //
  (void)munmap((caddr_t)addr, length);
#else
  (void)munmap(addr, length);
#endif
}



#ifdef MAIN
int
main(int argc, char **argv) {

  size_t   lw;
  char    *ww = mapFile("mmap.c.test", &lw, 'w');

  for (int a=0; a<256; a++)
    ww[a] = a;

  unmapFile(ww, lw);

  //

  ww = mapFile("mmap.c.test", &lw, 'r');

  fprintf(stderr, "opened with length %d\n", lw);
  for (int a=0; a<256; a++)
    fprintf(stderr, "%3d ", ww[a]);

  unmapFile(ww, lw);


}
#endif

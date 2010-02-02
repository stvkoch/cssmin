
// 
// cssmin.c
// 
// (c) 2009 TJ Holowaychuk <tj@vision-media.ca> (MIT Licensed)
//

#include "cssmin.h"

#define last l
#define undo (ungetc(n, stream), n)
#define next (n = fgetc(stream), undo)
#define buffer (buf[bufpos++] = c)
#define end (buf[bufpos] = '\0')
#define out (++nchars_out, putchar(c))
#define input (l = c, c = fgetc(stream))
#define stat(S, ...) fprintf(stderr, S "\n", __VA_ARGS__);
#define option(S) (strcmp(argv[1], S) == 0)

static int 
minify(FILE *stream) {
  clock_t start = clock();
  char c, l, n, buf[CSSMIN_BUFFER_MAX];
  int bufpos = 0, inblock = 0, nchars = 0, nchars_out = 0;
start:
  switch (++nchars, input) {
    case '\n': goto start;
    case '{' : goto block;
    case '}' : goto blockend;
    case ' ' : goto space;
    case '"' : goto string;
    case EOF : goto finish;
    default  : out;
  }
  goto start;
block:
  out; inblock = 1;
  goto start;
blockend:
  out; inblock = 0;
  goto start;
string:
  out;
  if (input != '"') goto string;
  else out;
  goto start;
space:
  if ((isalpha(last) && isalpha(next)) || // foo bar
      (isalpha(last) && isdigit(next)) || // 1px 0
      (isalpha(last) && next == '"') ||   // 1px "solid"
      (last == '"' && !ispunct(next)) ||  // "solid" red
      (!inblock && next == ':') ||        // foo :last-child
      isdigit(last) ||                    // 0 0
      (next == '*' || last == '*')) out;  // foo * bar
  goto start;
finish:
  stat("Compressed : %0.1f%%", nchars_out / nchars * 100);
  stat("Duration   : %0.5fms", (float) (clock() - start) / CLOCKS_PER_SEC);
  return 0;
}

static int
usage() {
  printf("cssmin: %s\n"
         "usage: cssmin < in.css > out.min.css\n", CSSMIN_VERSION);
  exit(0);
}

int 
main(int argc, char **argv) {
  if (argc > 1)
    if (option("--help")) usage();
    else if (option("--version")) puts(CSSMIN_VERSION), exit(0);
  return minify(stdin);
}
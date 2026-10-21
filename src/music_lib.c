#include <string.h>

int alpha_sort(const void *a, const void *b) {
  const char *music_a = *(const char **)a;
  const char *music_b = *(const char **)b;

  return strcmp(music_a, music_b);
}

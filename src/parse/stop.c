#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "../uthash.h"
#include "stop.h"

int stoplist_init(struct stoplist **stopwords, char *wordlist) {
  char buffer[256];

  FILE *input = fopen(wordlist, "r");
  if (!input) {
    printf("Error opening stopwords file: %s\n", wordlist);
    return 1;
  }

  while(fgets(buffer, sizeof(buffer), input) != NULL) {
    for (int i = 0; i < sizeof(buffer); i++) {
      if (buffer[i] == '\n') {
        struct stoplist *word;
        word = malloc(sizeof(struct stoplist));
        strncpy(word->key, buffer, strlen(buffer) - 1);

        HASH_ADD_STR(*stopwords, key, word);
        break;
      }
    }
  }

  fclose(input);
  return 0;
}

int stoplist_find(struct stoplist **stopwords, char *keyname) {
  struct stoplist *word;
  HASH_FIND_STR(*stopwords, keyname, word);
  if (word == NULL) {
    return 0;
  } else {
    return 1;
  }
}
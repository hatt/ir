#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "../common.h"

// Initialise a lexicon, optionally with a source file
void lexicon_init(struct trie *lexicon, FILE *input) {
  trie_init(&lexicon);

  // Import lexicon from file
  if (input != NULL) {
    while((!feof(input)) && (!ferror(input))) {
      char *key;
      uint32_t length, value;

      // Length header is key+value size from end of length field
      // Key length is length - sizeof(value), ie length - 4 bytes
      fread(&length, 4, 1, input);
      fread(key, length - 4, 1, input);
      fread(&value, 4, 1, input);

      lexicon_add(lexicon, key, value);
    }
  }

  return;
}

// Add a token to the lexicon
void lexicon_add(struct trie *lexicon, char *key, uint32_t value) {
  trie_add(&lexicon, key, value);
  return;
}

uint32_t lexicon_find(struct trie *lexicon, char *key) {
  struct trie *term = trie_find(lexicon, key);
  return term->value;
}
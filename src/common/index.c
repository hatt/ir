#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "../common.h"
#include "../uthash.h"

uint32_t index_token(FILE *index, struct tokenlist *token) {
  struct token *document, *tmp;
  uint32_t offset = ftell(index);
  uint32_t length = HASH_COUNT(token);

  // Write length header to index
  fwrite(&length, 4, 1, index);

  // Iterate documents for token
  HASH_ITER(hh, token->document, document, tmp) {
    // Write id,count pairs to index
    fwrite(&document->id, 4, 1, index);
    fwrite(&document->count, 4, 1, index);

    // Cleanup
    HASH_DEL(token->document, document);
    free(document);
  }

  // Return index file pointer offset for token
  return offset;
}

void index_find(FILE *index, uint32_t offset, struct tokenlist *tokens) {}
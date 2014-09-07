#import <stdio.h>
#import <stdlib.h>
#import <stdint.h>

#include "../common.h"
#include "../uthash.h"

void query(struct trie *lexicon, FILE *index, struct mapping **map, char *term);

int main(int argc, char **argv) {
  char *term;

  // error if less than 4 args provided (0 indexed)
  if (argc < 5) {
    printf("Invalid number of arguments passed!\n");
    printf("Syntax is %s: <lexicon> <index> <map> [<query 1>...<query n>]\n", argv[0]);
    return 1;
  }

  // Verify files can open
  FILE *lexfile = fopen(argv[1], "r");
  FILE *invlist = fopen(argv[2], "rb");
  FILE *mapfile = fopen(argv[3], "r");

  // The number of query terms is 3 less than total args
  char *queries[argc - 3];
  for (int i = 4; i < argc; i++) {
    queries[i - 4] = argv[i];
  }

  // Construct root lexicon
  struct trie *lexicon = NULL;
  lexicon_init(lexicon, lexfile);
  fclose(lexfile);

  // Load map into memory
  struct mapping *map = NULL;
  map_init(mapfile, &map);
  fclose(mapfile);
  
  // For each query word, search in index
  for (int i = 0; i < sizeof(*queries); i++) {
    // Apply tokenizer rules to each query
    while((term = strsep(&queries[i], "\\,.?/:;=-()[]{}\'\" \n\t")) != NULL) {
      query(lexicon, invlist, &map, term);
    }
  }

  // Cleanup
  free(map);
  free(lexicon);
  fclose(invlist);

  return 0;
}

void query(struct trie *lexicon, FILE *index, struct mapping **map, char *term) {
  struct token *results = malloc(sizeof(struct token));
  struct token *s, *tmp;

  uint32_t ptr = lexicon_find(lexicon, term);
  fseek(index, ptr, SEEK_SET);

  index_find(index, ptr, &results);

  printf("%s\n", term);
  printf("%u\n", HASH_COUNT(results));
  HASH_ITER(hh, results, s, tmp) {
    printf("%s, %u\n", unmap_document(map, s->id), s->count);
  }
}
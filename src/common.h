#include "uthash.h"

/*
 * ID    - the logical ID of the document the token appears in
 * Count - the number of appearances of the token in the current document
 */
struct token {
  uint32_t id;
  uint32_t count;
  UT_hash_handle hh;
};

/*
 * Token       - the token name
 * Document    - the hash table of document metadata for the token
 * Hash Handle - the cursor for the hash table
 */
struct tokenlist {
  char           *token;
  struct token   *document;
  UT_hash_handle hh;
};

/*
 This struct maps the logical ID to the document real ID inside our source
 content file. A uthash handler is provided for iterative processing if
 need be. 
 */
struct mapping {
  uint32_t       id;
  char           doc[13];
  UT_hash_handle hh;
};
/*
struct trie {
  char *key;
  struct 
};
*/

// Functions for mapping
int map_init(FILE *mapfile, struct mapping **map);
void map_document(FILE *mapfile, char document[13], uint32_t id);
char *unmap_document(struct mapping **map, uint32_t id);

// Functions for lexicon

// Functions for index
uint32_t index_token(FILE *index, struct tokenlist *token);
void index_find(FILE *index, uint32_t offset, struct tokenlist *tokens);
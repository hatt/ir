#include "uthash.h"

/*
 * ID    - the logical ID of the document the token appears in
 * Count - the number of appearances of the token in the current document
 */
struct token {
  uint32_t       id;
  uint32_t       count;
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
 * ID  - The logical ID of the document being mapped
 * Doc - The 13 character string of the document name (\0 removed)
 */
struct mapping {
  uint32_t       id;
  char           doc[13];
  UT_hash_handle hh;
};

/*
 * Key    - the character for this node in the trie
 * Value  - the value associated with the node
 * Next   - the next node for sideways traversal
 * Prev   - the previous node for sideways traversal
 * Child  - the next node for downwards traversal
 * Parent - the next node for upwards traversal
 */
struct trie {
  char        key;
  uint32_t    value;
  struct trie *next;
  struct trie *child;
  struct trie *parent;
};

// Functions for mapping
int map_init(FILE *mapfile, struct mapping **map);
void map_document(FILE *mapfile, char document[13], uint32_t id);
char *unmap_document(struct mapping **map, uint32_t id);

// Functions for trie structure
void trie_init(struct trie **root);
struct trie *trie_find(struct trie *root, char *key);
struct trie *trie_create(char key, uint32_t value);
void trie_add(struct trie **root, char *key, uint32_t value);

// Functions for lexicon
void lexicon_init(struct trie *lexicon, FILE *input);
void lexicon_add(struct trie *lexicon, char *key, uint32_t value);
uint32_t lexicon_find(struct trie *lexicon, char *key);

// Functions for index
uint32_t index_token(FILE *index, struct tokenlist *token);
void index_find(FILE *index, uint32_t offset, struct token **token);
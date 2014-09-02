#include "../uthash.h"
#include "stop.h"

/*
 * Name  - the tag name as it appears in the source
 * Data  - the value of the tag data, if any
 * State - closed (0) or open (1)
 */
struct tag {
  char *name;
  char *data;
  int  state;
};

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


struct tag *read_tag(char *buffer);

/*
token[
  char *string[],
  int  document ID
]

token[][char *string[], int id]
token[...][2]
*/
int tokenize(struct tokenlist **tokens, struct stoplist **stopwords, char *buffer, uint32_t id, int print, int stop);
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
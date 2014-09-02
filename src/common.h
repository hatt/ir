#include "uthash.h"

/*
// Entry in an index
typedef struct index_entry {
  uint32_t id;
  uint32_t freq;
} index_entry;

// Index of entries
typedef struct index {
  uint32_t    len;
  index_entry entries[];
} document_index;

// Mapping of index ID to document ID
typedef struct document_map {
  uint32_t index_id;
  uint32_t document_id;
} document_map;

typedef struct mapping {
  size_t   size;
  uint64_t ptr;
} mapping;
*/

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

int load_map(FILE *mapfile, struct mapping **map);
void map_document(FILE *mapfile, char document[13], uint32_t id);
char *unmap_document(struct mapping **map, uint32_t id);
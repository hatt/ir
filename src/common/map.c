#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "../common.h"
#include "../uthash.h"


// Map a logical ID to a document ID string
void map_document(FILE *mapfile, char document[13], uint32_t id) {
  fwrite(&id, 4, 1, mapfile);
  fwrite(document, 13, 1, mapfile);
}

// Retrieve the document ID string for a logical ID
char *unmap_document(struct mapping **map, uint32_t id) {
  struct mapping *directory;

  directory = malloc(sizeof(struct mapping));
  HASH_FIND_INT(*map, &id, directory);

  if (directory != NULL) {
    return directory->doc;
  } else {
    return NULL;
  }
}

// Rebuild the mapping hash table from disk
int map_init(FILE *mapfile, struct mapping **map) {
  // Read x bytes of our dumped binary structure
  // First 4 bytes are the unsigned integer for logical ID
  // Next 13 bytes are the character array for document ID
  while((!ferror(mapfile)) && (!feof(mapfile))) {
    struct mapping *directory;
    directory = malloc(sizeof(struct mapping));

    fread(&directory->id, 4, 1, mapfile);
    fread(&directory->doc, 13, 1, mapfile);

    if (directory != NULL) {
      HASH_ADD_INT(*map, id, directory);
    } else {
      return -1;
    }
  }
  return 0;
}
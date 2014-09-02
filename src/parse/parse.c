#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <getopt.h>

#include "../common.h"
#include "../uthash.h"
#include "tokenizer.h"

int main(int argc, char *argv[]) {
  FILE *input = NULL;

  // Get options
  int opt = 0;
  int print = 0;
  char *stopfile;
  char *sourcefile;

  while (optind < argc) {
    if ((opt = getopt(argc, argv, "ps:")) != -1) {
      switch(opt) {
        case 'p':
          print = 1;
          break;
        case 's':
          stopfile = optarg;
          break;
        default:
          break;
      }
    } else {
      sourcefile = argv[optind++];
    }
  }

  // Create our hash tables
  struct stoplist  *stopwords = NULL;
  struct tokenlist *tokens    = NULL;

  // build stoplist hash table
  if (stopfile != NULL) {
    stoplist_init(&stopwords, stopfile);
  }

  // map documents to logical IDs and tokenize each document
  input = fopen(sourcefile, "r");
  if (!input) {
    printf("Error opening index file: %s\n", sourcefile);
    return 1;
  }

  FILE *mapfile = fopen("map", "wb");
  if (!mapfile) {
    printf("Error opening map file\n");
    return 1;
  }

  FILE *lexfile = fopen("lexicon", "wb");
  if (!lexfile) {
    printf("Error opening lexicon file\n");
    return 1;
  }

  FILE *indexfile = fopen("invlists", "wb");
  if (!indexfile) {
    printf("Error opening inverted lists file\n");
    return 1;
  }

  // create index trie
  struct trie *trie = NULL;

  char buffer[256]; // this really only needs to be 80 for the index file
  while(fgets(buffer, sizeof(buffer), input) != NULL) {
    int open;             // state of tags while iterating
    char document_id[13]; // actual document number
    uint32_t logical_id;  // mapped document ID

    for (int i = 0; i < sizeof(buffer); i++) {
      if (buffer[i] == '\n') {
        struct tag *tagdata = read_tag(buffer);

        // Is a valid tag?
        if (tagdata != NULL) {
          // Is the tag a document ID?
          if (strncmp("DOCNO", tagdata->name, 5) == 0) {

            // Only map if not seen before
            if (strncmp(document_id, tagdata->data, 13)) {
              strncpy(document_id, tagdata->data, 13);
              map_document(mapfile, document_id, logical_id);

              logical_id++;
            }

          // Headline text, we want this
          } else if (strncmp("HEADLINE", tagdata->name, 8) == 0) {
            open = tagdata->state;

          // Body text, we want this
          } else if (strncmp("TEXT", tagdata->name, 4) == 0) {
            open = tagdata->state;
          }

        // Not a tag, tokenize if preceeding tag state is open
        } else {
          if (open) {
            if (stopfile != NULL) {
              tokenize(&tokens, &stopwords, buffer, logical_id, print, 1);
            } else {
              tokenize(&tokens, NULL, buffer, logical_id, print, 0);
            }
          }
        }
      }
    }
  }

  free(stopwords);

  // construct lexicon radix trie
  // open index file
  // iterate tokens hash table
  // write document count for token - HASH_COUNT(token->document)
  // write <id, count> pairs for document
  // free token
  // write token and offset for <count, <id, count>> groups to lexicon trie
  fclose(indexfile);
  free(tokens);
  // write lexicon
  // close lexicon

  fclose(lexfile);
  fclose(mapfile);

  return 0;
}
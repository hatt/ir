#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

#include "tokenizer.h"

struct tag *read_tag(char *buffer) {
  char *str;
  int length;
  // not a tag
  if (buffer[0] != '<') {
    return NULL;
  }

  // Find string beginning with '<DOCNO> '
  if ((str = strstr(buffer, "<DOCNO> ")) != NULL) {
    // ignore first 8 chars (<DOCNO> ), go from there
    for (int i = 8; i < 80; i++) {
      if ((str[i] == ' ') && (str[i+1] == '<')) {
        length = i - 8;
        break;
      }
    }
    struct tag *tagdata = malloc(sizeof(struct tag));
    tagdata->name = "DOCNO";
    tagdata->data = strndup(str + 8, length);
    tagdata->state = 0; // tag closes on same line

    return tagdata;
  }

  if ((str = strstr(buffer, "<HEADLINE>")) != NULL) {
    int state = 0;
    for (int i = 0; i < 80; i++) {
      if (str[i] == '<') {
        switch (str[i+1]) {
          case '/':
            state = 0;
            break;
          case 'H':
            state = 1;
            break;
          default:
            return NULL;
        }
      }
    }

    struct tag *tagdata = malloc(sizeof(struct tag));
    tagdata->name = "HEADLINE";
    tagdata->data = NULL;
    tagdata->state = state;

    return tagdata;
  }

  if ((str = strstr(buffer, "<TEXT>")) != NULL) {
    int state = 0;
    for (int i = 0; i < 80; i++) {
      if (str[i] == '<') {
        switch (str[i+1]) {
          case '/':
            state = 0;
            break;
          case 'T':
            state = 1;
            break;
          default:
            return NULL;
        }
      }
    }
    
    struct tag *tagdata = malloc(sizeof(struct tag));
    tagdata->name = "TEXT";
    tagdata->data = NULL;
    tagdata->state = state;

    return tagdata;
  }

  return NULL;
}

/*
 Map tokens to logical document ID
 */
int tokenize(struct tokenlist **tokens, char *buffer, uint32_t id, int print) {
  char *word;

  // Separators. Don't split on single quotes.
  while ((word = strsep(&buffer, "\\,.?/:;=-()[]{}\'\" \n\t")) != NULL) {
    struct tokenlist *tok = malloc(sizeof(struct tokenlist));
    struct tokenlist *tmp = malloc(sizeof(struct tokenlist));
    struct token *doc = malloc(sizeof(struct token));
    struct token *del = malloc(sizeof(struct token));

    if (strlen(word) > 0) {
      // this shouldn't be necessary, but for some reason
      // tags are sent to be tokenized by the parser
      if (word[0] == '<') {
        return 1;
      }

      for (int i = 0; i < strlen(word); i++) {
        word[i] = tolower(word[i]);
      }

      // Find our tokenized word, dump into tmp table
      HASH_FIND_STR(*tokens, word, tmp);
      /* If tmp table is empty, then word hasn't
       * been added to our token list yet. Initialise
       * a new token entry in our token list.
       */
      if (tmp == NULL) {
        // Initialize nested hash table
        tok->document = NULL;

        // Set nested hash table fields
        doc->id = id;
        doc->count = 1;

        // Assign token and nested metadata
        // Add local hash table entry to global
        tok->token = word;
        HASH_ADD_INT(tok->document, id, doc);
        HASH_ADD_STR(*tokens, token, tok);
      } else {
        HASH_FIND_INT(tmp->document, &id, doc);

        doc->count += 1;

        printf("Token \"%s\" has count %u in ID %u\n", word, doc->count, doc->id);
        HASH_REPLACE_INT(tmp->document, id, doc, del);
        HASH_REPLACE_STR(*tokens, token, tmp, tok);

        free(tok);
        free(del);
      }

      if (print) {
        //printf("Found %d occurence of token %s\n", tok->document.count +1, word);
        printf("%s\n", word);
      }
    }
  }

  return 0;
}

/*
 Map tokens to logical document ID with stopwords filter
 */
int tokenize_stoplist(struct tokenlist **tokens, struct stoplist **stopwords, char *buffer, uint32_t id, int print) {
  char *word;

  // Separators. Don't split on single quotes.
  while ((word = strsep(&buffer, "\\,.?/:;=-()[]{}\'\" \n\t")) != NULL) {
    struct tokenlist *tok;
    struct token *doc;

    struct tokenlist *newtok;
    struct token *newdoc;

    if (strlen(word) > 0) {
      // this shouldn't be necessary, but for some reason
      // tags are sent to be tokenized by the parser
      if (word[0] == '<') {
        return 1;
      }

      // Convert to lowercase
      for (int i = 0; i < strlen(word); i++) {
        word[i] = tolower(word[i]);
      }

      // Prune stopwords
      if (stoplist_find(stopwords, word)) {
        return 1;
      }

      // Check if exists so we can INCR counter
      HASH_FIND_STR(*tokens, word, tok);
      if (tok == NULL) {
        tok = malloc(sizeof(struct tokenlist));
        tok->document = NULL;
        tok->token = word;

        doc = malloc(sizeof(struct token));
        doc->id = id;
        doc->count = 1;

        HASH_ADD_INT(tok->document, id, doc);
        HASH_ADD_STR(*tokens, token, tok);
      } else {
        newtok = malloc(sizeof(struct tokenlist));
        newtok->document = NULL;
        newtok->token = tok->token;

        HASH_FIND_INT(tok->document, &id, doc);
        if (doc == NULL) {
          newdoc = malloc(sizeof(struct token));
          newdoc->id = id;
          newdoc->count = 1;
        } else {
          newdoc = malloc(sizeof(struct token));
          newdoc->id = doc->id;
          newdoc->count = doc->count + 1;
        }

        //printf("Token \"%s\" has count %u in ID %u\n", word, doc->count, doc->id);
        HASH_REPLACE_INT(newtok->document, id, newdoc, doc);
        HASH_REPLACE_STR(*tokens, token, newtok, tok);

        free(doc);
        free(tok);
      }

      if (print) {
        printf("%s\n", word);
      }
    }
  }

  return 0;
}
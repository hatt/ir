#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "../common.h"

// Initialise a new empty trie node
void trie_init(struct trie **root) {
  *root = trie_create('\0', 0xFFFFFFFF);
}

// Create a new trie node
struct trie *trie_create(char key, uint32_t value) {
  struct trie *node = malloc(sizeof(struct trie));

  node->key = key;
  node->value = value;
  node->next = NULL;
  node->child = NULL;
  node->parent= NULL;

  return node;
}

// Find a key string in a trie
struct trie *trie_find(struct trie *root, char *key) {
  struct trie *level = root;
  struct trie *node = NULL;

  for (uint32_t i = 0; i < strlen(key); i++) {
    struct trie *found = NULL;
    struct trie *current;

    // Iterate horizontally though the trie
    for(current = level; current != NULL; current = current->next) {
      if (current->key == key[i]) {
        found = current;
        break;
      }
    }

    // Found nothing in this level, full key doesn't exist
    if (found == NULL) {
      return NULL;
    }

    // Reached end of full key name
    if (key[i+1] == '\0') {
      node = current;
      return node;
    }

    // Full key doesn't end in this level, go deeper
    level = found->child;
  }

  return NULL;
}

// Add a node to an existing root
void trie_add(struct trie **root, char *key, uint32_t value) {
  struct trie *node = NULL;

  // Assign node as root child
  node = (*root)->child;

  // If node is null, root has no children
  if (node == NULL) {
    node = *root;
    for (int i = 0; i < strlen(key); i++) {
      node->child = trie_create(key[i], 0xFFFFFFFF);
      node->child->parent = node;

      node = node->child;
    }

    node->child = trie_create('\0', value);
    return;
  }

  // Node isn't null, check if our key exists
  // If it exists, return, do NOT update value!
  if (trie_find(node, key)) {
    return;
  }

  // Cycle the key pointer until we find the last common parent
  while(*key) {
    if (*key == node->key) {
      key++;
      node = node->child;
    } else {
      break;
    }
  }

  // From the common parent, insert intermediary keys
  // If we need to branch across the current level,
  //   then add at that level and return now
  while(node->next) {
    if (*key == node->next->key) {
      trie_add(&(node->next), key++, value);
      return;
    }
    node = node->next;
  }

  // If key is at the end of string (\0), terminate it
  // Otherwise, inject it with no value
  if (*key) {
    node->next = trie_create(*key, 0xFFFFFFFF);
  } else {
    node->next = trie_create('\0', value);
  }

  // We're at the bottom of a token
  // Set the neighbor edge to share a parent
  node->next->parent = node->parent;

  // Key is empty? We're done
  // Otherwise, already iterated neighbor nodes
  // Fill out their children
  if (!(*key)) {
    return;
  } else {
    for (int i = 0; i < strlen(key); i++) {
      node->child = trie_create(key[i], 0xFFFFFFFF);
      node->child->parent = node;

      node = node->child;
    }

    node->child = trie_create('\0', value);
    return;
  }
}

// Remove a node from a root
void trie_delete(struct trie **trie, char *key) {}
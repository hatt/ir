struct stoplist {
  char           key[13]; // longest key is 13 chars
  UT_hash_handle hh;
};

int stoplist_init(struct stoplist **stopwords, char *wordlist);
int stoplist_find(struct stoplist **stopwords, char *keyname);
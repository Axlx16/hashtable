#include <stdlib.h>
#include <stdbool.h>

/* Hashtable will use wd (int) and return a linked list of arrays containing all the users/clients tracking the wd */


#define HASHTABLE_SIZE 10 // Entires in hashtable (10 user connections at most) 

/* PUBLIC */
typedef struct {
    int key;
    int *value;
    struct hashEntry *next;
} hashEntry;

typedef struct {
    hashEntry **entries;
} hashTable;


hashEntry * heCreate(int key, int *value);
hashEntry * heDeepCpy(const hashEntry *heOriginal);
bool htDestroy(hashTable *ht);

int hashFunc(int key);

bool htInsert(hashTable *ht, int key, int *value);
bool htRemove(hashTable *ht, int key);

bool htContains(hashTable *ht, int key);
hashEntry * htLookup(hashTable *ht, int key); 
bool htEmpty(hashTable *ht);



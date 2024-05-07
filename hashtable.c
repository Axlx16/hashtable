#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "hashtable.h"

#define TEST_KEY 4949

hashTable * htCreate() {
    hashTable *ht = (hashTable * )malloc(sizeof(hashTable));

    ht->entries = (hashEntry **)malloc(sizeof(hashEntry *) * HASHTABLE_SIZE);
    
    for (int i = 0; i < HASHTABLE_SIZE; ++i) {
        ht->entries[i] = NULL; 
    }

    return ht;
}

bool heFree(hashEntry *he) {
    if (he != NULL) {
        heFree(he->next);
        free(he);
    }
}

bool htDestroy(hashTable *ht) {
    for (int i = 0; i < HASHTABLE_SIZE; ++i) {
        if (ht->entries[i] != NULL) {
            heFree(ht->entries[i]);
        } 
    }

    free(ht->entries);
    free(ht);
    return true;
}


/* Division Method */
int hashFunc(int key) {
    return key % HASHTABLE_SIZE;
}

hashEntry * heCreate(int key, int *value) {
    hashEntry * he = (hashEntry *)malloc(sizeof(hashEntry)); 
    he->key = key;
    he->value = value; // shallow copy
    he->next = NULL; 

    return he;

}

hashEntry * heDeepCpy(const hashEntry *heOriginal) {
    hashEntry * heCpy = heCreate(heOriginal->key, heOriginal->value);
    if (heOriginal->next != NULL) {
        heCpy->next = heDeepCpy(heOriginal->next);
        if (heCpy->next == NULL) {
            free(heCpy->value);
            free(heCpy);
            return NULL;
        }
    }

    return heCpy;
}

bool htInsert(hashTable *ht, int key, int *value) {
    
    int hashVal = hashFunc(key);

    hashEntry *he = heCreate(key, value);

    if (ht->entries[hashVal] == NULL) {
        ht->entries[hashVal] = he;
        return true;
    }


    ht->entries[hashVal]->next = (hashEntry *)he;

    return true; 

}

bool htContains(hashTable *ht, int key) {
    int hashVal = hashFunc(key);

    if (ht->entries[hashVal] != NULL) { 
        hashEntry * entries_cpy = heDeepCpy(ht->entries[hashVal]);

        while (entries_cpy->key != key) {
            if (entries_cpy->next != NULL) {
                entries_cpy = entries_cpy->next;
            } else {
                return false;
            }
        }
        return true;
    }

    return false;
}



hashEntry * htLookup(hashTable *ht, int key) {

    int hashVal = hashFunc(key);

    if (ht->entries[hashVal] != NULL) {
        hashEntry * entries_cpy = heDeepCpy(ht->entries[hashVal]);
        

        while (entries_cpy->key != key) {
            if (entries_cpy->next != NULL) {
                entries_cpy = entries_cpy->next;
            } else {
                printf("--NO MATCH FOUND--\n"); 
                return NULL;
            }
        }
        printf("--KEY %d FOUND--\n", key);
        return entries_cpy; 
    }
    /* If no match is found*/ 
    printf("--NO MATCH FOUND--\n"); 
    return NULL;
}

bool htEmpty(hashTable *ht) {
    for (int i = 0; i < HASHTABLE_SIZE; ++i) {
        if (ht->entries[i] != NULL) {
            return false;
        }
    }

    return true;
}

int main() {
    hashTable *ht = htCreate();
    int *intArr = malloc(sizeof(int) * 10);
    for (int i = 0; i < 10; ++i) {
        intArr[i] = i + 3;
    }
    
    htInsert(ht, TEST_KEY, intArr);
    
    hashEntry *output1 = htLookup(ht, TEST_KEY); 

    htInsert(ht, 4959, intArr);
 
    hashEntry *output2 = htLookup(ht, 4959);

    printf("%d\n", output2->key);

    
    // htInsert(ht, 4950, intArr);
    
    // hashEntry *output3 = htLookup(ht, 4950); 
    
    // printf("%d\n", output3->key);

    htDestroy(ht);
}
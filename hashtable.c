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
        if (he->next != NULL) {
            heFree(he->next);
        }
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


    he->next = ht->entries[hashVal];
    ht->entries[hashVal] = he;

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

bool htRemove(hashTable *ht, int key) {
    int hashVal = hashFunc(key); 
    if (ht->entries[hashVal] != NULL) { 
          
        hashEntry *head = ht->entries[hashVal];
        if (head->key == key) {
            ht->entries[hashVal] = head->next;
            return true;
        }         

        while (head->next != NULL) {
            if (((hashEntry *)(head->next))->key == key) {
                hashEntry *tmp = head->next;
                head->next = tmp->next;
                free(tmp);
                return true;
            }
            head = head->next;
        } 
        
        printf("--NO ENTRY FOUND--\n"); 
        
        return false;
    }

    return false;
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

    htInsert(ht, 4969, intArr);
 
    hashEntry *output3 = htLookup(ht, 4969);
    
    /* 99% sure this works well */ 
    hashEntry *he_temp = ht->entries[9]->next;
    hashEntry *he_temp2 = he_temp->next;
    printf("%d\n", ht->entries[9]->key); 
    printf("%d\n", he_temp->key);
    printf("%d\n", he_temp2->key);

    htRemove(ht, 4969);

    printf("--AFTER REMOVING ELEMENT--\n");
    for (int i = 0; i < 2; ++i) {
        printf("%d\n", ht->entries[9]->key);
        ht->entries[9] = ht->entries[9]->next;
    }

    /* TEST TO ITERATATE THROUGH ALL KEYS*/
    // htInsert(ht, 4950, intArr);
    
    // hashEntry *output3 = htLookup(ht, 4950); 
    
    // printf("%d\n", output3->key);

    htDestroy(ht);
}
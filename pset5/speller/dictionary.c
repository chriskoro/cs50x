/**
 * Implements a dictionary's functionality.
 * Provides the functionality to speller.c
 */

#include <stdio.h>
#include <cs50.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

//max hashtable indexes (heads)
const int SIZE = 27;

//create dictionary as hashtable
typedef struct node
{
    //LENGTH of any word + 1 for the null terminator
    char word[LENGTH+1];
    struct node* next;
}
node;

// create hashtable
node* hashtable[SIZE];

//hash function (source StackExchange)
int hash (const char* word)
{
    int hash = 0;
    int n;
    for (int i = 0; word[i] != '\0'; i++)
    {
        // alphabet case
        if(isalpha(word[i]))
            n = word [i] - 'a' + 1;

        // comma case
        else
            n = 27;

        hash = ((hash << 3) + n) % SIZE;
    }
    return hash;
}


/**
 * Returns true if word is in dictionary data structure else false. 
 */
bool check(const char *word)
{
    //not case sensitive, assume words passed in are only alphabetical or apostrophes
    
    //gives you "bucket" or linked list that your word is in
    int llToSearch = hash(word);
    
    //cursor node starts at head (first node in ll) and traverses in loop (node pointer points to heap)
    node* cursor = hashtable[llToSearch];
    
    while (cursor != NULL)
    {
        //strcasecmp returns 0 if word is the same (case-insensitive)
        if (strcasecmp(word, cursor->word) == 0)
        {
            //found word in dictionary (meaning it was spelled correctly)
            return true;
        }
        else
        {
            // traverse the Linked List
            cursor = cursor->next;
        }
    }
    
    return false;
}

    //global variable, words in dictionary (for late use in size function)
    int wordsInDictionary = 0;

/**
 * Loads dictionary into memory (the data structr we've created, a hashtable). Returns true if successful else false.
 */
bool load(const char *dictionary)
{
    FILE* dicPtr = fopen(dictionary, "r");
    
    if (dicPtr == NULL)
        return false;
    
    char word[LENGTH+1];
    
    //Take word from dictionary and put in newly allocated node
    while (fscanf(dicPtr, "%s", word) != EOF)
    {
        wordsInDictionary++;
        
        node* new_node = malloc(sizeof(node));
        
        if (new_node == NULL)
        {
            unload();
            return false;
        }
        else
        {
            for(int i = 0; word[i] != '\0'; i++)
            {
                word[i] = tolower(word[i]);
            }
            strcpy(new_node->word, word);
        }
       
        
        //which bucket to put word in (use hash function)
        int bucket = hash(new_node->word);
        
        //insert node into linked list / link nodes together
        //ht is array of linked lists, each element in arry is node pointer, pointing to first node in bucket's linked list
        if (hashtable[bucket] == NULL)
        {
            //make head of linked list (bucket) (insert into linked list)
            hashtable[bucket] = new_node;
            new_node->next = NULL;
        }
        
        //else if head is not null (swap out head for this new head, new_node)
        else
        {
            new_node->next = hashtable[bucket];
            hashtable[bucket] = new_node;
        }
    }
    
    fclose(dicPtr);
    
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    // words in dictionary
    if (wordsInDictionary > 0)
        return wordsInDictionary;
    
    else
        return 0;
}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload(void)
{
    // TODO
    for (int i = 0; i < 27; i++)
    {
        //head of linked list
        node* cursor = hashtable[i];
        
        //for each linked list (each node in this element of the array)
        while (cursor != NULL)
        {
            node* temp = cursor;
            cursor = cursor->next;
            free(temp);
        }
    }

    return true;
}

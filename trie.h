#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#ifndef TRIE_H
#define TRIE_H

struct TrieNode {
    struct TrieNode* next_character[256];
    void *ptr;
};

typedef struct TrieNode TrieNode;

TrieNode *create_trie_node();
void traverse_trie(TrieNode *root);
void insert_in_trie(TrieNode * root, char *word, void *ptr);
void traverse_trie_aux(TrieNode *root, char *buffer, int index);
void delete_from_trie(TrieNode *root, char *buffer);
void insert_file_in_trie(const char *file_name, TrieNode *root);
void free_trie(TrieNode *root);
void insert_file_in_trie(const char *file_name, TrieNode *root);
void *search_in_trie(TrieNode *root, char *target);

extern TrieNode *Medication_ID2Index;
extern TrieNode *Batch_ID2PrevPtr;
extern TrieNode *Medication_Name2Index;
extern TrieNode *Supplier_ID2PrevPtr;

#endif // !TRIE_H

#include "./trie.h"

void insert_in_trie(TrieNode *root, char *word, void *ptr)
{
    TrieNode *node = root;
    char *s = word;
    char current;
    while (*s) {
        current = *s;
        if (node->next_character[(size_t)current] == NULL) {
            node->next_character[(size_t)current] = create_trie_node();
        }
        node = node->next_character[(size_t)current];
        s = s + 1;
    }
    node->ptr = ptr;
}

TrieNode *create_trie_node()
{
    TrieNode *new = calloc(1, sizeof(TrieNode));
    return new;
}

void traverse_trie_aux(TrieNode *node, char *buffer, int index)
{
    if (node->ptr) {
        buffer[index] = '\0';
        printf("%s\n",buffer);
    }
    for (int i = 0; i < 256; ++i) {
        if (node->next_character[i]) {
            buffer[index] = i;
            traverse_trie_aux(node->next_character[i], buffer, index + 1);
        }
    }
}

void traverse_trie(TrieNode *root)
{
    char buffer[100];
    int index = 0;
    printf("  Entries in trie are\n");
    traverse_trie_aux(root, buffer, index);
}

void delete_from_trie(TrieNode *root, char *buffer)
{
    bool found = true;
    char *s = buffer;
    TrieNode *node = root;
    while (*s && found) {
        char current = *s;
        if (!node->next_character[(size_t)current]) {
            /*printf("\"%s\" is not present in trie\n", buffer);*/
            found = false;
        }
        else {
            ++s;
            node = node->next_character[(size_t)current];
        }
    }
    if (found && node->ptr != NULL) {
        node->ptr = NULL;
        /*printf("\"%s\" deleted from trie successfully\n", buffer);*/
    }
    else {
        /*printf("\"%s\" is not present in trie\n", buffer);*/
    }
}

void free_trie_rec(TrieNode *node)
{
    for (int i = 0; i < 256; ++i) {
        if (node->next_character[i]) {
            free_trie_rec(node->next_character[i]);
        }
    }
    free(node);
}

void free_trie(TrieNode *root)
{
    if (!root) {
        return;
    }
    free_trie_rec(root);
}

void insert_file_in_trie(const char *file_name, TrieNode *root)
{
    FILE *file = fopen(file_name, "r");
    fseek(file, 0, SEEK_END);
    size_t length = ftell(file);
    fseek(file, 0, SEEK_SET);
    size_t index = 0;
    ++length;
    char *buffer = malloc(length * sizeof(char));
    char c;

    printf("length = %ld\n", length);

    while ((c = fgetc(file)) != EOF) {
        buffer[index++] = c;
    }

    buffer[index] = '\n';
    fclose(file);
    char word[100];
    index = 0;
    int widx;
    while (index < length) {
        widx = 0;
        while (buffer[index] != '\n') {
            word[widx++] = buffer[index++];
        }
        word[widx] = '\0';
        insert_in_trie(root, word, (void *)1);
        ++index;
    }
    free(buffer);
}

void *search_in_trie(TrieNode *root, char *target)
{
    char *s = target;
    bool found = true;
    TrieNode *node = root;
    while (*s && found) {
        char current = *s;
        if (!node->next_character[(size_t)current]) {
            found = false;
        }
        else {
            ++s;
            node = node->next_character[(size_t)current];
        }
    }
    void *retVal = node->ptr;
    if (!found) {
        retVal = NULL;
    }
    return retVal;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALPHABET_SIZE 26

// Trie node structure
struct TrieNode {
    struct TrieNode *children[ALPHABET_SIZE];
    int isEndOfWord;
    int count; // Number of occurrences of the word
};

// Creates a new trie node
struct TrieNode *createNode() {
    struct TrieNode *newNode = (struct TrieNode *)malloc(sizeof(struct TrieNode));
    if (newNode) {
        newNode->isEndOfWord = 0;
        newNode->count = 0;
        for (int i = 0; i < ALPHABET_SIZE; i++)
            newNode->children[i] = NULL;
    }
    return newNode;
}

// Inserts a word into the trie
void insert(struct TrieNode *root, char *word) {
    struct TrieNode *temp = root;
    for (int i = 0; i < strlen(word); i++) {
        int index = word[i] - 'a';
        if (!temp->children[index])
            temp->children[index] = createNode();
        temp = temp->children[index];
    }
    temp->isEndOfWord = 1;
    temp->count++;
}

// Searches for a word in the trie and returns the number of occurrences
int numberOfOccurrences(struct TrieNode *root, char *word) {
    struct TrieNode *temp = root;
    for (int i = 0; i < strlen(word); i++) {
        int index = word[i] - 'a';
        if (!temp->children[index])
            return 0; // Word not found
        temp = temp->children[index];
    }
    return (temp != NULL && temp->isEndOfWord) ? temp->count : 0;
}

// Deallocates memory used by the trie
void deallocateTrie(struct TrieNode *root) {
    if (root) {
        for (int i = 0; i < ALPHABET_SIZE; i++) {
            if (root->children[i])
                deallocateTrie(root->children[i]);
        }
        free(root);
    }
}

// Reads the dictionary from file and returns the number of words
int readDictionary(char *filename, char **pInWords) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error opening file.\n");
        exit(1);
    }

    int numWords = 0;
    char word[100]; // Assuming maximum word length of 100 characters

    while (fscanf(file, "%s", word) != EOF) {
        pInWords[numWords] = strdup(word);
        numWords++;
    }

    fclose(file);
    return numWords;
}

int main(void) {
    char *inWords[256];

    // Read the number of words in the dictionary
    int numWords = readDictionary("dictionary.txt", inWords);
    for (int i = 0; i < numWords; ++i) {
        printf("%s\n", inWords[i]);
    }

    // Create and populate trie
    struct TrieNode *root = createNode();
    for (int i = 0; i < numWords; i++) {
        insert(root, inWords[i]);
    }

    // Words to search for
    char *pWords[] = {"notaword", "ucf", "no", "note", "corg"};
    for (int i = 0; i < 5; i++) {
        printf("\t%s : %d\n", pWords[i], numberOfOccurrences(root, pWords[i]));
    }

    // Deallocate memory
    deallocateTrie(root);
    
    return 0;
}

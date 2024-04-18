#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALPHABET_SIZE 26  // Number of possible children each Trie node can have (a-z)

// Trie node structure
struct TrieNode {
    struct TrieNode *children[ALPHABET_SIZE];
    int wordCount; // Count of words ending here
};

// Trie structure
struct Trie {
    struct TrieNode *root;
};

// Initializes a trie node
struct TrieNode *createTrieNode() {
    struct TrieNode *pNode = (struct TrieNode *)malloc(sizeof(struct TrieNode));
    pNode->wordCount = 0;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        pNode->children[i] = NULL;
    }
    return pNode;
}

// Initializes a trie structure
struct Trie *createTrie() {
    struct Trie *pTrie = (struct Trie *)malloc(sizeof(struct Trie));
    pTrie->root = createTrieNode();
    return pTrie;
}

// Inserts the word to the trie structure
void insert(struct Trie *pTrie, char *word) {
    struct TrieNode *pCrawl = pTrie->root;
    while (*word) {
        int index = *word - 'a';
        if (!pCrawl->children[index]) {
            pCrawl->children[index] = createTrieNode();
        }
        pCrawl = pCrawl->children[index];
        word++;
    }
    pCrawl->wordCount++;
}

// Computes the number of occurrences of the word
int numberOfOccurances(struct Trie *pTrie, char *word) {
    struct TrieNode *pCrawl = pTrie->root;
    while (*word) {
        int index = *word - 'a';
        if (!pCrawl->children[index]) {
            return 0;
        }
        pCrawl = pCrawl->children[index];
        word++;
    }
    return pCrawl->wordCount;
}

// Deallocates a trie node recursively
void deallocateTrieNode(struct TrieNode *pNode) {
    if (!pNode) return;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (pNode->children[i]) {
            deallocateTrieNode(pNode->children[i]);
        }
    }
    free(pNode);
}

// Deallocate the trie structure
struct Trie *deallocateTrie(struct Trie *pTrie) {
    if (pTrie) {
        deallocateTrieNode(pTrie->root);
        free(pTrie);
    }
    return NULL;
}

// Reads words from the dictionary file into the trie structure
int readDictionary(char *filename, char **pInWords) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Failed to open file\n");
        return 0;
    }

    int numWords;
    fscanf(file, "%d", &numWords);

    for (int i = 0; i < numWords; ++i) {
        pInWords[i] = malloc(256 * sizeof(char)); // Allocate memory for each word
        fscanf(file, "%s", pInWords[i]);
    }

    fclose(file);
    return numWords;
}

int main(void) {
    char *inWords[256];

    // Read the number of the words in the dictionary
    int numWords = readDictionary("dictionary.txt", inWords);
    for (int i = 0; i < numWords; ++i) {
        printf("%s\n", inWords[i]);
    }

    struct Trie *pTrie = createTrie();
    for (int i = 0; i < numWords; i++) {
        insert(pTrie, inWords[i]);
    }

    char *pWords[] = {"notaword", "ucf", "no", "note", "corg"};
    for (int i = 0; i < 5; i++) {
        printf("\t%s : %d\n", pWords[i], numberOfOccurances(pTrie, pWords[i]));
    }
    pTrie = deallocateTrie(pTrie);

    // Clean up allocated word memory
    for (int i = 0; i < numWords; ++i) {
        free(inWords[i]);
    }

    if (pTrie != NULL)
        printf("There is an error in this program\n");
    return 0;
}

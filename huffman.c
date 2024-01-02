#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define ASCII 95

/* 
Code for priorty queue in binary heap implementation
----------------------------------------------------------------------------------
*/

// Struct for a key-value pair in PQ
typedef struct KVPair {
    int key;
    int value;
} KVP;

// Struct for PQ
typedef struct PQ {
    KVP* heap; // in array form
    int capacity; // maybe dynamically allocated this
    int size;
} PriorityQueue;

// Function to initialise a priority queue
PriorityQueue* initPQ(int capacity) {
    PriorityQueue* pq = (PriorityQueue*) malloc(sizeof(PriorityQueue));
    pq->heap = (KVP*) malloc((capacity + 1)*sizeof(KVP)); // + 1 for the heap implementation of 1-index based
    pq->capacity = capacity;
    pq->size = 0;
    return pq;
}

// Function to swap to key-value pair nodes in a binary heap - for reheap function
void swap(KVP* a, KVP* b) {
    KVP temp = *a;
    *a = *b;
    *b = temp;
}

// Function to reheap the binary heap
void minHeapify(PriorityQueue* pq, int index) {
    int smallest = index;
    int left = 2 * index;
    int right = 2 * index + 1;

    if (left <= pq->size && pq->heap[left].key < pq->heap[smallest].key) {
        smallest = left;
    }

    if (right <= pq->size && pq->heap[right].key < pq->heap[smallest].key) {
        smallest = right;
    }

    if (smallest != index) {
        swap(&pq->heap[index], &pq->heap[smallest]);
        minHeapify(pq, smallest);
    }
}

// Function to insert a key-value pair into the priority queue
void insert(PriorityQueue* pq, int key, int value) {
    if (pq->size == pq->capacity) {
        printf("Priority Queue Overflow\n");
        return;
    }

    pq->size++;
    int i = pq->size;

    // Key is frequency of character, value is ASCII value of character
    pq->heap[i].key = key;
    pq->heap[i].value = value;

    while (i > 1 && pq->heap[i].key < pq->heap[i / 2].key) {
        swap(&pq->heap[i], &pq->heap[i / 2]);
        i /= 2;
    }
}

// Function to extract the key with the minimum value from the priority queue
KVP extractMin(PriorityQueue* pq) {
    KVP err = {0, 0};
    if (pq->size == 0) {
        printf("Priority Queue Underflow\n");
        return err;
    }

    KVP minEle = pq->heap[1];
    pq->heap[1] = pq->heap[pq->size];
    pq->size--;

    minHeapify(pq, 1);

    return minEle;
}

// Function to free the memory allocated for the priority queue
void freePriorityQueue(PriorityQueue* pq) {
    free(pq->heap);
    free(pq);
}

/* 
----------------------------------------------------------------------------------
*/

/* 
Code for Huffman Coding implementation
----------------------------------------------------------------------------------
*/

// Structure to represent the Huffman tree
typedef struct HuffmanTree {
    int* L;   // Left child indices
    int* R;   // Right child indices
    int* P;   // Parent indices
} HuffmanTree ;

// Function to initialize the Huffman tree
HuffmanTree* initializeHuffmanTree(int n) {
    
    HuffmanTree* huffmanTree = (HuffmanTree*)malloc(sizeof(HuffmanTree));

    huffmanTree->L = (int*)malloc((2 * n + 1) * sizeof(int));
    huffmanTree->R = (int*)malloc((2 * n + 1) * sizeof(int));
    huffmanTree->P = (int*)malloc((2 * n + 1) * sizeof(int));

    // Initialize arrays
    for (int i = 1; i < 2 * n + 1; i++) {
        huffmanTree->L[i] = 0;
        huffmanTree->R[i] = 0;
        huffmanTree->P[i] = 0;
    }

    return huffmanTree;
}

void freeHuffmanTree(HuffmanTree* huffmanTree) {
    free(huffmanTree->L);
    free(huffmanTree->R);
    free(huffmanTree->P);
    free(huffmanTree);
}

// Function to build Huffman Tree
HuffmanTree* buildHuffmanTree(int freqMap[]) {

    HuffmanTree* ht = initializeHuffmanTree(ASCII);
    PriorityQueue* pq = initPQ(2 * ASCII);
    int distinct = 0;

    for (int i = 0; i < ASCII; i++) {
        if (freqMap[i] > 0) {
            insert(pq, freqMap[i], i + 32);
            distinct++;
        }
    }

    for (int i = ASCII + 32; i < distinct - 1 + ASCII + 32; i++) {

        KVP x = extractMin(pq); // Gives min frequency key-value pair based on key
        KVP y = extractMin(pq);

        freqMap[i - 32] = freqMap[x.value - 32] + freqMap[y.value - 32];

        insert(pq, freqMap[i - 32], i);
        ht->L[i] = x.value;
        ht->R[i] = y.value;
        ht->P[x.value] = i;
        ht->P[y.value] = i;
    }

    ht->P[2 * ASCII - 1] = 0;
    return ht;
}

// Function to encode symbol
void encodeSymbol(int a, char** B, int* l, HuffmanTree* ht, int distinct) {
    if (a != (ASCII + 32 + distinct - 2)) {
        encodeSymbol(ht->P[a], B, l, ht, distinct);
        if (a == ht->L[ht->P[a]]) {
            (*B)[(*l)++] = '0';
        } else {
            (*B)[(*l)++] = '1';
        }
    }

    // Dynamically allocate memory for B as needed
    *B = realloc(*B, (*l + 1) * sizeof(char));
    if (*B == NULL) {
        // Handle memory allocation failure
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
}

// Function to encode text
char* encodeTree(char* A, HuffmanTree* ht, int freqMap[]) {

    char* B = (char*)malloc(1 * sizeof(char));
    int l = 0;

    int distinct = 0;
    // Getting the number of distinct characters in A
    for (int i = 0; i < ASCII; i++) {
        if (freqMap[i] > 0) {
            distinct++;
        }
    }

    for (int i = 0; i < strlen(A); i++) {
        encodeSymbol(A[i], &B, &l, ht, distinct);
    }

    return B;
}

// Function to decode text
char* decodeText(char* B, HuffmanTree* ht, int freqMap[]) {

    int distinct = 0;
    // Getting the number of distinct characters in A
    for (int i = 0; i < ASCII; i++) {
        if (freqMap[i] > 0) {
            distinct++;
        }
    }

    char* A = NULL;
    int k = 0;
    int a = (ASCII + 32 + distinct - 2);

    for (int i = 0; i < strlen(B); i++) {
        if (B[i] == '0') {
            a = ht->L[a];
        } else {
            a = ht->R[a];
        }

        if (ht->L[a] == 0) {
            // Dynamically allocate memory for A as needed
            A = realloc(A, (k + 1) * sizeof(char));
            if (A == NULL) {
                // Handle memory allocation failure
                fprintf(stderr, "Memory allocation failed\n");
                exit(EXIT_FAILURE);
            }

            A[k++] = a;
            a = (ASCII + 32 + distinct - 2);
        }
    }

    // Null-terminate the decoded text
    A = realloc(A, (k + 1) * sizeof(char));
    if (A == NULL) {
        // Handle memory allocation failure
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    A[k] = '\0';

    return A;
}

/* 
----------------------------------------------------------------------------------
*/

// Get frequency array - frequency analysis
void freqArr(const char* filename, int freqMap[]) {
    FILE* inputFile = fopen(filename, "r");

    if (!inputFile) {
        fprintf(stderr, "Error opening file: %s\n", filename);
        return;
    }

    int c;
    while((c = fgetc(inputFile)) != EOF) {
        freqMap[c-32]++;
    }

    fclose(inputFile);
}

int main() {
    const char* filename = "input.txt";

    // Step 1: Generate Frequency Array
    int frequencyMap[2 * ASCII] = {0};
    freqArr(filename, frequencyMap);

    // Step 2: Build Huffman Tree
    HuffmanTree* ht = buildHuffmanTree(frequencyMap);

    // Step 3: Encode Text
    FILE* inputFile = fopen(filename, "r");
    if (!inputFile) {
        fprintf(stderr, "Error opening file: %s\n", filename);
        return 1;
    }

    char* inputText = NULL;
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), inputFile) != NULL) {
        // Allocate memory for inputText and initialize if it's the first iteration
        if (inputText == NULL) {
            inputText = strdup(buffer);
        } else {
            // Reallocate memory and concatenate the buffer
            inputText = realloc(inputText, strlen(inputText) + strlen(buffer) + 1);
            strcat(inputText, buffer);
        }
    }

    fclose(inputFile);

    printf("Original Text:\n%s\n", inputText);

    char* encodedText = encodeTree(inputText, ht, frequencyMap);
    printf("Encoded Text:\n%s\n", encodedText);

    // Step 4: Decode Text
    char* decodedText = decodeText(encodedText, ht, frequencyMap);
    printf("Decoded Text:\n%s\n", decodedText);

    // Cleanup
    free(inputText);
    free(encodedText);
    free(decodedText);
    freeHuffmanTree(ht);

    return 0;
}

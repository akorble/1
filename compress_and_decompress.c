//
// Created by Cameron Brewster on 2024-11-20.
//
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "compress_and_decompress.h"
#include <string.h>


HuffmanNode *createLeafNode(char data, int frequency) {
    HuffmanNode * newNode = (HuffmanNode*)malloc(sizeof(HuffmanNode)); // Allocating memory
    if(newNode == NULL) {
        printf("Memory allocation failed");
        exit(EXIT_FAILURE); //Edge case if memory allocation fails
    }

    newNode -> frequency = frequency;
    newNode -> data = data;
    newNode -> left = NULL;
    newNode -> right =NULL; // Initializing leaf node
    return newNode;

}

HuffmanNode *createInternalNodes(HuffmanNode *left, HuffmanNode *right) {
    HuffmanNode *newNode = (HuffmanNode*)malloc(sizeof(HuffmanNode)); // allocating memory for internal node
    if(newNode == NULL) {
        printf("Memory allocation failed");
        exit(EXIT_FAILURE);  //Edge case if memory allocation fails
    }
    newNode -> frequency = left->frequency + right->frequency; // Setting the frequency to the sum of both its child nodes
    newNode -> data = '\0'; // Internal nodes do not hold a character
    newNode -> left = left; // Point to children
    newNode -> right = right;

    return newNode;
}



HuffmanNode* buildHuffmanTree(Queue* queue) {
    while (queue->front != NULL && queue->front->next != NULL) {  // While more than one node in the queue

        // Dequeue the two nodes with the smallest frequencies
        HuffmanNode* left = priorityDequeue(queue);
        HuffmanNode* right = priorityDequeue(queue);

        // Create a new internal node
        HuffmanNode* internalNode = createInternalNodes(left, right);

        // Enqueue the new internal node back into the queue
        priorityEnqueue(queue, internalNode);
    }

    // The final node left in the queue is the root of the Huffman tree
    return priorityDequeue(queue);
}

    // Only one node should remain in the queue, which is the root of the Huffman tree

void freeHuffmanTree(HuffmanNode* root) {
    if(root == NULL) {
        return; // Edge case if tree DNE
    }
    freeHuffmanTree(root->left); // Recusively calling left subtree
    freeHuffmanTree(root->right); // Recursive calls to right subtree

    free(root); // Freeing each node
}
void getCode(HuffmanNode *root, char codes[256][MAX], char* currentCode, int depth) {
    if (root == NULL) { // Edge cse if tree DNE
        return;
    }
    //If the node is a leaf node (no children)
    if (root->left == NULL && root->right == NULL) {
        currentCode[depth] = '\0';  // Adding a null character to the end of the string
        strcpy(codes[(unsigned char)root->data], currentCode); //copying the code into a string
        return;
    }

    // Traverse the left subtree
    currentCode[depth] = '1';
    // Note going left in the subtree should append 0; however our output is opposite if we append 0
    // So we had to update this to append 1 going left in order to get the proper output
    getCode(root->left, codes, currentCode, depth + 1); // Recursively calling left subtree

    // Traverse the right subtree
    // Appending 0 when travelling right
    currentCode[depth] = '0';
    getCode(root->right, codes, currentCode, depth + 1); // Recursively calling right subtree
}

void HuffmanCodes(HuffmanNode *root, char codes[256][MAX]) {
    char currentCode[MAX] = {0};  // Initialize currentCode array to store the current code being generated

    // Special case for when there is only one character (tree with only one node)
    if (root != NULL && root->left == NULL && root->right == NULL) { //Edge case
        currentCode[0] = '0';  //Assigning the single node to 0
        codes[(unsigned char)root->data][0] = '0';
        codes[(unsigned char)root->data][1] = '\0';  //handling the null character at the end of the string

    } else {
        getCode(root, codes, currentCode, 0);  //else getting codes from the getCode function
    }
}
void compress(const char *filename, char *compressed, HuffmanNode *root, char codes[256][MAX]) {
    FILE *file = fopen(filename, "r"); // Edge case if the file cannot be opened
    if (file == NULL) {
        printf("Error: Unable to open file '%s'\n", filename);
        exit(1);
    }

    int j = 0;
    int c;
    while ((c = fgetc(file)) != EOF) {//traversing the file one character at a time to get the code
        char *code = codes[(unsigned char)c]; //adding the code to the string


        for (int k = 0; code[k] != '\0'; k++) { //Appending the code to the array
            compressed[j++] = code[k];  // storing each character as a 0 or 1 in the array
        }
    }


    compressed[j] = '\0';  //Adding a null character
    fclose(file); // Closing the file
}

void decompress(const char *compressed, char *decompressed, HuffmanNode *root) {
    if (root == NULL) { // If the huffman tree for the certain code frequencies DNE
        printf("Error: Huffman Tree is empty!\n");
        return;
    }


    if (root->left == NULL && root->right == NULL) {//Tree only has one node

        int index = 0;
        while (compressed[index] != '\0') {//traversing the compressed string
            decompressed[index] = root->data;  //append the character
            index++;
        }
        decompressed[index] = '\0'; //adding a null character
        printf("Decompressed Data: %s\n", decompressed);
        return;
    }


    int index = 0;  // pointer to compressed string
    int decompressedIndex = 0;  //pointer into decompressed string
    HuffmanNode *currentNode = root; //start from root of tree to decode

    // Traverse the compressed binary string
    while (compressed[index] != '\0') {// Traversing to the end of the compressed encoded string
        // Traverse the tree based on the current bit (0 or 1)
        if (compressed[index] == '1') { //If the compressed is a one go left
            // Note: Again this is opposite as it should go right if it reads a 1 however our output has been backwards
            currentNode = currentNode->left;
        } else if (compressed[index] == '0') {
            //The opposite going right if reading 0
            currentNode = currentNode->right;
        } else {
            printf("Error: Invalid compressed data!\n");
            return;
        }

        //Leaf node reached (a character)
        if (currentNode->left == NULL && currentNode->right == NULL) {
            decompressed[decompressedIndex++] = currentNode->data;  // Add the character to the decompressed string
            currentNode = root;  // Reset to the root of the tree to decode the next character
        }

        index++;// Move to the next index of the encoded string
    }

    decompressed[decompressedIndex] = '\0';  //Add a null character to the end of the de-coded string
}
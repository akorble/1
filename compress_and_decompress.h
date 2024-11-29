
#define MAX 256
#ifndef CAMO_H
#define CAMO_H

#endif //CAMO_H
//
// Created by Cameron Brewster on 2024-11-20.
//
// huffman_tree.h: Define the structure and operations for the Huffman Tree.
// priority_queue.h: Define the priority queue for storing nodes.
// compression.h: Functions for compressing files.
// decompression.h: Functions for decompressing files.
// file_utils.h: Utility functions for reading and writing files.



#include "priority_queue.h"

// Create a Tree data structure for the Huffman Nodes


// A create node function
HuffmanNode *createLeafNode(char data, int frequency);
/* Parameters :
 a character to hold the node's symbol in tree
 an integer to hold the frequency associated with that character - Frequency of letters
 */



HuffmanNode *createInternalNodes(HuffmanNode* left, HuffmanNode *right);


HuffmanNode* buildHuffmanTree(Queue* queue);



void getCode(HuffmanNode *root, char codes[256][MAX], char* currentCode, int depth);


void freeHuffmanTree(HuffmanNode* root);

void compress(const char *filename, char *compressed, HuffmanNode *root, char codes[256][MAX]);


void decompress(const char *compressed, char *decompressed, HuffmanNode *root);
void HuffmanCodes(HuffmanNode *root, char codes[256][MAX]);
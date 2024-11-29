
#ifndef AKORBLEHUFFMAN_H
#define AKORBLEHUFFMAN_H

#endif //AKORBLEHUFFMAN_H
/*****************************************************************************
**
* Filename: priority_queue.h
* Author: Amelia Korble
* Student ID: 251 336 169
* Version: 3
* Date Created: Nov 25
* Last Modified: Nov 28
*
* Description:
* This .h file defines the structure and functionality required to implement a priority queue for use in building the Huffman code. Manages nodes with frequency data and ensures they are processed in the correct order based on priority (low-high frequency). It initializes and defines the key data structures (node_t, HuffmanNode, QueueNode, and Queue, all described below) required to manage the frequency data and tree-building process.
*
* Dependencies:
* N/A
*
* Compilation:
*gcc compress_and_decompress.c priority_queue.c ptest.c write_to_files.c -o 1
* ./1 <FileName.txt> <CompressedFileName.huf>
*****************************************************************************/

/*****************************************************************************
**
* Structure: node_t
*
* Purpose:
* Represents a basic data node used for storing an index and a weight value.
*Used for frequency tracking during the initial stages of the Huffman coding algorithm.
*
* Fields:
* index (int) - Represents the index/identifier associated with the data.
* weight (unsigned int) - Represents the weight/frequency of the data.
*
* Notes:
* - This structure is used to store frequency counts of characters from a file and their corresponding indices. It is used as part of an array for frequency tracking during Huffman tree construction.
* - The `index` field uniquely identifies each character in the file.
* - The `weight` field stores the frequency count for each character.
* Proper deallocation is used to prevent memory leaks
*****************************************************************************/
typedef struct {
    int index;
    unsigned int weight;
} node_t;

/*****************************************************************************
**
* Structure: HuffmanNode
*
* Purpose:
* Represents a node in the Huffman tree used for encoding and decoding data. This structure is part of the Huffman coding algorithm, where each node contains a frequency value, pointers to left and right child nodes, and the associated character data.
*
* Fields:
* frequency (int) - The frequency of the character/combined frequency of the subtree (used for building the tree).
* left (HuffmanNode*) - Pointer to the left child node in the Huffman tree.
* right (HuffmanNode*) - Pointer to the right child node in the Huffman tree.
* data (char) - The character associated with this node, relevant for leaf nodes in the Huffman tree.
*
* Notes:
* - This structure is used to represent both leaf and internal nodes in the Huffman tree. Leaf nodes store actual data characters, while internal nodes represent combined frequencies of child nodes.
* - The `left` and `right` pointers form the binary tree structure, where each node has at most two children.
* - Memory for individual `HuffmanNode` structures is dynamically allocated
* - The `frequency` field represents the node's weight in the tree and is used to build the tree by combining nodes with the least frequencies.
* - The `data` field is used for storing characters in leaf nodes. For internal nodes, this field can be left as a null character.
* - This struct is implemented in the other .h/.c files as well
*****************************************************************************/
typedef struct HuffmanNode {
    int frequency;
    struct HuffmanNode* left;
    struct HuffmanNode* right;
    char data;
} HuffmanNode;
/*****************************************************************************
**
* Structure: QueueNode
*
* Purpose:
* Represents a node in a linked list used to implement a queue. The queue is used in the construction of the Huffman tree, where each node stores a pointer to a `HuffmanNode` and points to the next node in the queue. This queue structure is essential in managing nodes during the tree-building process of Huffman coding.
*
* Fields:
* node (HuffmanNode*) - A pointer to the `HuffmanNode` associated with this queue node. This points to a specific Huffman tree node that will be processed during the algorithm.
* next (QueueNode*) - A pointer to the next `QueueNode` in the queue, forming a linked list for the queue structure.
*
* Notes:
* - This structure is used to implement a linked list-based queue for managing the `HuffmanNode` objects during the Huffman tree construction.
* - The `node` field holds the pointer to the actual `HuffmanNode`, which could be either a leaf node (containing character data) or an internal node (representing combined frequencies).
* - The `next` pointer links the current node to the next in the queue.
* - Memory for each `QueueNode` is dynamically allocated. Proper memory management is used to avoid memory leaks.
*****************************************************************************/
typedef struct QueueNode {
    HuffmanNode *node;
    struct QueueNode* next;
} QueueNode;
/*******************************************************************************
* Structure: Queue
*
* Purpose:
* Represents a queue that is implemented using a linked list of `QueueNode` structures. The queue is used to manage the Huffman nodes during sorting and merging nodes based on their frequency.
*
* Fields:
* front (QueueNode*) - A pointer to the front node in the queue, where nodes are dequeued from. This is the first node that will be processed in the queue.
* rear (QueueNode*) - A pointer to the rear node in the queue, where new nodes are enqueued. This is the last node in the queue, which gets updated when a new node is added.
*
* Notes:
* - This structure uses two pointers (`front` and `rear`) to efficiently manage the queue. The `front` pointer tracks the first element in the queue, while the `rear` pointer tracks the last element, allowing for efficient enqueue and dequeue operations.
* - The queue is built on top of a linked list of `QueueNode` structures.
* - Memory for `QueueNode` elements is dynamically allocated during the
*   enqueue operation.
* - The queue is used during the construction of the Huffman tree, where nodes are dequeued, merged, and re-enqueued as part of the algorithm.
*****************************************************************************/
typedef struct {
    QueueNode* front;
    QueueNode* rear;
} Queue;
/*****************************************************************************
**
* Function: countFrequencies
*
* Purpose:
* Reads a file and counts the frequency of each character in the file. This function stores the frequencies in a `node_t` array, where each node represents a character and its frequency. It also updates the`uniqueCharCount` to indicate the number of unique characters found.
*
* Parameters:
* filename (const char*) - The name of the file to read and count character frequencies.
* nodes (node_t*) - An array of `node_t` structures that will hold the index and frequency for each unique character in the file.
* uniqueCharCount (int*) - A pointer to an integer where the function will store the number of unique characters in the file.
*
* Returns:
* void - This function does not return a value. It modifies the `nodes` array and the `uniqueCharCount` parameter.
*
* Errors:
* - If the file cannot be opened, an error message is printed to `stderr` and the program exits with `exit(1)`.
* - It assumes that the file is a text file and handles characters using their ASCII values. Non-text files may lead to unexpected results.
*
* Notes:
* - The function uses an array of 256 integers (`frequencies[256]`) to count the occurrences of each character. The indices of this array represent the ASCII values of characters, and the values at those indices represent the number of occurrences.
* - Only characters with a non-zero frequency are added to the `nodes` array.
* - The file is read character by character using `fgetc`, which unfortunately may lead to inefficient performance for large files.
* - This function assumes the file is text-based.
*****************************************************************************/
void countFrequencies(const char *filename, node_t nodes[], int *uniqueCount);
/*****************************************************************************
**
* Function: priorityEnqueue
*
* Purpose:
* Inserts a new `HuffmanNode` into a priority queue based on its frequency.
* The nodes in the queue are ordered such that the node with the smallest frequency is at the front.
*
* Parameters:
* q (Queue*) - A pointer to the `Queue` where the new node will be inserted.
* newNode (HuffmanNode*) - A pointer to the `HuffmanNode` to be inserted into the queue.
*
* Returns:
* void - This function does not return a value. It modifies the state of the `Queue`.
*
* Errors:
* - If memory allocation for a new `QueueNode` fails, the function prints an error message to `stderr` and exits the program with `exit(1)`.
* - Assumes the queue and node pointers are valid. If a `NULL` pointer is passed for either `q` or `newNode`, undefined behavior may occur.
*
* Notes:
* - The queue is maintained in ascending order based on the frequency of the nodes. Nodes with smaller frequencies are given higher priority and are inserted at the front of the queue or at the correct position in the middle.
* - If the queue is empty, the new node is inserted at the front.
* - Memory for the new `QueueNode` is dynamically allocated.
* - If inserting a node with a frequency smaller than the current front node, the new node is inserted directly at the front of the queue.
* - If the new node has a larger frequency, it is inserted at the correct position by traversing the queue and comparing frequencies.
*****************************************************************************/
void priorityEnqueue(Queue *q, HuffmanNode *newNode);
/*****************************************************************************
**
* Function: priorityDequeue
*
* Purpose:
* Removes and returns the node with the smallest frequency from the front of the priority queue. The function maintains the priority queue by adjusting the front and rear pointers accordingly.
*
* Parameters:
* q (Queue*) - A pointer to the `Queue` from which the node will be dequeued.
*
* Returns:
* HuffmanNode* - A pointer to the dequeued `HuffmanNode` with the smallest frequency, or `NULL` if the queue is empty.
*
* Errors:
* - If the queue is empty, the function prints an error message to `stderr` and returns `NULL`. This prevents accessing invalid memory.
* - Assumes that the `Queue` pointer is valid. If a `NULL` pointer is passed for the queue, undefined behavior may occur.
*
* Notes:
* - The function dequeues the node at the front of the queue, adjusting the queue's front pointer. If the queue becomes empty after the operation, the rear pointer is also set to `NULL`.
* - The memory allocated for the `QueueNode` is freed after the node is dequeued.
* - If the queue is empty, the function returns `NULL`, indicating that there are no nodes to dequeue.
*****************************************************************************/
HuffmanNode* priorityDequeue(Queue *q);
/*****************************************************************************
**
* Function: initQueue
*
* Purpose:
* Initializes an empty queue by setting both the front and rear pointers of the queue to `NULL`.
*
* Parameters:
* q (Queue*) - A pointer to the `Queue` that will be initialized.
*
* Returns:
* void - The function does not return a value.
*
* Errors:
* - N/A
*
* Notes:
* - This function prepares a `Queue` for use by ensuring the front and rear pointers are properly set to `NULL`, which indicates an empty queue.
* - The function assumes that the queue passed is valid. If a `NULL` pointer is passed for the queue, undefined behavior will occur.
*****************************************************************************/
void initQueue(Queue *q);
/*****************************************************************************
**
* Function: freeQueue
*
* Purpose:
* Frees the memory allocated for all nodes in the queue. This function traverses the queue from front to rear, deallocates each node, and resets the `front` and `rear` pointers to `NULL` to indicate that the queue is empty after being freed.
*
* Parameters:
* q (Queue*) - A pointer to the `Queue` that will be freed.
*
* Returns:
* void - The function does not return a value.
*
* Errors:
* - N/A
*
* Notes:
* - This function is intended to release all dynamically allocated memory for the queue's nodes. It assumes the queue is valid.
* - After calling this function, the `Queue` object will be empty.
*****************************************************************************/
void freeQueue(Queue *q);

#include "compress_and_decompress.h"
#include <stdio.h>
#include <stdlib.h>

/*****************************************************************************
**
* Filename: priority_queue.c
* Author: Amelia Korble
* Student ID: 251 336 169
* Version: 3
* Date Created: Nov 25
* Last Modified: Nov 28
*
* Description:
* The countFrequencies function reads an input file, counts the occurrences of each character, and stores these frequencies in an array of node_t structures. Each node_t structure holds the ASCII value of the character (index) and its frequency (weight).
It updates the count of unique characters in the file and returns it
*priorityEnqueue inserts a new node into the queue in sorted order, ensuring that nodes are ordered by their frequency, with the smallest frequency node at the front of the queue
*priorityDequeue removes and returns the node_t from the front of the queue. The node with the smallest weight (highest priority) is dequeued first. If the queue is empty, the function will exit
*The initQueue and freeQueue functions do basic memory management (initialize and free the queue memory)
* Dependencies:
* stdio.h
*stdlib.h
*#include "compress_and_decompress.h"
*
*
* Compilation:
* gcc compress_and_decompress.c priority_queue.c ptest.c write_to_files.c -o 1
* ./1 <FileName.txt> <CompressedFileName.huf>
*****************************************************************************/
void countFrequencies(const char *filename, node_t *nodes, int *uniqueCharCount) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error: Unable to open file '%s'\n", filename);
        exit(1);
    }

    // Initialize frequency array
    int frequencies[256] = {0};

    // Read characters and count occurrences
    int c;
    while ((c = fgetc(file)) != EOF) {
        frequencies[c]++;
    }

    // Populate the nodes array with non-zero frequencies
    int nodeIndex = 0;
    for (int i = 0; i < 256; i++) {
        if (frequencies[i] > 0) {
            nodes[nodeIndex].index = i;
            nodes[nodeIndex].weight = frequencies[i];
            nodeIndex++;
        }
    }

    *uniqueCharCount = nodeIndex;
    fclose(file);
}

// Updated priorityDequeue function with debugging
HuffmanNode* priorityDequeue(Queue* q) {
    if (q->front == NULL) {
        fprintf(stderr, "Queue is empty! Cannot dequeue.\n");
        return NULL;  // Return NULL if the queue is empty
    }

    // Store the data of the front node to return
    QueueNode* temp = q->front;
    HuffmanNode* dequeuedNode = temp->node;

    // Move the front pointer to the next node
    q->front = q->front->next;

    // If the queue is now empty, set rear to NULL
    if (q->front == NULL) {
        q->rear = NULL;
    }

    // Free the memory of the dequeued node
    free(temp);

    return dequeuedNode;
}



// Updated priorityEnqueue function with debugging
void priorityEnqueue(Queue* q, HuffmanNode* newNode) {
    // Allocate memory for the new QueueNode
    QueueNode* newQueueNode = (QueueNode*)malloc(sizeof(QueueNode));
    if (!newQueueNode) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        exit(1);  // Exit if memory allocation fails
    }
    newQueueNode->node = newNode;
    newQueueNode->next = NULL;

    // If the queue is empty, insert at the front
    if (q->front == NULL) {
        q->front = q->rear = newQueueNode;
        return;
    }

    // If the new node has a smaller frequency than the front node, insert at the front
    if (newNode->frequency < q->front->node->frequency) {
        newQueueNode->next = q->front;
        q->front = newQueueNode;
        return;
    }

    // Traverse to find the correct insertion point (based on frequency)
    QueueNode* current = q->front;
    while (current->next != NULL && current->next->node->frequency <= newNode->frequency) {
        current = current->next;
    }

    // Insert the new node
    newQueueNode->next = current->next;
    current->next = newQueueNode;

    // If inserted at the end, update the rear pointer
    if (newQueueNode->next == NULL) {
        q->rear = newQueueNode;
    }
}


void initQueue(Queue *q) {
    q->front = NULL;// Front of the queue is set to NULL to indicate it's empty
    q->rear = NULL;// Rear of the queue is set to NULL to indicate it's empty
}
void freeQueue(Queue* q){
    QueueNode* current = q->front;// Start from the front of the queue
    // Iterate through all nodes in the queue
    while (current != NULL) {
        QueueNode* temp = current;// Temporarily store the current node
        current = current->next;// Move to the next node
        free(temp);// Free the memory of the current node
    }
    // Reset the front and rear pointers of the queue to indicate it's empty
    q->front = NULL;
    q->rear = NULL;
}


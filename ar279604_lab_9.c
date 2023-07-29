#include <stdio.h>
#include <stdlib.h>

// RecordType
struct RecordType
{
    int id;
    char name;
    int order;
};

struct Node
{
    struct RecordType record;
    struct Node* next;
};

// Fill out this structure
struct HashType
{
    struct Node* head; // Head of the linked list
};


// Compute the hash function
int hash(int x)
{
    return x % 10; // Assuming hash table size is 10
}

// parses input file to an integer array
int parseData(char* inputFileName, struct RecordType** ppData)
{
    FILE* inFile;
    int dataSz = 0;
    int i, n;
    char c;
    struct RecordType* pRecord;
    *ppData = NULL;

    if (fopen_s(&inFile, inputFileName, "r") == 0) // Using fopen_s
    {
        fscanf_s(inFile, "%d\n", &dataSz);
        *ppData = (struct RecordType*)malloc(sizeof(struct RecordType) * dataSz);
        // Implement parse data block
        if (*ppData == NULL)
        {
            printf("Cannot allocate memory\n");
            exit(-1);
        }
        for (i = 0; i < dataSz; ++i)
        {
            pRecord = *ppData + i;
            fscanf_s(inFile, "%d ", &n);
            pRecord->id = n;
            fscanf_s(inFile, " %c ", &c); // Added space to consume the newline character after reading the integer
            pRecord->name = c;
            fscanf_s(inFile, "%d ", &n);
            pRecord->order = n;
        }

        fclose(inFile);
    }

    return dataSz;
}

// prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
    int i;
    printf("\nRecords:\n");
    for (i = 0; i < dataSz; ++i)
    {
        printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
    }
    printf("\n\n");
}

// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType* pHashArray, int hashSz)
{
    for (int i = 0; i < hashSz; ++i)
    {
        struct Node* current = pHashArray[i].head;
        while (current != NULL)
        {
            printf("Index %d -> %d, %c, %d\n", i, current->record.id, current->record.name, current->record.order);
            current = current->next;
        }
    }
}


int main(void)
{
    struct RecordType* pRecords;
    int recordSz = 0;

    recordSz = parseData("input.txt", &pRecords);
    printRecords(pRecords, recordSz);

    // Your hash implementation
    int hashTableSize = 10; // Assuming hash table size is 10
    struct HashType* hashTable = (struct HashType*)malloc(sizeof(struct HashType) * hashTableSize);

    // Initialize the hash table
    for (int i = 0; i < hashTableSize; ++i)
    {
        hashTable[i].head = NULL; // Set all slots as empty linked lists
    }

    // Insert records into the hash table
    for (int i = 0; i < recordSz; ++i)
    {
        int index = hash(pRecords[i].id); // Calculate the hash index

        // Create a new node for the current record
        struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
        newNode->record = pRecords[i];
        newNode->next = NULL;

        if (hashTable[index].head == NULL)
        {
            // If the linked list is empty at the hash index, insert the new node as the head
            hashTable[index].head = newNode;
        }
        else
        {
            // If there are already nodes in the linked list, insert the new node at the end
            struct Node* current = hashTable[index].head;
            while (current->next != NULL)
            {
                current = current->next;
            }
            current->next = newNode;
        }
    }

    // Display records in the hash table
    printf("\nHash Table with Chaining:\n");
    for (int i = 0; i < hashTableSize; ++i)
    {
        struct Node* current = hashTable[i].head;
        while (current != NULL)
        {
            printf("Index %d -> %d, %c, %d\n", i, current->record.id, current->record.name, current->record.order);
            current = current->next;
        }
    }

    
    for (int i = 0; i < hashTableSize; ++i)
    {
        struct Node* current = hashTable[i].head;
        while (current != NULL)
        {
            struct Node* temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(pRecords);
    free(hashTable);

    return 0;
}
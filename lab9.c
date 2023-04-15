#include <stdio.h>
#include <stdlib.h>

// RecordType
struct RecordType
{
    int id;
    char name;
    int order;
    struct RecordType *next; // create a next pointer (for linkedlist structure)
};

// Fill out this structure
struct HashType
{
    struct RecordType *record; // create a pointer to a RecordType
};

// Compute the hash function
int hash(int x, int tableSize)
{
    // hash function is x % tableSize
    return x % tableSize;
}

// parses input file to an integer array
int parseData(char *inputFileName, struct RecordType **ppData)
{
    FILE *inFile = fopen(inputFileName, "r");
    int dataSz = 0;
    int i, n;
    char c;
    struct RecordType *pRecord;
    *ppData = NULL;

    if (inFile)
    {
        fscanf(inFile, "%d\n", &dataSz);
        *ppData = (struct RecordType *)malloc(sizeof(struct RecordType) * dataSz);
        if (*ppData == NULL)
        {
            printf("Cannot allocate memory\n");
            exit(-1);
        }
        for (i = 0; i < dataSz; ++i)
        {
            pRecord = *ppData + i;
            fscanf(inFile, "%d ", &n);
            pRecord->id = n;
            fscanf(inFile, "%c ", &c);
            pRecord->name = c;
            fscanf(inFile, "%d ", &n);
            pRecord->order = n;
            pRecord->next = NULL;
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

// insert record into the hash table
void insertRecord(struct HashType *hashTable, struct RecordType *record, int tableSize)
{
    // call the hash function to get the index
    int index = hash(record->id, tableSize);
    struct RecordType *curRecord = hashTable[index].record;
    // if the RecordType at that index is NULL
    if (curRecord == NULL)
    {
        // set 'record' equal to the HashType pointer in the table at index
        hashTable[index].record = record;
    }
    else
    {
        // else traverse to the end of the linkedlist and add 'record' to the end of it
        while (curRecord->next != NULL)
        {
            curRecord = curRecord->next;
        }
        curRecord->next = record;
    }
}

// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType *hashTable, int tableSize)
{
    struct RecordType *record;
    int i;

    for (i = 0; i < tableSize; i++)
    {
        printf("Index %d -> ", i);
        if (hashTable[i].record == NULL)
        {
            printf("NULL\n");
        }
        else
        {
            record = hashTable[i].record;
            while (record != NULL)
            {
                printf("%d, %c, %d", record->id, record->name, record->order);
                if (record->next != NULL)
                {
                    printf(" -> ");
                }
                else
                {
                    printf(" -> NULL\n");
                }
                record = record->next;
            }
        }
    }
}


int main(void)
{
    struct RecordType *pRecords;
    int recordSz = 0;
    int hashTableSize = 10;

    recordSz = parseData("input.txt", &pRecords);
    printRecords(pRecords, recordSz);

    // Initialize the hash table
    struct HashType *hashTable = (struct HashType *) calloc(hashTableSize, sizeof(struct HashType));
    if (hashTable == NULL)
    {
        printf("Cannot allocate memory\n");
        exit(-1);
    }

    // Insert each record into the hash table
    for (int i = 0; i < recordSz; i++)
    {
        insertRecord(hashTable, &pRecords[i], hashTableSize);
    }

    // Display the records in the hash table
    displayRecordsInHash(hashTable, hashTableSize);

    // Free the memory allocated for the hash table and the records
    free(hashTable);
    free(pRecords);

    return 0;
}

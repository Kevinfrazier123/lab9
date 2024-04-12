#include <stdio.h>
#include <stdlib.h>
#define HASHSIZE 23
//Kevin Frazier


// RecordType
struct RecordType
{
	int		id;
	char	name;
	int		order; 
};

// Node for linked list
struct Node {
    struct RecordType data;
    struct Node* next;
};

// Fill out this structure
struct HashType
{
	struct RecordType data;
    struct HashType* next;

};

// Compute the hash function
int hash(int x)
{
	 return x % HASHSIZE;

}

// parses input file to an integer array
int parseData(char* inputFileName, struct RecordType** ppData)
{
	FILE* inFile = fopen(inputFileName, "r");
	int dataSz = 0;
	int i, n;
	char c;
	struct RecordType *pRecord;
	*ppData = NULL;

	if (inFile)
	{
		fscanf(inFile, "%d\n", &dataSz);
		*ppData = (struct RecordType*) malloc(sizeof(struct RecordType) * dataSz);
		// Implement parse data block
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
void displayRecordsInHash(struct HashType *pHashArray, int hashSz)
{
	int i;

	for (i=0;i<hashSz;++i)
	{
		// if index is occupied with any records, print all
		struct HashType* current = pHashArray + i;
        if (current->data.id != 0) {
            printf("Index %d -> ", i);
            while (current != NULL) {
                printf("%d,%c,%d", current->data.id, current->data.name, current->data.order);
                current = current->next;
                if (current != NULL) {
                    printf(" -> ");
                }
            }
            printf("\n");
        }
		
	}
}

int main(void)
{
	struct RecordType *pRecords;
	int recordSz = 0;

	recordSz = parseData("input_lab_9.txt", &pRecords);
	printRecords(pRecords, recordSz);
	// Your hash implementation
	struct HashType hashTable[HASHSIZE] = {0};

    // Insert records into hash table
    for (int i = 0; i < recordSz; ++i) {
        struct RecordType record = pRecords[i];
        int index = hash(record.id);
        struct HashType* newNode = (struct HashType*)malloc(sizeof(struct HashType));
        if (!newNode) {
            printf("Memory allocation failed!\n");
            exit(-1);
        }
        newNode->data = record;
        newNode->next = NULL;

        // If no element exists at this index, create a new linked list
        if (hashTable[index].data.id == 0) {
            hashTable[index] = *newNode;
        } else {
            // If elements already exist, append to the end of the linked list
            struct HashType* temp = &hashTable[index];
            while (temp->next != NULL) {
                temp = temp->next;
            }
            temp->next = newNode;
        }
    }

    // Display records stored in hash table
    displayRecordsInHash(hashTable, HASHSIZE);

    // Free dynamically allocated memory
    for (int i = 0; i < HASHSIZE; ++i) {
        struct HashType* temp = hashTable[i].next;
        while (temp != NULL) {
            struct HashType* prev = temp;
            temp = temp->next;
            free(prev);
        }
    }
    free(pRecords);

    return 0;
}

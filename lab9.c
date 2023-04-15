//Justin Rimmeli
//5299451

#include <stdio.h>
#include <stdlib.h>
// RecordType
struct RecordType
{
	int	id;
	char	name;
	int	order; 
};

// Fill out this structure
struct HashType
{
	struct RecordType** hashArray;
	int hashSize;

};

// Compute the hash function
int hash(int x, int hashSize)
{
	return x % hashSize;
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
		if (pHashArray->hashArray[i] != NULL)
		{
			printf("Index %d -> %d, %c, %d\n", i, pHashArray->hashArray[i]->id, pHashArray->hashArray[i]->name, pHashArray->hashArray[i]->order);
		}
	}
}

int main(void)
{
	struct RecordType *pRecords;
	int recordSz = 0;

	recordSz = parseData("input.txt", &pRecords);
	printRecords(pRecords, recordSz);
	
	struct HashType hashArray;
	hashArray.hashSize = recordSz;
	
	hashArray.hashArray = (struct RecordType**)malloc(sizeof(struct RecordType*) * hashArray.hashSize);
	if (hashArray.hashArray == NULL)
	{
		printf("Cannot allocate memory\n");
		return -1;
	}
	
	for (int i = 0; i < hashArray.hashSize; ++i)
	{
		hashArray.hashArray[i] = NULL;
	}
	
	for (int i = 0; i < recordSz; ++i)
	{
		int index = hash(pRecords[i].id, hashArray.hashSize); // Compute hash index
		while (hashArray.hashArray[index] != NULL)
		{
			index = (index + 1) % hashArray.hashSize; // Move to next index if collision occurs
		}
		hashArray.hashArray[index] = &pRecords[i]; // Insert record into hash array
	}
	displayRecordsInHash(&hashArray, hashArray.hashSize);
	
	// Free dynamically allocated memory
	free(hashArray.hashArray);
	free(pRecords);

	return 0;
}

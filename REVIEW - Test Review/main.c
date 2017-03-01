#include "main.h"
#include "file1.h"
#include "file2.h"

/*
#define MAX_THREADS 8

static void* threadedFunction(void* args);
unsigned int global_int = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

struct Point {
	char label;
	double x;
	double y;
};

typedef struct {
	struct Point p1;
	struct Point p2;
	struct Point p3;
} Triangle;
*/

int main(int argc, char* argv[])
{
	int comparing(const void* a, const void* b);
	HeapADT heap = createHeap(comparing);
	/*
	unsigned int* num1 = malloc(sizeof(unsigned int));
	*num1 = 50;

	unsigned int* num2 = malloc(sizeof(unsigned int));
	*num2 = 70;

	unsigned int* num3 = malloc(sizeof(unsigned int));
	*num3 = 15;
	*/

	unsigned int num1 = 5;
	unsigned int num2 = 10;
	unsigned int num3 = 423;
	unsigned int num4 = 11;
	unsigned int num5 = 32;
	unsigned int num6 = 425;

	addData(heap, &num1);
	addData(heap, &num2);
	addData(heap, &num3);
	addData(heap, &num4);
	addData(heap, &num5);
	addData(heap, &num6);

	/*
	for (unsigned int i = 10; i--;)
	{
		printf("%d\n", i);
	}


	// attempt to open file
	FILE* file;
	if ((file = fopen(argv[1], "r")) == NULL)
	{
		fprintf(stderr, "ERROR: Failed to open file, %s\n", strerror(errno));
		return EXIT_FAILURE;
	}


	// ask for a number
	int number = 0;
	printf("> ");
	scanf("%d", &number);
	int obtainedNumber = 0;
	while (fgetc(file) != EOF)
	{
		if (fscanf(file, "%i", &obtainedNumber) &&
			obtainedNumber == number)
		{
			printf("found: %d\n", number);
			break;
		}
	}

	fclose(file);
	*/
	/*
	char* thing[1000] = { NULL };
	for (int i = 0; i < 1000; i++) {
		thing[i] = malloc(sizeof(char) * 10);
		strcpy(thing[i], "lollhilol");
		//printf("%c", i);
	}
	char value;
	value = *(char*)thing[0];
	printf("%#X\n", value);

	
	for (int i = 0; i < 1000; i++)
	{
		free(thing[i]);
	}

	value = *(char*)thing[0];
	printf("%#X\n", value);
	// Allocates space needed for the struct AND the points associated.
	// Data at points is initailzied
	Triangle *tri = (Triangle *)malloc(sizeof(Triangle));
	value = *(char*)&tri->p1.x;
	printf("%#X\n", value);
	printf("%c\n%g\n%g\n", tri->p1.label, tri->p1.x, tri->p1.y);
	printf("%c\n%g\n%g\n", tri->p2.label, tri->p2.x, tri->p2.y);
	printf("%c\n%g\n%g\n", tri->p3.label, tri->p3.x, tri->p3.y);

	// This just allocates the space needed for this struct, DOES NOT initalize it.
	struct Point *newPoint = (struct Point*)malloc(sizeof(struct Point));

	printf("%c\n%g\n%g\n", newPoint->label, newPoint->x, newPoint->y);
	*/

	/*
	hi();
	pthread_t threadIDs[MAX_THREADS] = { 0U };
	void* exitStatus = NULL;

	for (unsigned int i = MAX_THREADS; i; --i)
	{
		pthread_create(&threadIDs[i - 1], NULL, threadedFunction, (void*)&i);
		//pthread_detach(threadIDs[i - 1]);
	}

	for (unsigned int i = MAX_THREADS; i; --i)
		pthread_join(threadIDs[i - 1], &exitStatus);
	printf("DONE\n");
	*/
	return EXIT_SUCCESS;
}

int comparing(const void* a, const void* b)
{
	unsigned int num1 = *(unsigned int*)a;
	unsigned int num2 = *(unsigned int*)b;
	if (num1 > num2)
		return 1;
	else if (num1 < num2)
		return -1;
	else
		return 0;
}

/*
static void* threadedFunction(void* args)
{
	unsigned int theInt = *((unsigned int*)args);
	
	pthread_mutex_lock(&mutex);
	global_int++;
	printf("Num: %d\n", global_int);
	pthread_mutex_unlock(&mutex);
	pthread_exit(NULL);
}*/
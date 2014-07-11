#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int   global_num = 0;
char  global_str_arr [65536] = {'a'};

int main(int argc, char** argv)
{
	char* heap_var = NULL;
	int local_var = 0;

	printf("Address of function main 0x%lx\n", main);
	printf("Address of global_num 0x%lx\n", &global_num);
	printf("Address of global_str_arr 0x%lx ~ 0x%lx\n", &global_str_arr[0], &global_str_arr[65535]);
	printf("Top of stack is 0x%lx\n", &local_var);
	printf("Top of heap is 0x%lx\n", sbrk(0));

	heap_var = (char *)malloc(sizeof(char) * 127 * 1024);

	printf("Address of heap_var is 0x%lx\n", heap_var);
	printf("Top of heap after malloc is 0x%lx\n", sbrk(0));

	free(heap_var);
	heap_var = NULL;

	printf("Top of heap after free is 0x%lx\n", sbrk(0));

	return 0;
}


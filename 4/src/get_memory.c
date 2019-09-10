#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct linkedList {
	char* memory;
	int size;
	struct linkedList *next;
} linked;

void quit(void*);
void exec(char*);
void get(void*);
int split_args(char*);
void show(void*);

int length, size;

linked *head;

static const struct {
	const char *name;
	void (*func)(void*); 
} function_map [] = {
	{ "quit", quit },
	{ "get", get },
	{ "show", show }
};

int main()
{
	char ch, command[150];
	int i = 0;
	head = (linked*) malloc(sizeof(linked));
	for (;;) {
		ch = getchar();
		if(ch != '\n') command[i++] = ch;
		else {
			command[i] = '\0';
			exec(command);
			i = 0;
		}
	}
}

void exec(char* command) {
	int i;
	int argsPosition = split_args(command);
	for (i = 0; i < (int)(sizeof(function_map) / sizeof(function_map[0])); i++) {
		if (!strcmp(function_map[i].name, command) && function_map[i].func) {
			function_map[i].func(command+argsPosition);
			break;
		}
	}

}

int split_args(char* line) {
	int i = 0;
	while(*(line+(i++)) != ' ' && *(line+(i)) != '\0');
	if(*(line+i) == '\0') return 0;
	*(line+i-1) = '\0';
	return i;
}

void quit(void* data) {
	printf("%s\n", (char*)data);
	exit(0);
}

char sizeOfBytes[][5] = {" B\0", " KiB\0", " MiB\0", " GiB\0", " TiB\0"};

int power(int b, int e) {	
	return b * (e > 0 ? power(b, e-1) : 1);
}

void get(void* data) {
	size = 1, length = 1;
	int i;	
	char* interationLoop = strstr(data, "-l ");
	if (interationLoop && *(interationLoop+3) != '\0') {
		length = atoi(interationLoop+3);
	}
	char* sizeOf = strstr(data, "-s ");
	if (sizeOf && *(sizeOf+3) != '\0') {
		char* typeOf;
		for(i = 0; i < 5; i++){
			typeOf = strstr(data, sizeOfBytes[i]);
			if(typeOf) {
				*(typeOf) = '\0';
				break;
			}	
		}
		size = atoi(sizeOf+3) * power(1024, i-1);
	}

	for (i = 0; i < length; i++) {
		linked *new_struct_memory;
		new_struct_memory = (linked*) malloc(size * sizeof(linked));
		new_struct_memory->memory = (char*) malloc(size * sizeof(char));
		new_struct_memory->size = size;
		new_struct_memory->next = head->next;
		head->next = new_struct_memory;
	}
}


void show(void* data) {
	linked* tmp;
	int count = 0, freeMemory = 0;	
	char* sizeOf = strstr(data, "--free");
	if (sizeOf && *(sizeOf+3) != '\0') freeMemory = 1;
	for (tmp = head->next; tmp; tmp = tmp->next) {
		count += tmp->size;
		if (freeMemory) {
			head = head->next;
		}
	}
	int i;
	for (i = 0; count/1024 && i < 4; count /= 1024) i++;
	printf("Using %d%s\n", count, sizeOfBytes[i]);
	while(freeMemory && head) {
		tmp = head;
		head = head->next;
		free(tmp->memory);
		free(tmp);
	}
}
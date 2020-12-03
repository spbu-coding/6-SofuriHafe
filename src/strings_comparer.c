#include <stdio.h>
#include <stdlib.h>
#include "sortings.h"

typedef void (*sort_type_t)(strings_array_t, array_size_t, comparator_func_t);

struct arguments
{
	array_size_t size;
	sort_type_t sort_type;
	comparator_func_t cmp;
};

int asc(const char* str1, const char* str2) {return strcmp(str1, str2);}

int des(const char* str1, const char* str2) {return -strcmp(str1, str2);}

int get_args(int argc, char* argv[], struct arguments* input_argc)
{
	if(argc != 6)
	{
		printf("Five arguments are required");

		return -1;
	}

	char *p = NULL;
	const char *size_pointer = argv[1];
	if(strtol(argv[1], &p, 10) < 0 || size_pointer == p)
	{
		printf("Weird argument");

		return -1;
	}

	else input_argc->size = strtoul(argv[1], NULL, 10);

	if(strstr(argv[2], ".txt") == NULL)
	{
		printf("!.txt files only!");

		return -1;
	}

	if(strstr(argv[3], ".txt") == NULL)
	{
		printf("!.txt files only!");

		return -1;
	}

	if(strcmp(argv[4], "bubble") == 0 ) input_argc->sort_type = bubble;

	else if(strcmp(argv[4], "insertion") == 0 ) input_argc->sort_type = insertion;

	else if(strcmp(argv[4], "merge") == 0 ) input_argc->sort_type = merge;

	else if(strcmp(argv[4], "quick") == 0) input_argc->sort_type = quick;

	else if(strcmp(argv[4], "radix") == 0) input_argc->sort_type = radix;

	else
	{
		printf("Sorting method isn't supported");

		return -1;
	}

	if(strcmp(argv[5], "asc") == 0) input_argc->cmp = asc;

	else if(strcmp(argv[5], "des") == 0) input_argc->cmp = des;

	else
	{
		printf("Comparator is not supported");

		return -1;
	}

	return 0;
}

int read_file(FILE* input_file, struct arguments* option, strings_array_t array)
{
	for(size_t i = 0; i < option->size; i++)
	{
		if(!feof(input_file))
		{
			if(fgets(array[i], MAX_INPUT_STRING_SIZE, input_file) == NULL)
			{
				printf("Failed getting values");

				return -1;
			}
		}

		else if(feof(input_file) && i < option->size)
		{
			printf("Real strings number is less than requested");

			return -1;
		}
	}

	return 0;
}

int main(int argc, char* argv[])
{
	struct arguments input_argc;
	if(get_args(argc, argv, &input_argc) < 0) return -1;

	strings_array_t array = (char**)malloc(sizeof(char*) * input_argc.size);

	if(array != NULL)
	{
		for(size_t i = 0; i < input_argc.size; i++)
		{
			array[i] = (char*)malloc(sizeof(char) * MAX_INPUT_STRING_SIZE);

			if(array[i] == NULL)
			{
				for(size_t j = 0; j < input_argc.size; j++) free(array[j]);

				free(array);

				array = NULL;

				printf("Unable to allocate memory\n");
				return -1;
			}
		}
	}

	else if(array == NULL)
	{
		printf("Unable to allocate memory\n");

		return -1;
	}

	FILE *input_file = fopen(argv[2], "r");

	if(input_file == NULL)
	{
		for(size_t i = 0; i < input_argc.size; i++) free(array[i]);
		free(array);

		array = NULL;

		printf("Unable to open file\n");

		return -1;
	}

	if(read_file(input_file, &input_argc, array) < 0)
	{
		fclose(input_file);

		for(size_t i = 0; i < input_argc.size; i++) free(array[i]);

		free(array);
		array = NULL;

		printf("Unable to read file\n");

		return -1;
	}

	if(input_argc.size > 0) input_argc.sort_type(array, input_argc.size, input_argc.cmp);

	FILE* output_file = fopen(argv[3], "w");

	if(output_file == NULL)
	{
		fclose(input_file);

		for(size_t i = 0; i < input_argc.size; i++) free(array[i]);

		free(array);

		array = NULL;

		printf("Ouput file is missing or Unable to open\n");

		return -1;
	}

	if(input_argc.size == 0)
	{
		if(fputs("\n", output_file) == EOF)
		{
			printf("Unable to write to file");

			return -1;
		}
	}

	else
	{
		for(size_t i = 0; i < input_argc.size; i++)
		{
			if(fputs(array[i], output_file) != EOF)
			{
				if(strcspn(array[i], "\n") == strlen(array[i]))
				{
					if(fputs("\n", output_file) == EOF)
					{
						printf("Unable to write to file\n");

						return -1;
					}
				}
			}

			else
			{
				printf("Unable to write to file");

				return -1;
			}
		}
	}
	
	fclose(input_file);

	fclose(output_file);
 
	for(size_t i = 0; i < input_argc.size; i++) free(array[i]);

	free(array);

	array = NULL;

	return 0;
}
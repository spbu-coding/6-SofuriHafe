#include "sortings.h"
#include <stdbool.h>
#include <stdlib.h>

#define ASCII_LENGTH 128
#define CHAR_SET 256

void swap(char **str1, char **str2)
{
	char *tmp = *str2;
	*str2 = *str1;
	*str1 = tmp;
}

void bubble(strings_array_t array, array_size_t size, comparator_func_t cmp)
{
	for(size_t i = 0; i < size - 1; i++)
	{
		for(size_t j = 0; j < size - 1; j++)
		{
			if(cmp(array[j], array[j + 1]) > 0) swap(&array[j], &array[j + 1]);
		}
	}
}

void insertion(strings_array_t array, array_size_t size, comparator_func_t cmp)
{
	for(size_t i = 1; i < size; i++)
	{
		for(size_t j = i; j > 0; j--)
		{
			if(cmp(array[j - 1], array[j]) > 0) swap(&array[j - 1], &array[j]);
		}
	}
}

void merge_part2(strings_array_t array, int left, int middle, int right, comparator_func_t cmp)
{
	int lsize = middle - left + 1;
	int rsize = right - middle;

	char *lbuff[lsize], *rbuff[rsize];

	for(int i = 0; i < lsize; i++) lbuff[i] = array[left + i];

	for(int j = 0; j < rsize; j++) rbuff[j] = array[middle + 1 + j];

	int i = 0;
	int j = 0;
	int k = left;

	while(i < lsize && j < rsize)
	{
		if(cmp(lbuff[i], rbuff[j]) < 0)
		{
			array[k] = lbuff[i];
			i++;
		}

		else
		{
			array[k] = rbuff[j];
			j++;
		}

		k++;
	}

	while(i < lsize)
	{
		array[k] = lbuff[i];
		i++;
		k++;
	}

	while(j < rsize)
	{
		array[k] = rbuff[j];
		j++;
		k++;
	}
}

void merge_part1(strings_array_t array, int left, int right, comparator_func_t cmp)
{
    if(left >= right) return;

    int middle = (left + right - 1) / 2;

	merge_part1(array, left, middle, cmp);
	merge_part1(array, middle + 1, right, cmp);
	merge_part2(array, left, middle, right, cmp);
}

void merge(strings_array_t array, array_size_t size, comparator_func_t cmp)
{
	merge_part1(array, 0, (int)size - 1, cmp);
}

int quick_part2(strings_array_t array, int left, int right, comparator_func_t cmp)
{
	char *p = array[right];
	int i = (left - 1);

	for(int j = left; j <= right - 1; j++)
	{
		if(cmp(array[j], p) < 0)
		{
			i++;

			swap(&array[i], &array[j]);
		}
	}

	swap(&array[i + 1], &array[right]);

	return (i + 1);
}

void quick_part1(strings_array_t array, int left, int right, comparator_func_t cmp)
{
	if(left < right)
	{
		int middle = quick_part2(array, left, right, cmp);
		quick_part1(array, left, middle - 1, cmp);
		quick_part1(array, middle + 1, right, cmp);
	}
}

void quick(strings_array_t array, array_size_t size, comparator_func_t cmp)
{
	quick_part1(array, 0, (int) size - 1, cmp);
}

void radix(strings_array_t array, array_size_t size, comparator_func_t cmp)
{
	const bool is_asc = (cmp("a", "b") < 0);

	array_size_t max_length = 0;

	for(size_t i = 0; i < size; i++)
	{
		array_size_t length = strlen(array[i]);

		if(max_length < length) max_length = length;
	}

	if(max_length == 0) return;

	size_t buff[CHAR_SET];
	strings_array_t tmp = (strings_array_t)(malloc(size * sizeof(char *)));

	for(size_t i = max_length; i > 0; i--)
	{
		size_t counter = 0;

		for(size_t j = 0; j < CHAR_SET; j++) buff[j] = 0;

		for(size_t j = 0; j < size; j++) buff[(unsigned char)(array[j][i - 1])]++;

		if(is_asc)
		{
			for(size_t j = 0; j < CHAR_SET; j++)
			{
				size_t value = buff[j];
				buff[j] = counter;
				counter += value;
			}
		}

		else
		{
			for(size_t j = CHAR_SET; j > 0; j--)
			{
				size_t value = buff[j - 1];
				buff[j - 1] = counter;
				counter += value;				
			}
		}

		for(size_t j = 0; j < size; j++)
		{
			tmp[buff[(unsigned char)array[j][i - 1]]] = array[j];
			buff[(unsigned char)array[j][i - 1]]++;
		}

		for(size_t j = 0; j < size; j++) array[j] = tmp[j];
	}

	free(tmp);
}
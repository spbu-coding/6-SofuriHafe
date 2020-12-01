#include "sortings.h"

#define ASCII_LENGTH 128

void swap(char **str1, char **str2)
{
	char *tmp = *str2;
	*str2 = *str1;
	*str1 = tmp;
}

void bubble(strings_array_t array, array_size_t size, comparator_func_t cmp)
{
	for(int i = 0; i < size - 1; i++)
	{
		for(int j = 0; j < size - 1; j++)
		{
			if(cmp(array[j], array[j + 1]) > 0) swap(&array[j], &array[j + 1]);
		}
	}
}

void insertion(strings_array_t array, array_size_t size, comparator_func_t cmp)
{
	for(int i = 1; i < size; i++)
	{
		for(int j = i; j > 0; j--)
		{
			if(cmp(array[j - 1], array[j]) > 0) swap(&array[j - 1], &array[j]);
		}
	}
}

void merge(strings_array_t array, array_size_t size, comparator_func_t cmp)
{
	for (int parts_num = size; parts_num > 1; parts_num = 1 + (parts_num - 1) / 2)
	{
		int part_len = 1 + (size - 1) / parts_num;

		for (int i = 0; i < parts_num - 1; i += 2)
		{
			int len1 = part_len, len2 = (((i + 2) * part_len) <= size) ? part_len : (size - (i + 1) * part_len), buf_len = len1 + len2;

			int ind1 = i * part_len, ind2 = (i + 1) * part_len;

			char* buf[buf_len];

			while (len1 > 0 && len2 > 0)
			{
				if (cmp(array[ind1], array[ind2]) <= 0) buf[buf_len - (len1-- + len2)] = array[ind1++];

				else buf[buf_len - (len1 + len2--)] = array[ind2++];
			}

			if (len1 > 0) memcpy(&buf[buf_len - len1], &array[ind1], len1 * sizeof(char*));

			else memcpy(&buf[buf_len - len2], &array[ind2], len2 * sizeof(char*));

			memcpy(&array[i * part_len], buf, buf_len * sizeof(char*));
		}
	}
}

void quick(strings_array_t array, array_size_t size, comparator_func_t cmp)
{
	int left = 0;
	int right = size - 1;
	int middle = size / 2;

	do
	{
		while(cmp(array[left], array[middle]) < 0) left++;

		while(cmp(array[middle], array[right]) < 0) right--;

		if (left <= right)
		{
			swap(&array[left], &array[right]);

			left++;
			right--;
		}
	} while (left <= right);

	if(right > 0) quick(array, right + 1, cmp);

	if (left < size) quick(array + left, size - left, cmp);
}

void radix(strings_array_t array, array_size_t size, comparator_func_t cmp)
{
	const _Bool is_asc_order = (cmp("a", "b") < 0);

	int strs_len[size], max_str_len = 0;

	for (int i = 0; i < size; i++)
	{
		strs_len[i] = strlen(array[i]) - 1;

		if (strs_len[i] > max_str_len) max_str_len = strs_len[i];
	}

	for (int i = (int) max_str_len - 1; i >= 0; i--)
	{
		int char_counter[ASCII_LENGTH] = {0};

		for (int j = 0; j < size; j++)
		{
			if (strs_len[j] - 1 >= i) char_counter[array[j][i]]++;

			else char_counter[0]++;
		}

		if (is_asc_order)
		{
			for (int j = 1; j < ASCII_LENGTH; j++) char_counter[j] += char_counter[j - 1];
		}

		else
		{
			for (int j = ASCII_LENGTH - 2; j >= 0; j--) char_counter[j] += char_counter[j + 1];
		}

		char* buf[size];

		int buf_l[size];

		for (int j = size - 1; j >= 0; j--)
		{
			if (strs_len[j] - 1 >= i)
			{
				buf[(char_counter[array[j][i]]) - 1] = array[j];
				buf_l[(char_counter[array[j][i]]--) - 1] = strs_len[j];
			}

			else
			{
				buf[(char_counter[0]) - 1] = array[j];
				buf_l[(char_counter[0]--) - 1] = strs_len[j];
			}
		}

		memcpy(array, buf, size * sizeof(char*));
		memcpy(strs_len, buf_l, size * sizeof(int));
	}
}
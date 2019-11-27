#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "chat1002.h"
#define MAX_WORD_LENGTH 100
#define MAX_MEANING_LENGTH 4000

typedef struct dictionary
{
	char *word;
	char *meaning;
	struct dictionary * next;
}

dict;
dict *dict_head = NULL;

int dict_lookup(char *query, char *response, int n)
{
	dict *cursor = dict_head;
	while (cursor != NULL)
	{
		if (compare_token(query, cursor->word) == 0)
		{
			snprintf(response, n, "The Oxford definition of %s is as below:\n%s", query, cursor->meaning);
			return KB_OK;
		}

		cursor = cursor->next;
	}

	return KB_NOTFOUND;
}

void dict_free()
{
	dict *cursor = NULL;
	while (dict_head != NULL)
	{
		cursor = dict_head;
		dict_head = dict_head->next;
		free(cursor->word);
		free(cursor->meaning);
		free(cursor);
	}
}

void dict_load()
{
	FILE *f = fopen("Oxford English Dictionary.txt", "r");
	if (f == NULL) return;

	while (!feof(f))
	{
		char temp[MAX_WORD_LENGTH + MAX_MEANING_LENGTH];
		memset(temp, '\0', MAX_WORD_LENGTH + MAX_MEANING_LENGTH);
		fgets(temp, sizeof(temp), f);
		if (strlen(temp) == 1)
		{
			continue;
		}

		char *token = strtok(temp, "==");
		dict *new = (dict*) malloc(sizeof(dict));
		if (new == NULL)
		{
			perror("Dictionary allocation failure");
			fclose(f);
			dict_free();
			return;
		}

		new->word = strdup(token);
		new->meaning = strdup(strtok(NULL, "=="));
		new->next = NULL;
		if (dict_head == NULL)
		{
			dict_head = new;
		}
		else
		{
			new->next = dict_head;
			dict_head = new;
		}
	}

	fclose(f);

}
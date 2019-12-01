#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "chat1002.h"
#define MAX_WORD_LENGTH 100
#define MAX_MEANING_LENGTH 4000

typedef struct dictionary // Struct to use as LL for dictionary
{
	char word[MAX_WORD_LENGTH];
	char meaning[MAX_MEANING_LENGTH];
	struct dictionary * next;
}

dict;

dict *dict_head = NULL; // Dictionary head to point to dictionary LL


/*
 *Search the dictionary for word and it's corresponding meaning.
 *
 *Input:
 *  query - String query word to search for
 *
 *Return:
 *  KB_OK - Definition found for word
 *  KB_NOTFOUND - Definition not found
 */
int dict_lookup(char *query)
{
	dict *cursor = dict_head; // Pointer to iterate LL
	while (cursor != NULL)
	{
		if (compare_token(query, cursor->word) == 0) // If USER query matches dictionary word
		{
			printf("%s: The Oxford definition of %s is as below:\n\n%s\n\n", chatbot_botname(), query, cursor->meaning); // Print formatted word and meaning
			return KB_OK;
		}

		cursor = cursor->next;
	}

	return KB_NOTFOUND;
}

void dict_free()
{
	dict *cursor = NULL; // Pointer to iterate LL
	while (dict_head != NULL)
	{
		cursor = dict_head;
		dict_head = dict_head->next;
		//Free memory used by Dict
		free(cursor);
	}
}

void dict_load()
{
	FILE *f = fopen("Oxford English Dictionary.txt", "r"); // Open file for reading
	if (f == NULL){ // If unable to open file, resume program flow
		printf("Unable to read Oxford English Dictionary.txt, meaning function diabled.\n");
		return; 
	}


	while (!feof(f)) // Read entire file
	{
		char temp[MAX_WORD_LENGTH + MAX_MEANING_LENGTH]; // Create buffer to read in entire line
		memset(temp, '\0', MAX_WORD_LENGTH + MAX_MEANING_LENGTH); // Initialise buffer
		fgets(temp, sizeof(temp), f);
		if (strlen(temp) == 1) //Skip empty line
		{
			continue;
		}

		char *token = strtok(temp, "=="); // Split string by delimeter
		dict *new = (dict*) malloc(sizeof(dict)); // Allocate memory for new node
		if (new == NULL) // If unable to allocate memory
		{
			//Print error and free dictionary, then resume program flow
			perror("Dictionary allocation failure, meaning function disabled. Reason");
			fclose(f);
			dict_free();
			return;
		}

		// Copies word and meaning to dict node
		strncpy(new->word, token, MAX_WORD_LENGTH);
		strncpy(new->meaning, strtok(NULL, "=="), MAX_MEANING_LENGTH);
		new->next = NULL;
		if (dict_head == NULL)
		{
			dict_head = new; // If LL empty, set node as head
		}
		else
		{
			new->next = dict_head; // IF not empty, prepend node
			dict_head = new;
		}
	}

	fclose(f);

}
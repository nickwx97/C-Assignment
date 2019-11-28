/*
 *ICT1002 (C Language) Group Project, AY19 Trimester 1.
 *
 *This file implements the chatbot's knowledge base.
 *
 *knowledge_get() retrieves the response to a question.
 *knowledge_put() inserts a new response to a question.
 *knowledge_read() reads the knowledge base from a file.
 *knowledge_reset() erases all of the knowledge.
 *kowledge_write() saves the knowledge base in a file.
 *
 *You may add helper functions as necessary.
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "chat1002.h"

typedef struct header // Struct to use as LL to store intent and pointer to question and answer
{
	char *intent;
	struct row * content;
	struct header * next;
}

header;

typedef struct row // Struct to use as LL to store question and answer
{
	char *question;
	char *answer;
	struct row * next;
}

row;

header *k_arr = NULL; // Linked List head (A.K.A. Knowledge Base)

/*
 *Get the response to a question.
 *
 *Input:
 *  intent   - the question word
 *  entity   - the entity
 *  response - a buffer to receive the response
 *  n        - the maximum number of characters to write to the response buffer
 *
 *Returns:
 *  KB_OK, if a response was found for the intent and entity (the response is copied to the response buffer)
 *  KB_NOTFOUND, if no response could be found
 *  KB_INVALID, if 'intent' is not a recognised question word
 */
int knowledge_get(const char *intent, const char *entity, char *response, int n)
{
	header *cursor = k_arr; // Pointer to iterate first layer of KB
	while (cursor != NULL)
	{
		if (compare_token(cursor->intent, intent) == 0) // If KB intent matches USER intent
		{
			row *incursor = cursor->content; // pointer to iterate LL of Q&As
			while (incursor != NULL)
			{
				if (compare_token(incursor->question, entity) == 0) // IF KB question matches USER question
				{
					snprintf(response, n, incursor->answer); // Store answer in response buffer if match
					return KB_OK;
				}

				incursor = incursor->next;
			}

			return KB_NOTFOUND;
		}

		cursor = cursor->next;
	}

	return KB_NOTFOUND;
}

/*
 *Removes the question.
 *
 *Input:
 *  intent   - the question word
 *  entity   - the entity
 *
 *Returns:
 *  KB_OK, if a response was found for the intent and entity (and if successfully removed from KB)
 *  KB_NOTFOUND, if no question could be found
 */
int knowledge_delete(const char *intent, const char *entity)
{
	header *cursor = k_arr; // Pointer to iterate first layer of KB
	while (cursor != NULL)
	{
		if (compare_token(cursor->intent, intent) == 0) // If KB intent matches USER intent
		{
			row *incursor = cursor->content; // Pointer to iterate Q&As of intent
			row *prevcursor = NULL; // Pointer to store previous node to prepare for delete
			while (incursor != NULL)
			{
				if (compare_token(incursor->question, entity) == 0) // If KB question matches USER question
				{
					if (prevcursor != NULL) // If node is NOT the first item in the Content LL
					{
						prevcursor->next = incursor->next;
					}
					else // If node is the first item in the Content LL
					{
						cursor->content = incursor->next;
					}

					/* Release memory used to store Deleted Knowledge */
					free(incursor->question);
					free(incursor->answer);
					free(incursor);
					return KB_OK;
				}

				prevcursor = incursor;
				incursor = incursor->next;
			}

			return KB_NOTFOUND;
		}

		cursor = cursor->next;
	}

	return KB_NOTFOUND;
}

/*
 *Insert a new response to a question. If a response already exists for the
 *given intent and entity, it will be overwritten. Otherwise, it will be added
 *to the knowledge base.
 *
 *Input:
 *  intent    - the question word
 *  entity    - the entity
 *  response  - the response for this question and entity
 *
 *Returns:
 *  KB_FOUND, if successful
 *  KB_NOMEM, if there was a memory allocation failure
 *  KB_INVALID, if the intent is not a valid question word
 */
int knowledge_put(const char *intent, const char *entity, const char *response)
{
	header *cursor = k_arr; // Pointer to iterate KB
	
	/* Initialise variables to store entity and response */
	char qn[strlen(entity)];
	char ans[strlen(response)];
	memset(qn, '\0', strlen(entity));
	memset(ans, '\0', strlen(response));

	for (int i = 0; i < strlen(entity); ++i)
	{
		qn[i] = tolower(entity[i]); // Converts entity to lowercase and store
	}

	for (int i = 0; i < strlen(response); ++i)
	{
		if (response[i] != '\n')
			ans[i] = response[i]; // Stores reponse stripped of newline to maintain formatting
	}

	/* Terminate the "strings" */
	qn[strlen(entity)] = '\0';
	ans[strlen(ans)] = '\0';

	if (cursor == NULL) // If KB is uninitialized
	{
		header *newh = (header*) malloc(sizeof(struct header)); // Create new header
		row *newr = (row*) malloc(sizeof(struct row)); // Create new row

		char n_intent[strlen(intent) + 1];
		memset(n_intent, '\0', sizeof(n_intent));
		for (int i = 0; i < strlen(intent); ++i)
		{
			n_intent[i] = tolower(intent[i]); // Convert intent to lowercase and store
		}

		newh->intent = strdup(n_intent); // Malloc and copy intent to struct
		newh->next = NULL;

		newr->question = strdup(qn); // Malloc and copy question to struct
		newr->answer = strdup(ans); // Malloc and copy answer to struct
		newr->next = NULL;

		newh->content = newr; // Set pointer in header to point to new row
		k_arr = newh; // Set KB head to point to new header

		return KB_OK;
	}

	while (cursor != NULL) // Iterate KB
	{
		if (compare_token(cursor->intent, intent) == 0) // If KB intent match USER intent
		{
			row *new = (row*) malloc(sizeof(struct row)); // Create new row
			new->question = strdup(qn); // Malloc and copy question to struct
			new->answer = strdup(ans); // Malloc and copy answer to struct
			
			// Prepend the new row to existing list
			new->next = cursor->content;
			cursor->content = new;
			return KB_OK;
		}
		else if (cursor->next == NULL) // If KB intent does not exists for USER intent
		{
			header *newh = (header*) malloc(sizeof(struct header)); // Create new header
			row *newr = (row*) malloc(sizeof(struct row)); //Create new row

			char n_intent[strlen(intent) + 1];
			memset(n_intent, '\0', sizeof(n_intent));
			for (int i = 0; i < strlen(intent); ++i)
			{
				n_intent[i] = tolower(intent[i]); // Converts intent to lower and store
			}

			newh->intent = strdup(n_intent); // Malloc and copy intent to struct
			newh->next = k_arr;

			newr->question = strdup(qn); // Malloc and copy question to struct
			newr->answer = strdup(ans); // Malloc and copy answer to struct
			newr->next = NULL;

			newh->content = newr; // Prepend new intent to KB
			k_arr = newh;

			return KB_OK;
		}

		cursor = cursor->next;
	}

	return KB_INVALID;
}

/*
 *Read a knowledge base from a file.
 *
 *Input:
 *  f - the file
 *
 *Returns: the number of entity/response pairs successful read from the file
 */
int knowledge_read(FILE *f)
{
	if (NULL != f) // If file is not NULL, check file size
	{
		fseek(f, 0, SEEK_END);
		int size = ftell(f);
		rewind(f);
		if (0 == size)
		{
			return 0; // If file is blank, return 0 lines read
		}
	}

	int num_of_lines = 0;
	knowledge_reset(); // Reset knowledge base before reading file
	header *cursor = k_arr; // Pointer to KB
	while (!feof(f)) // Read the entire file
	{
		char temp[MAX_INPUT];
		fgets(temp, MAX_INPUT, f);
		if (strlen(temp) == 1)
		{
			continue; //Skip line if blank
		}

		if (strchr(temp, '[') != NULL && strchr(temp, ']') != NULL && strchr(temp, '=') == NULL) // If read in header
		{
			int len = strlen(temp);
			// Strip square brackets from header
			for (int i = 1; i < len - 1; i++)
			{
				temp[i - 1] = temp[i];
			}
			temp[len - 3] = '\0';

			header *new = (header*) calloc(1, sizeof(struct header)); // Create new header
			if (k_arr == NULL) // If KB is empty, set header as new KB head
			{
				new->intent = strdup(temp);
				new->content = NULL;
				new->next = NULL;
				k_arr = new;
				cursor = new;
			}
			else // If not empty, prepend header
			{
				new->intent = strdup(temp);
				new->content = NULL;
				new->next = k_arr;
				k_arr = new;
				cursor = new;
			}
		}
		else // If not header, means is data line
		{
			num_of_lines++; // Increment num of lines
			int alen = strlen(strchr(temp, '=')) - 1, qlen = strlen(temp) - alen + 1; // Calculate length of question and answer
			// Initialise question and answer
			char qn[qlen];
			char ans[alen];
			memset(qn, '\0', qlen + 1);
			memset(ans, '\0', alen + 1);

			int check = 1, q = 0, a = 0;
			// Store into vairables, strip answer of newline and convert question into lowercase
			for (int i = 0; i < strlen(temp); ++i)
			{
				if (temp[i] == '=')
				{
					check = 0;
					continue;
				}

				if (check)
				{
					qn[q++] = tolower(temp[i]);
				}
				else
				{
					if (temp[i] != '\n')
						ans[a++] = temp[i];
				}
			}

			// Create new row and store question and answer
			row *new = (row*) calloc(1, sizeof(struct row));
			new->question = strdup(qn);
			new->answer = strdup(ans);
			if (cursor->content == NULL) // If content for cursor is empty
			{
				new->next = NULL;
			}
			else // If not empty
			{
				new->next = cursor->content;
			}

			cursor->content = new;
		}
	}

	return num_of_lines;
}

/*
 *Reset the knowledge base, removing all know entitities from all intents.
 */
void knowledge_reset()
{
	header *temp = NULL; // Create pointer to iterate through KB LL
	row *r_temp1 = NULL, *r_temp2 = NULL; // create pointer to iterate through Q&As
	while (k_arr != NULL)
	{
		temp = k_arr;
		r_temp1 = temp->content;
		while (r_temp1 != NULL)
		{
			r_temp2 = r_temp1;
			r_temp1 = r_temp1->next;
			// Free memory used by Q&As
			free(r_temp2->question);
			free(r_temp2->answer);
			free(r_temp2);
		}

		k_arr = k_arr->next;
		//Free memory used by KB
		free(temp->intent);
		free(temp);
	}
}

/*
* Checks if KB is empty
*
* Returns 1 if empty, 0 is not empty
*/
int knowledge_is_empty()
{
	if (k_arr == NULL) // If KB head is not set, means empty
	{
		return 1;
	}
	else
	{
		header *cursor = k_arr;
		while (cursor != NULL)
		{
			if (cursor->content != NULL) // If there is at least 1 Q&A
			{
				return 0;
			}

			cursor = cursor->next;
		}

		return 1; // If there is no Q&As in KB
	}
}

/*
 *Write the knowledge base to a file.
 *
 *Input:
 *  f - the file
 *
 *Return:
 *  number of lines as int OR
 *  -1 if no lines written
 */
int knowledge_write(FILE *f)
{
	int num_of_rows = 0;
	header *cursor = k_arr; // Create pointer to iterate KB
	row *row_cursor = NULL; // Create pointer to iterate Q&As

	while (cursor != NULL)
	{
		// Write intent to file
		fwrite("[", 1, 1, f);
		fwrite(cursor->intent, strlen(cursor->intent), 1, f);
		fwrite("]\n", 2, 1, f);

		row_cursor = cursor->content;
		while (row_cursor != NULL)
		{
			// Write Q&A to file
			fwrite(row_cursor->question, strlen(row_cursor->question), 1, f);
			fwrite("=", 1, 1, f);
			fwrite(row_cursor->answer, strlen(row_cursor->answer), 1, f);
			if (row_cursor->next != NULL)
				fwrite("\n", 1, 1, f);
			row_cursor = row_cursor->next;
			num_of_rows++; // increment row counter
		}

		if (cursor->next != NULL)
			fwrite("\n", 1, 1, f); // Write a new line for every line except last line
		cursor = cursor->next;
	}

	return num_of_rows == 0 ? -1 : num_of_rows; // Returns the number of lines, if 0 lines, returns -1 instead
}
/*
 * ICT1002 (C Language) Group Project, AY19 Trimester 1.
 *
 * This file implements the chatbot's knowledge base.
 *
 * knowledge_get() retrieves the response to a question.
 * knowledge_put() inserts a new response to a question.
 * knowledge_read() reads the knowledge base from a file.
 * knowledge_reset() erases all of the knowledge.
 * kowledge_write() saves the knowledge base in a file.
 *
 * You may add helper functions as necessary.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "chat1002.h"

typedef struct header{
	char* intent;
	struct row* content;
	struct header* next;
}header;

typedef struct row{
	char* question;
	char* answer;
	struct row* next;
}row;

header *k_arr = NULL;

/*
 * Get the response to a question.
 *
 * Input:
 *   intent   - the question word
 *   entity   - the entity
 *   response - a buffer to receive the response
 *   n        - the maximum number of characters to write to the response buffer
 *
 * Returns:
 *   KB_OK, if a response was found for the intent and entity (the response is copied to the response buffer)
 *   KB_NOTFOUND, if no response could be found
 *   KB_INVALID, if 'intent' is not a recognised question word
 */
int knowledge_get(const char *intent, const char *entity, char *response, int n) {
	
	header *cursor = k_arr;
	while(cursor != NULL){
		if(compare_token(cursor->intent, intent) == 0){
			row *incursor = cursor->content;
			while(incursor != NULL){
				if(compare_token(incursor->question, entity) == 0){
					snprintf(response, n, incursor->answer);
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
 * Insert a new response to a question. If a response already exists for the
 * given intent and entity, it will be overwritten. Otherwise, it will be added
 * to the knowledge base.
 *
 * Input:
 *   intent    - the question word
 *   entity    - the entity
 *   response  - the response for this question and entity
 *
 * Returns:
 *   KB_FOUND, if successful
 *   KB_NOMEM, if there was a memory allocation failure
 *   KB_INVALID, if the intent is not a valid question word
 */
int knowledge_put(const char *intent, const char *entity, const char *response) {
	
	header *cursor = k_arr;
	char qn[strlen(entity)];
	char ans[strlen(response)];
	memset(qn, '\0', strlen(entity));
	memset(ans, '\0', strlen(response));
	for (int i = 0; i < strlen(entity); ++i)
	{
		qn[i] = tolower(entity[i]);
	}
	for (int i = 0; i < strlen(response); ++i)
	{
		if(response[i] != '\n')
			ans[i] = response[i];
	}
	qn[strlen(entity)] = '\0';
	ans[strlen(ans)] = '\0';

	if(cursor == NULL){
		header* newh = (header*)malloc(sizeof(struct header));
		row* newr = (row*)malloc(sizeof(struct row));

		char n_intent[strlen(intent)];
		for (int i = 0; i < strlen(intent); ++i)
		{
			n_intent[i] = tolower(intent[i]);
		}

		newh->intent = strdup(n_intent);
		newh->next = NULL;

		newr->question = strdup(qn);
		newr->answer = strdup(ans);
		newr->next = NULL;

		newh->content = newr;
		k_arr = newh;

		return KB_OK;
	}

	while(cursor != NULL){
		if(compare_token(cursor->intent, intent) == 0){
			row* new = (row*)malloc(sizeof(struct row));
			new->question = strdup(qn);
			new->answer = strdup(response);
			new->next = cursor->content;
			cursor->content = new;
			return KB_OK;
		}
		cursor = cursor->next;
	}
	
	return KB_INVALID;
}


/*
 * Read a knowledge base from a file.
 *
 * Input:
 *   f - the file
 *
 * Returns: the number of entity/response pairs successful read from the file
 */
int knowledge_read(FILE *f) {
	knowledge_reset();
	header* cursor = k_arr;
	while(!feof(f)){
		char temp[MAX_INPUT];
		fgets(temp, MAX_INPUT, f);
		if(strlen(temp) == 1){
			continue;
		}
		if (strchr(temp, '[')!=NULL && strchr(temp, ']')!=NULL){
			int len=strlen(temp);
			for(int i=1;i<len-1;i++){
				temp[i-1]=temp[i];
			}
			temp[len-3]='\0';
			//printf("%s\n", temp);
			header* new = (header*)calloc(1,sizeof(struct header));
			if(k_arr == NULL){
				new->intent = strdup(temp);
				new->content = NULL;
				new->next = NULL;
				k_arr = new;
				cursor = new;
			}else{
				new->intent = strdup(temp);
				new->content = NULL;
				new->next = k_arr;
				k_arr = new;
				cursor = new;
			}
		}else{
			int alen = strlen(strchr(temp, '='))-1, qlen = strlen(temp)-alen+1;
			char qn[qlen];
			char ans[alen];
			memset(qn, '\0', qlen+1);
			memset(ans, '\0', alen+1);

			int check = 1, q=0, a=0;
			for (int i = 0; i < strlen(temp); ++i){
				if(temp[i] == '='){
					check = 0;
					continue;
				}
				if (check){
					qn[q++] = tolower(temp[i]);
				}else{
					if(temp[i] != '\n')
						ans[a++] = temp[i];
				}
			}
			row* new = (row*)calloc(1, sizeof(struct row));
			new->question = strdup(qn);
			new->answer = strdup(ans);
			if(cursor->content == NULL){
				new->next = NULL;
			}else{
				new->next = cursor->content;
			}
			//printf("%s=%s\n", qn, ans);
			cursor->content = new;
		}
	}
	return 0;
}


/*
 * Reset the knowledge base, removing all know entitities from all intents.
 */
void knowledge_reset() {
	header *temp = NULL;
	row *r_temp1 = NULL, *r_temp2 = NULL;
	while(k_arr!=NULL)	{
		temp = k_arr;
		r_temp1 = temp->content;
		while(r_temp1!=NULL){
			r_temp2 = r_temp1;
			r_temp1 = r_temp1->next;
			free(r_temp2->question);
			free(r_temp2->answer);
			free(r_temp2);
		}
		k_arr = k_arr->next;
		free(temp->intent);
		free(temp);
	}
}


/*
 * Write the knowledge base to a file.
 *
 * Input:
 *   f - the file
 */
void knowledge_write(FILE *f) {
	header *cursor = k_arr;
	row *row_cursor = NULL;

	while(cursor != NULL){

		fwrite("[", 1, 1, f);
		fwrite(cursor->intent, strlen(cursor->intent), 1, f);
		fwrite("]\n", 2, 1, f);

		row_cursor = cursor->content;
		while(row_cursor != NULL){
			fwrite(row_cursor->question, strlen(row_cursor->question), 1, f);
			fwrite("=", 1, 1, f);
			fwrite(row_cursor->answer, strlen(row_cursor->answer), 1, f);
			if(row_cursor->next != NULL)
				fwrite("\n", 1, 1, f);
			row_cursor = row_cursor->next;
		}
		if(cursor->next != NULL)
			fwrite("\n\n", 2, 1, f);

		cursor = cursor->next;
	}

}
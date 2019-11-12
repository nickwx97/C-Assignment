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
	
	printf("Testing\n");
	
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
	
	/* to be implemented */
	
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
	
	header* cursor = k_arr;
	while(!feof(f)){
		char temp[MAX_INPUT];
		fgets(temp, MAX_INPUT, f);
		if(strlen(temp) == 1){
			continue;
		}
		if (strchr(temp, '[')!=NULL && strchr(temp, ']')!=NULL){
			header* new = (header*)malloc(sizeof(struct header));
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
			int alen = strlen(strchr(temp, '=')), qlen = strlen(temp)-alen;
			char qn[qlen];
			char ans[alen];
			memset(qn, '\0', qlen);
			memset(ans, '\0', alen);
			int check = 1, q=0, a=0;
			for (int i = 0; i < strlen(temp); ++i){
				if(temp[i] == '='){
					check = 0;
					continue;
				}
				if (check){
					qn[q++] = tolower(temp[i]);
				}else{
					ans[a++] = temp[i];
				}
			}
			row* new = (row*)malloc(sizeof(struct row));
			new->question = strdup(qn);
			new->answer = strdup(ans);
			if(cursor->content == NULL){
				new->next = NULL;
			}else{
				new->next = cursor->content;
			}
			cursor->content = new;
		}
	}
	return 0;
}


/*
 * Reset the knowledge base, removing all know entitities from all intents.
 */
void knowledge_reset() {
	
	/* to be implemented */
	
}


/*
 * Write the knowledge base to a file.
 *
 * Input:
 *   f - the file
 */
void knowledge_write(FILE *f) {
	
	/* to be implemented */
	
}
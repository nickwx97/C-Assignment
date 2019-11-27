/*
 * ICT1002 (C Language) Group Project, AY19 Trimester 1.
 *
 * This file contains the definitions and function prototypes for all of
 * features of the ICT1002 chatbot.
 */

#ifndef _CHAT1002_H
#define _CHAT1002_H

#include <stdio.h>

/* the maximum number of characters we expect in a line of input (including the terminating null)  */
#define MAX_INPUT    256

/* the maximum number of characters for the username (including the terminating null)  */
#define MAX_USERNAME 30

/* the maximum number of characters allowed in the name of an intent (including the terminating null)  */
#define MAX_INTENT   32

/* the maximum number of characters allowed in the name of an entity (including the terminating null)  */
#define MAX_ENTITY   64

/* the maximum number of characters allowed in a response (including the terminating null) */
#define MAX_RESPONSE 256

/* return codes for knowledge_get() and knowledge_put() */
#define KB_OK        0
#define KB_NOTFOUND -1
#define KB_INVALID  -2
#define KB_NOMEM    -3

/* functions defined in main.c */
int compare_token(const char *token1, const char *token2);
void prompt_user(char *buf, int n, const char *format, ...);
char userName[MAX_USERNAME];/* to store the username typed in by the user*/

/* functions defined in chatbot.c */
void request_username();
const char *chatbot_botname();
const char *chatbot_username();
int chatbot_is_swear(int inc, char* inv[], char* response, int n);
int chatbot_main(int inc, char *inv[], char *response, int n);
int chatbot_is_exit(const char *intent);
int chatbot_do_exit(int inc, char *inv[], char *response, int n);
int chatbot_is_load(const char *intent);
int chatbot_do_load(int inc, char *inv[], char *response, int n);
int chatbot_is_question(const char *intent);
int chatbot_do_question(int inc, char *inv[], char *response, int n);
int chatbot_is_delete(const char *intent);
int chatbot_do_delete(int inc, char *inv[], char *response, int n);
int chatbot_is_reset(const char *intent);
int chatbot_do_reset(int inc, char *inv[], char *response, int n);
int chatbot_is_save(const char *intent);
int chatbot_do_save(int inc, char *inv[], char *response, int n);
int chatbot_is_smalltalk(const char *intent);
int chatbot_do_smalltalk(int inc, char *inv[], char *resonse, int n);
int chatbot_do_help(int inc, char* inv[], char* response, int n);
int chatbot_is_help(const char* intent);
int chatbot_do_identity(int inc, char* inv[], char* response, int n);
int chatbot_is_identity(int inc, char* inv[]);
int chatbot_is_update(const char* intent);
int is_quantifier(const char* intent);
int chatbot_do_update(int inc, char* inv[], char* response, int n);

/* functions defined in knowledge.c */
int knowledge_delete(const char *intent, const char *entity);
int knowledge_get(const char *intent, const char *entity, char *response, int n);
int knowledge_put(const char *intent, const char *entity, const char *response);
void knowledge_reset();
int knowledge_read(FILE *f);
int knowledge_write(FILE *f);
#endif
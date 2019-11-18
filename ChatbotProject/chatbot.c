/*
 * ICT1002 (C Language) Group Project, AY19 Trimester 1.
 *
 * This file implements the behaviour of the chatbot. The main entry point to
 * this module is the chatbot_main() function, which identifies the intent
 * using the chatbot_is_*() functions then invokes the matching chatbot_do_*()
 * function to carry out the intent.
 *
 * chatbot_main() and chatbot_do_*() have the same method signature, which
 * works as described here.
 *
 * Input parameters:
 *   inc      - the number of words in the question
 *   inv      - an array of pointers to each word in the question
 *   response - a buffer to receive the response
 *   n        - the size of the response buffer
 *
 * The first word indicates the intent. If the intent is not recognised, the
 * chatbot should respond with "I do not understand [intent]." or similar, and
 * ignore the rest of the input.
 *
 * If the second word may be a part of speech that makes sense for the intent.
 *    - for WHAT, WHERE and WHO, it may be "is" or "are".
 *    - for SAVE, it may be "as" or "to".
 *    - for LOAD, it may be "from".
 * The word is otherwise ignored and may be omitted.
 *
 * The remainder of the input (including the second word, if it is not one of the
 * above) is the entity.
 *
 * The chatbot's answer should be stored in the output buffer, and be no longer
 * than n characters long (you can use snprintf() to do this). The contents of
 * this buffer will be printed by the main loop.
 *
 * The behaviour of the other functions is described individually in a comment
 * immediately before the function declaration.
 *
 * You can rename the chatbot and the user by changing chatbot_botname() and
 * chatbot_username(), respectively. The main loop will print the strings
 * returned by these functions at the start of each line.
 */
 
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "chat1002.h"
 
 
/*
 * Get the name of the chatbot.
 *
 * Returns: the name of the chatbot as a null-terminated string
 */
const char *chatbot_botname() {

	return "Miss Magnolia";
	
}


/*
 * Get the name of the user.
 *
 * Returns: the name of the user as a null-terminated string
 */
const char *chatbot_username() {
	
	return userName;
	
}


/*
 * Get a response to user input.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0, if the chatbot should continue chatting
 *   1, if the chatbot should stop (i.e. it detected the EXIT intent)
 */
int chatbot_main(int inc, char *inv[], char *response, int n) {
	
	/* check for empty input */
	if (inc < 1) {
		snprintf(response, n, "");
		return 0;
	}

	/* look for an intent and invoke the corresponding do_* function */
	if (chatbot_is_exit(inv[0]))
		return chatbot_do_exit(inc, inv, response, n);
	else if (chatbot_is_smalltalk(inv[0]))
		return chatbot_do_smalltalk(inc, inv, response, n);
	else if (chatbot_is_load(inv[0]))
		return chatbot_do_load(inc, inv, response, n);
	else if (chatbot_is_question(inv[0]))
		return chatbot_do_question(inc, inv, response, n);
<<<<<<< HEAD
	else if (chatbot_is_game(inv[0]))
		return chatbot_do_game(inc, inv, response, n);
=======
	else if (chatbot_is_help(inv[0]))
		return chatbot_do_help(inc, inv, response, n);
>>>>>>> cc679e93390cc60bdd982691502f16bd643a660c
	else if (chatbot_is_reset(inv[0]))
		return chatbot_do_reset(inc, inv, response, n);
	else if (chatbot_is_save(inv[0]))
		return chatbot_do_save(inc, inv, response, n);
	else {
		snprintf(response, n, "I don't understand \"%s\".", inv[0]);
		return 0;
	}

}


/*
 * Determine whether an intent is EXIT.
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is "exit" or "quit"
 *  0, otherwise
 */
int chatbot_is_exit(const char *intent) {
	
	return compare_token(intent, "exit") == 0 || compare_token(intent, "quit") == 0;
	
}


/*
 * Perform the EXIT intent.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0 (the chatbot always continues chatting after a question)
 */
int chatbot_do_exit(int inc, char *inv[], char *response, int n) {
	 
	snprintf(response, n, "Goodbye!");
	
	return 1;
	 
}


/*
 * Determine whether an intent is HELP.
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is "help"
 *  0, otherwise
 */
int chatbot_is_help(const char* intent) {

	return compare_token(intent, "help") == 0;

}

/*
 * Perform the HELP intent.
 *
 * Returns:
 *   0 (the chatbot always continues chatting after a question)
 */
int chatbot_do_help(int inc, char* inv[], char* response, int n) {
	snprintf(response, n, "\n<------List of commands:------> \n load 'sample.ini' - load the knowledge base\n save - save knowledge base\n reset - reset knowledge base.\n quit/exit - exit the chatbot.\n");
	
	return 0;
}


/*
 * Determine whether an intent is LOAD.
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is "load"
 *  0, otherwise
 */
int chatbot_is_load(const char *intent) {
	
	return compare_token(intent, "load") == 0;
	
}


/*
 * Load a chatbot's knowledge base from a file.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0 (the chatbot always continues chatting after loading knowledge)
 */
int chatbot_do_load(int inc, char *inv[], char *response, int n) {

	int len = 0;
	FILE *f;
	if (inc > 2){
		for (int i = 1; i < inc; ++i){
			len += strlen(inv[i]);
		}
		char fp[len];
		memset(fp, '\0', len);
		strcat(fp, inv[1]);
		for (int i = 2; i < inc; ++i){
			strcat(fp, " ");
			strcat(fp, inv[i]);
		}
		f = fopen(fp, "r");
		if(f == NULL){
			snprintf(response, n, "Unable to load file: \"%s\".", fp);
			return 0;
		}
	}else{
		f = fopen(inv[1], "r");
		if(f == NULL){
			snprintf(response, n, "Unable to load file: \"%s\".", inv[1]);
			return 0;
		}
	}
	knowledge_read(f);
	fclose(f);
	snprintf(response, n, "\"%s\" loaded.", inv[1]);
	return 0;
	 
}


/*
 * Determine whether an intent is a question.
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is "what", "where", or "who"
 *  0, otherwise
 */
int chatbot_is_question(const char *intent) {
	return compare_token(intent, "who") == 0 || compare_token(intent, "what") == 0 || compare_token(intent, "where") == 0;
}


/*
 * Answer a question.
 *
 * inv[0] contains the the question word.
 * inv[1] may contain "is" or "are"; if so, it is skipped.
 * The remainder of the words form the entity.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0 (the chatbot always continues chatting after a question)
 */
int chatbot_do_question(int inc, char *inv[], char *response, int n) {
	int len = 0;
	int num = 1;
	if(compare_token(inv[1], "is") == 0 || compare_token(inv[1], "are") == 0){
		num = 2;
	}

	for (int i = num; i < inc; ++i){
		len+=strlen(inv[i]);
	}
	char entity[len];
	memset(entity, '\0', len);
	strcat(entity, inv[num]);
	for (int i = num+1; i < inc; ++i){
		strcat(entity, " ");
		strcat(entity, inv[i]);
	}
	if(knowledge_get(inv[0], entity, response, n) == KB_NOTFOUND){
		if (num == 1) printf("%s: Sorry, I do not know. %s %s?\n", chatbot_botname(), inv[0], entity);
		else printf("%s: Sorry, I do not know. %s %s %s?\n", chatbot_botname(), inv[0], inv[1], entity);
		printf("%s: ", chatbot_username());
		char input[MAX_INPUT];
		fgets(input, MAX_INPUT, stdin);



		if(strlen(input) == 1){
			snprintf(response, n , ":-(");
		}else{
			knowledge_put(inv[0], entity, input);
			snprintf(response, n , "Thank you.");
		}
	}
	 
	return 0;
	 
}
int chatbot_is_game(const char* intent) {

	return compare_token(intent, "play") == 0;
	

}

int chatbot_do_game(int inc, char* inv[], char* response, int n) {
	//snprintf(response, n, "test");
	char* guessword;
	char* display;
	char* output;
	int roll,p;
	roll = (rand() % 3);
		
	switch (roll) {
	case 1: 
		guessword = "Computer";
		break;
	case 2: 
		guessword = "Architecture";
		break;
	default: 
		guessword = "None";
	}
	strtok(display, "\n");
	
	const int noofcharacters = strlen(guessword);
	
	for (int i = 0; i < noofcharacters; i++)
	{
		display[i] = '_';
		
	}

		
//	do
//	{
//		p = 0;
//
//		printf("Player 2 has so far guessed: %s\n", display);
//		printf("Player 2, you have %d guesses remaining. Enter your your next guess:\n", guesses);//getting input from users
//		fgets(guess_input, MAX_INPUT_CHAR, stdin);
//
//		
//		strtok(guess_input, "\n");
//		
//		for (int i = 0; i < (strlen(guess_input)); i++)
//		{
//			guess_input[i] = tolower(guess_input[i]);
//
//
//			if (checkpunct(guess_input[i]) == 1)
//			{
//				p++;
//			}
//		}
//
//		
//		if (p == 0)
//		{
//			for (int i = 0; i < strlen(user_input); i++)
//			{
//				if (user_input[i] == guess_input[0])
//				{
//					display[i] = guess_input[0];
//				}
//			}
//
//			guesses--;
//		}
//
//		if (strcmp(user_input, display) == 0)
//		{
//			break;
//		}
//	} while (guesses > 0);
//
//	if (strcmp(user_input, display) == 0)
//	{
//		printf("Player 2 wins.\n");
//	}
//	else
//	{
//		printf("Player 1 wins.\n");
//	}
//}
	output = ("Player 2 has so far guessed : \n", display);
	snprintf(response, n, output);
	return 0;
}

/*
 * Determine whether an intent is RESET.
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is "reset"
 *  0, otherwise
 */
int chatbot_is_reset(const char *intent) {
	
	/* to be implemented */
	
	return 0;
	
}



/*
 * Reset the chatbot.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0 (the chatbot always continues chatting after beign reset)
 */
int chatbot_do_reset(int inc, char *inv[], char *response, int n) {
	
	/* to be implemented */
	 
	return 0;
	 
}


/*
 * Determine whether an intent is SAVE.
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is "save"
 *  0, otherwise
 */
int chatbot_is_save(const char *intent) {
	
	return compare_token(intent, "save") == 0;
	
}


/*
 * Save the chatbot's knowledge to a file.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0 (the chatbot always continues chatting after saving knowledge)
 */
int chatbot_do_save(int inc, char *inv[], char *response, int n) {
	
	/* to be implemented */
	
	return 0;
	 
}
 
 
/*
 * Determine which an intent is smalltalk.
 *
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is the first word of one of the smalltalk phrases
 *  0, otherwise
 */
int chatbot_is_smalltalk(const char *intent) {
	if (compare_token(intent, "good") == 0 || compare_token(intent, "hello") == 0 || compare_token(intent, "goodbye") == 0 || compare_token(intent, "it's") == 0
		|| compare_token(intent, "today") == 0 || compare_token(intent, "i") == 0) {
		return 1;
	}
	else {
		return 0;
	}
}


/*
 * Respond to smalltalk.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0, if the chatbot should continue chatting
 *   1, if the chatbot should stop chatting (e.g. the smalltalk was "goodbye" etc.)
 */
int chatbot_do_smalltalk(int inc, char *inv[], char *response, int n) {
	for (int i = 0; i < inc; i++) {
		//printf("%s\n", inv[i]);
		if (compare_token(inv[i], "morning") == 0) {
			snprintf(response, n, "%s", "Good morning.");
		}
		else if (compare_token(inv[i], "afternoon") == 0) {
			snprintf(response, n, "%s", "Good afternoon.");
		}
		else if (compare_token(inv[i], "evening") == 0) {
			snprintf(response, n, "%s", "Good evening.");
		}
		else if (compare_token(inv[i], "goodbye") == 0) {
			//chatbot_do_exit(inc, inv, response, n); //this function returns 1 and exits the program
			return 1;
		}
		else if (compare_token(inv[i], "hello") == 0) {
			snprintf(response, n, "%s", "Hello.");
		}
		else if (compare_token(inv[i], "it's") == 0) {
			snprintf(response, n, "%s", "Indeed it is.");
		}
		else if (compare_token(inv[i], "today") == 0) {
			snprintf(response, n, "%s", "That's good to know.");
		}
		else if (compare_token(inv[i], "i") == 0) {
			snprintf(response, n, "%s", "I see.");
		}
	}

	
	return 0;
	
}
  
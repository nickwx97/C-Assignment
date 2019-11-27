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

#include <stdlib.h>

#include "chat1002.h"

int swearcount = 0;
/*
 * Get the name of the chatbot.
 *
 * Returns: the name of the chatbot as a null-terminated string
 */

void request_username(char* response, int n) {
	/* Request for a username */
	int same_name_check = 0;/* EASTER EGG to check if user inputs the same name as the chatbot*/
	int pest_check = 0;/*EASTER EGG to check if user types same name with chatbot 3 times in a row*/
	printf("%s: Ey, who are you ah?\n", chatbot_botname());
	do {
		fgets(userName, MAX_USERNAME, stdin);
		userName[strlen(userName) - 1] = '\0';
	} while (strlen(userName) <= 1);

	if (compare_token(chatbot_botname(), userName) == 0) {
		same_name_check = 1;
	}

	/*Loops to make sure the user chooses a different name as the chatbot*/
	while (same_name_check == 1) {
		printf("%s: Wa eh dun same name as me leh? Choose something else la.\n", chatbot_botname());
		fgets(userName, MAX_USERNAME, stdin);
		userName[strlen(userName) - 1] = '\0';

		if (compare_token(chatbot_botname(), userName) != 0) {
			same_name_check = 0;
			break;
		}

		if (pest_check++ == 2) {
			snprintf(response, n, "Wa you want try be funny is it, irritating fool. I call you Irritating Fool sua. What you wan ask me la?\n");
			strcpy(userName, "Irritating Fool");
			return;
		}
	}


	/* print a welcome message */
	const char* profname[] = {"Steven Wong","Frank Guan","Scott Jones"};
	const char* profresponse[] = {"Oh, you teach ICT1003 one right? That 'Fetch,Decode,Execute' guy? Heard you damn good sia. Haha, anyways, what's your question?\n",
							"Oh, wow. Please give my creator an A+ for his 1002 please. He finds that you are the best SIT professor. May I know what you would like to ask me, sir?\n" ,
							"Master Jones, Welcome sir. Nice to see you again. How may I help you today?\n"};
	const char* proftitle[] = {"Master","Lord","Emperor","Almighty"};

	int arraysize = sizeof(profname) / sizeof(char *);

	/*Checks if the user is an ICT professor. If so, chatbot attempts to por for grades*/
	for (int i = 0; i < arraysize;i++) {
		if (compare_token(userName, profname[i]) == 0) {
			const char* temp_title = proftitle[rand() % arraysize];
			snprintf(userName, strlen(userName) + strlen(temp_title) + 3, "%s %s", temp_title, profname[i]);
			snprintf(response, n, profresponse[i]);
			return;
		}
	}
	snprintf(response, n, "Hello %s, I'm %s. \nEnter help for list of commands.\n", userName, chatbot_botname());
}

const char * chatbot_botname() {
    return "Miss Magnolia";
}

/*
 * Get the name of the user.
 *
 * Returns: the name of the user as a null-terminated string
 */
const char * chatbot_username() {
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
int chatbot_main(int inc, char* inv[], char* response, int n) {

	/* check for empty input */
	if (inc < 1) {
		snprintf(response, n, " ");
		return 0;
	}


	/* look for an intent and invoke the corresponding do_* function */

		if (chatbot_is_swear(inc, inv, response, n))
			if (swearcount == 3){
				snprintf(response, n, "I do not want to talk to you anymore, get lost!");
				return 1;
			}
			else {
				return 0;
			}
		else if (chatbot_is_exit(inv[0]))
			return chatbot_do_exit(inc, inv, response, n);
		else if (chatbot_is_identity(inc, inv))
			return chatbot_do_identity(inc, inv, response, n);
		else if (chatbot_is_smalltalk(inv[0]))
			return chatbot_do_smalltalk(inc, inv, response, n);
		else if (chatbot_is_load(inv[0]))
			return chatbot_do_load(inc, inv, response, n);
		else if (chatbot_is_delete(inv[0]))
			return chatbot_do_delete(inc, inv, response, n);
		else if (chatbot_is_question(inv[0]))
			return chatbot_do_question(inc, inv, response, n);
		else if (chatbot_is_update(inv[0]))
			return chatbot_do_update(inc, inv, response, n);
		else if (chatbot_is_help(inv[0]))
			return chatbot_do_help(inc, inv, response, n);
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
 * Determine whether the user's response contains a swear word.
 *
 *
 * Returns:
 *  1, if the response contains any swear word
 *  0, otherwise
 */

int chatbot_is_swear(int inc, char* inv[], char* response, int n) {
	
	const char* swear[] = { "fuck", "fucker", "asshole", "idiot", "bastard", "dumbass", "hell", "faggot", "bitch" };
	const char* reply[] = { "Hey, you have quite a potty mouth.", "Watch your language!",
					  "Hey, I may be a robot but I have feelings too!", "Hey that hurts me inside!",
					  "Do you kiss your mother with that mouth?" };

	int arraysize = sizeof(reply) / sizeof(char *);

	for (int j = 0; j < inc; j++) {
		for (int i = 0; i < sizeof(swear) / sizeof(char *); i++) {
			if (compare_token(inv[j], swear[i]) == 0) {
				snprintf(response, n, "%s", reply[rand() % arraysize]);
				swearcount++;
				return 1;
			}
			
		}
	}
	return 0;
}

int chatbot_is_exit(const char * intent) {
    return compare_token(intent, "exit") == 0 || compare_token(intent, "quit") == 0;
}

/*
 * Perform the EXIT intent.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   1 (the chatbot quits after this)
 */
int chatbot_do_exit(int inc, char * inv[], char * response, int n) {
	char* greetings[] = {"Farewell!","Hope to talk you soon!","I will miss you!","Goodbye!"};
	int arraysize = sizeof(greetings) / sizeof(char *);
    snprintf(response, n, greetings[rand() % arraysize]);
    return 1;
}



/*
 * Determine whether an intent is FORGET.
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is "forget"
 *  0, otherwise
 */
int chatbot_is_delete(const char * intent) {
    return compare_token(intent, "forget") == 0;
}

/*
 * Perform the FORGET intent.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0 (the chatbot always continues chatting after a question)
 */
int chatbot_do_delete(int inc, char * inv[], char * response, int n) {

    if (inc == 1) {
        snprintf(response, n, "My memory is really good... I can't forget nothing :-)");
    } else if (inc == 2) {
        snprintf(response, n, "Ah-ha! You can't trick me into forgetting English words!");
    } else if (chatbot_is_question(inv[1])) {
        int len = 0;
        int num = 2;
        if (is_quantifier(inv[2]) && inc == 3) {
            snprintf(response, n, "Ah-ha! You can't trick me into forgetting English words!");
            return 0;
        }else if (is_quantifier(inv[2])){
        	num = 3;
        }
        for (int i = num; i < inc; ++i) {
            len += strlen(inv[i]);
        }
        char entity[len];
        memset(entity, '\0', len);
        strcat(entity, inv[num]);
        for (int i = num + 1; i < inc; ++i) {
            strcat(entity, " ");
            strcat(entity, inv[i]);
        }
        int ret = knowledge_get(inv[1], entity, NULL, 0);
        if (ret == KB_OK) {
            knowledge_delete(inv[1], entity);
            snprintf(response, n, "Forgotten!");
        } else if (ret == KB_NOTFOUND) {
            snprintf(response, n, "I cannot forget something that I do not already know :-(");
        }
    }
    return 0;
}

/*
 * Determine whether an intent is UPDATE.
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is "update"
 *  0, otherwise
 */
int chatbot_is_update(const char * intent) {
    return compare_token(intent, "update") == 0;
}

/*
 * Determine whether an intent is QUANTIFIER.
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is "is" or "are"
 *  0, otherwise
 */
int is_quantifier(const char * intent) {
    return compare_token(intent, "is") == 0 || compare_token(intent, "are") == 0;
}

/*
 * Perform the UPDATE intent.
 *
 * Returns:
 *   0 (the chatbot always continues chatting after a question)
 */
int chatbot_do_update(int inc, char * inv[], char * response, int n) {

    if (inc == 1) {
        snprintf(response, n, "So... Are you going to tell me the question you want to update?");
    } else if (inc == 2) {
        snprintf(response, n, "What? I cannot update the definition of a word...");
    } else if (chatbot_is_question(inv[1])) {
        int len = 0;
        int num = 2;
        if (is_quantifier(inv[2]) && inc == 3) {
            snprintf(response, n, "What? I cannot update the definition of a word...");
            return 0;
        }else if (is_quantifier(inv[2])){
        	num = 3;
        }        for (int i = num; i < inc; ++i) {
            len += strlen(inv[i]);
        }
        char entity[len];
        memset(entity, '\0', len);
        strcat(entity, inv[num]);
        for (int i = num + 1; i < inc; ++i) {
            strcat(entity, " ");
            strcat(entity, inv[i]);
        }
        char initial_ans[n];
        memset(initial_ans, '\0', n);
        int ret = knowledge_get(inv[1], entity, initial_ans, n);
        if (ret == KB_OK) {
            if (num == 2)
                printf("%s: I know \"%s %s\" is \"%s\". Tell me otherwise (Leave blank to cancel update).\n%s: ",
                    chatbot_botname(), inv[1], entity, initial_ans, chatbot_username());
            else
                printf("%s: I know \"%s %s %s\" is \"%s\". Tell me otherwise (Leave blank to cancel update).\n%s: ",
                    chatbot_botname(), inv[1], inv[2], entity, initial_ans, chatbot_username());
            char input[MAX_INPUT];
            fgets(input, MAX_INPUT, stdin);
            if (strlen(input) == 1) {
                snprintf(response, n, "Update cancelled.");
            } else {
                ret = knowledge_delete(inv[1], entity);
                ret += knowledge_put(inv[1], entity, input);
                if (ret == 0) {
                    snprintf(response, n, "Updated.");
                }
            }
        } else if (ret == KB_NOTFOUND) {
        	printf("%s: I don't know the answer to that... Tell me?\n%s: ", chatbot_botname(), chatbot_username());
            char input[MAX_INPUT];
            fgets(input, MAX_INPUT, stdin);
            if (strlen(input) == 1) {
                snprintf(response, n, ":-(");
            } else {
                if (knowledge_put(inv[1], entity, input) == KB_OK) {
                    snprintf(response, n, "Thanks! :-)");
                }else{
                	snprintf(response, n, "Something went wrong :-/");
                }
            }
        }
    }
    return 0;
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
int chatbot_is_help(const char * intent) {
    return compare_token(intent, "help") == 0;
}

/*
 * Perform the HELP intent.
 *
 * Returns:
 *   0 (the chatbot always continues chatting after a question)
 */
int chatbot_do_help(int inc, char * inv[], char * response, int n) {
    snprintf(response, n, "\n<------List of commands:------> \nload 'sample.ini' - load the knowledge base\nsave - save knowledge base\nreset - reset knowledge base.\nquit/exit - exit the chatbot.\n");
    return 0;
}

/*
 * Determine whether an intent is IDENTITY.
 *
 * Returns:
 *  1, if the intent is IDENTITY
 *  0, otherwise
 */
int chatbot_is_identity(int inc, char * inv[]) {
    if (inc == 2 || inc == 3) {
        if (compare_token(inv[0], "who") == 0) {
            if (compare_token(inv[1], "are") == 0 || compare_token(inv[1], "is") == 0) {
                if (compare_token(inv[2], "you") == 0) {
                    return 1;
                }
            } else if (compare_token(inv[1], "you") == 0) {
                return 1;
            } else if (compare_token(inv[1], "am") == 0) {
                if (compare_token(inv[2], "I") == 0) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

int chatbot_do_identity(int inc, char * inv[], char * response, int n) {

    if (inc == 2 || inc == 3) {
        if (compare_token(inv[0], "who") == 0) {
            if (compare_token(inv[1], "are") == 0 || compare_token(inv[1], "is") == 0) {
                if (compare_token(inv[2], "you") == 0) {
                    snprintf(response, n, "Hmm... I wonder who am I...?");
                }
            } else if (compare_token(inv[1], "you") == 0) {
                snprintf(response, n, "Hmm... I wonder who am I...?");
            } else if (compare_token(inv[1], "am") == 0) {
                if (compare_token(inv[2], "I") == 0) {
                    snprintf(response, n, "Didn't you say your name is %s?", chatbot_username());
                }
            }
        }
    }
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
int chatbot_is_load(const char * intent) {
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
int chatbot_do_load(int inc, char * inv[], char * response, int n) {

    int len = 0;
    FILE * f;
    if (inc > 2) {
        for (int i = 1; i < inc; ++i) {
            len += strlen(inv[i]);
        }
        char fp[len];
        memset(fp, '\0', len);
        strcat(fp, inv[1]);
        for (int i = 2; i < inc; ++i) {
            strcat(fp, " ");
            strcat(fp, inv[i]);
        }
        f = fopen(fp, "r");
        if (f == NULL) {
            snprintf(response, n, "Unable to load file: \"%s\".", fp);
            return 0;
        }
    } else {
        f = fopen(inv[1], "r");
        if (f == NULL) {
            snprintf(response, n, "Unable to load file: \"%s\".", inv[1]);
            return 0;
        }
    }
    int num_of_lines = knowledge_read(f);
    fclose(f);
    snprintf(response, n, "%d lines from \"%s\" loaded.", num_of_lines, inv[1]);
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
int chatbot_is_question(const char * intent) {
    return compare_token(intent, "who") == 0 || compare_token(intent, "what") == 0 || compare_token(intent, "where") == 0 ||
        compare_token(intent, "when") == 0 || compare_token(intent, "why") == 0 || compare_token(intent, "how") == 0;
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
int chatbot_do_question(int inc, char * inv[], char * response, int n) {

    if (inc == 1) {
        snprintf(response, n, "Sorry I do not understand %s.", inv[0]);
        return 0;
    } else if (inc == 2 && (is_quantifier(inv[1]))) {
        snprintf(response, n, "Sorry I do not understand %s %s.", inv[0], inv[1]);
        return 0;
    }
    int len = 0;
    int num = 1;
    if (is_quantifier(inv[1])) {
        num = 2;
    }
    for (int i = num; i < inc; ++i) {
        len += strlen(inv[i]);
    }
    char entity[len];
    memset(entity, '\0', len);
    strcat(entity, inv[num]);
    for (int i = num + 1; i < inc; ++i) {
        strcat(entity, " ");
        strcat(entity, inv[i]);
    }
    if (knowledge_get(inv[0], entity, response, n) == KB_NOTFOUND) {
        if (num == 1) printf("%s: Sorry, I do not know. %s %s?\n", chatbot_botname(), inv[0], entity);
        else printf("%s: Sorry, I do not know. %s %s %s?\n", chatbot_botname(), inv[0], inv[1], entity);
        printf("%s: ", chatbot_username());
        char input[MAX_INPUT];
        fgets(input, MAX_INPUT, stdin);

        if (strlen(input) == 1) {
            snprintf(response, n, ":-(");
        } else {
            if (knowledge_put(inv[0], entity, input) == KB_OK) {
                snprintf(response, n, "Thank you.");
            } else {
                snprintf(response, n, "An error has occured.");
            }
        }
    }
    return 0;
}

int chatbot_is_reset(const char * intent) {
    return (compare_token(intent, "reset") == 0) ? 1 : 0;
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
int chatbot_do_reset(int inc, char * inv[], char * response, int n) {
    knowledge_reset();
    snprintf(response, n, "%s", "Chatbot Reset");
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
int chatbot_is_save(const char * intent) {
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
int chatbot_do_save(int inc, char * inv[], char * response, int n) {

	int num_of_rows;

    if (inc == 1) {
        snprintf(response, n, "File path not specified for SAVE");
    } else if (inc == 2) {
        printf("%s\n", inv[1]);
        FILE * f = fopen(inv[1], "r");
        if (f != NULL) {
            printf("%s: File already exists, overwrite? (Yes/ No) (Defaults to Yes)\n%s: ", chatbot_botname(), chatbot_username());
            char input[MAX_INPUT];
            fgets(input, MAX_INPUT, stdin);
            input[strlen(input) - 1] = '\0';
            if (strlen(input) == 1 || compare_token(input, "no") == 0) {
                fclose(f);
                snprintf(response, n, "Save not done.");
            } else {
                f = fopen(inv[1], "w");
                if (f == NULL) {
                    snprintf(response, n, "Unable to write to %s", inv[1]);
                    perror("error");
                    return 0;
                }
				num_of_rows = knowledge_write(f);
                fclose(f);
                snprintf(response, n, "%d rows saved to %s",num_of_rows, inv[1]);
            }
        } else {
            fclose(f);
            f = fopen(inv[1], "w");
            if (f == NULL) {
                snprintf(response, n, "Unable to write to %s", inv[1]);
                perror("error");
                return 0;
            }
			num_of_rows = knowledge_write(f);
            fclose(f);
			snprintf(response, n, "%d rows saved to %s", num_of_rows, inv[1]);
        }
        fclose(f);
    } else if (inc == 3 && compare_token(inv[1], "to") == 0) {
        FILE * f = fopen(inv[2], "r");
        if (f != NULL) {
            printf("%s: File already exists, overwrite? (Yes/ No) (Defaults to Yes)", chatbot_botname());
            char input[MAX_INPUT];
            fgets(input, MAX_INPUT, stdin);
            input[strlen(input) - 1] = '\0';
            if (strlen(input) == 1 || compare_token(input, "no") == 0) {
                fclose(f);
                snprintf(response, n, "Save not done.");
            } else {
                fclose(f);
                f = fopen(inv[2], "w");
                if (f == NULL) {
                    snprintf(response, n, "Unable to write to %s", inv[2]);
                    return 0;
                }
				num_of_rows = knowledge_write(f);
                fclose(f);
                snprintf(response, n, "%d rows saved to %s", num_of_rows, inv[2]);
            }
        } else {
            f = fopen(inv[2], "w");
            if (f == NULL) {
                snprintf(response, n, "Unable to write to %s", inv[2]);
                return 0;
            }
            num_of_rows = knowledge_write(f);
            fclose(f);
			snprintf(response, n, "%d rows saved to %s", num_of_rows, inv[2]);
        }
        fclose(f);
    }
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
int chatbot_is_smalltalk(const char * intent) {
    if (compare_token(intent, "good") == 0 || compare_token(intent, "hello") == 0 || compare_token(intent, "goodbye") == 0 || compare_token(intent, "it's") == 0 ||
        compare_token(intent, "today") == 0 || compare_token(intent, "i") == 0 || compare_token(intent, "it") == 0) {
        return 1;
    } else {
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
int chatbot_do_smalltalk(int inc, char * inv[], char * response, int n) {
    for (int i = 0; i < inc; i++) {
        //printf("%s\n", inv[i]);
        if (compare_token(inv[i], "morning") == 0) {
            snprintf(response, n, "%s", "Good morning.");
        } else if (compare_token(inv[i], "afternoon") == 0) {
            snprintf(response, n, "%s", "Good afternoon.");
        } else if (compare_token(inv[i], "evening") == 0) {
            snprintf(response, n, "%s", "Good evening.");
        } else if (compare_token(inv[i], "goodbye") == 0) {
            snprintf(response, n, "%s", "Goodbye.");
            return 1;
        } else if (compare_token(inv[i], "hello") == 0) {
            snprintf(response, n, "%s", "Hello.");
        } else if (compare_token(inv[i], "it's") == 0 ||(compare_token(inv[0], "it") == 0 && compare_token(inv[1], "is") == 0)) {
            snprintf(response, n, "%s", "Indeed it is.");
        } else if (compare_token(inv[i], "today") == 0) {
            snprintf(response, n, "%s", "That's good to know.");
        } else if (compare_token(inv[i], "i") == 0) {
            snprintf(response, n, "%s", "I see.");
        }
    }
    return 0;
}
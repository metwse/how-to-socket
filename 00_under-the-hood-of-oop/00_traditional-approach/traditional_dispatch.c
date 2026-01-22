#include "traditional_dispatch.h"

#include <stdio.h>
#include <string.h>


void handle_message(char *msg)
{
	char first_char = msg[0];
	char *content, *receiver;

	switch (first_char) {
	case '/':
		printf("Command: %s\n", strtok(msg, " "));
		break;
	case '@':
	case '#':
		receiver = strtok_r(&msg[1] /* strip @ or # */, " ", &content);
		printf("Direct message to %s: %s\n",
		       receiver,
		       content);
		break;
	default:
		printf("Global message: %s\n", msg);
	}
}

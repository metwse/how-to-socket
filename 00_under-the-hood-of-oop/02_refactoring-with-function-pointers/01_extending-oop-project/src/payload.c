#include "payload.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>


static char *extract_token(const char *raw) {
	int end;
	char *token;

	for (end = 0; raw[end] != ' ' && raw[end] != '\0'; end++);

	if (end == 0) {
		return NULL;
	} else {
		token = malloc(sizeof(char) * (end + 1));
		assert(token);
		token[end] = '\0';

		for (int i = 0; i < end; i++)
			token[i] = raw[i];

		return token;
	}
}

bool parse_payload(struct payload *p, const char *raw)
{
	// PAIN, but separated
	if (raw[0] == '/') {
		char command_name[7];

		int i;

		for (i = 0; i < 6 && raw[i + 1] != ' '; i++)
			command_name[i] = raw[i + 1];

		command_name[i] = '\0';

		if (strcmp("login", command_name) == 0) {
			char *username, *password;
			assert((username = extract_token(raw + 7)));
			assert((password = extract_token(
				raw + strlen(username) + 8)));

			*p = (struct payload) {
				.process = process_command_login,
				.destroy = destroy_command_login,
				.data.command_login = {
					.username = username,
					.password = password
				}
			};
		} else if (strcmp("join", command_name) == 0) {
			char *channel;
			assert((channel = extract_token(raw + 6)));

			*p = (struct payload) {
				.process = process_command_join,
				.destroy = destroy_command_join,
				.data.command_join = {
					.channel = channel,
				}
			};
		} else if (strcmp("logout", command_name) == 0) {
			*p = (struct payload) {
				.process = process_command_logout,
				.destroy = destroy_command_logout,
			};
		} else {
			printf("Ignoring invalid command %s\n", command_name);
			return false;
		}
	} else {
		// deduce message type by checking first character
		switch (raw[0]) {
		case '@':
			p->process = process_message_direct;
			p->destroy = destroy_message_direct;
			break;
		case '#':
			p->process = process_message_group;
			p->destroy = destroy_message_group;
			break;
		default:
			p->process = process_message_global;
			p->destroy = destroy_message_global;
		}

		char *receiver_name = "";

		// both group and direct messages have a receiver name
		if (p->process != process_message_global)
			assert((receiver_name = extract_token(raw + 1)));

		int content_len = strlen(raw) - strlen(receiver_name);

		char *content = malloc(sizeof(char) * (content_len + 1));
		assert(content);

		if (receiver_name[0] != '\0')
			strcpy(content, raw + strlen(receiver_name) + 2);
		else
			strcpy(content, raw);

		switch (raw[0]) {
		case '@':
			p->data.message_direct.content = content;
			p->data.message_direct.username = receiver_name;
			break;
		case '#':
			p->data.message_group.content = content;
			p->data.message_group.channel = receiver_name;
			break;
		default:
			p->data.message_global.content = content;
		}
	}

	return true;
}

void process_command_login(const struct payload *self)
{
	printf("Command: login\n"
	       "  Arguments: [username: %s, password %s]\n",
	       self->data.command_login.username,
	       self->data.command_login.password);
}
void process_command_join(const struct payload *self)
{
	printf("Command: join\n"
	       "  Arguments: [channel: %s]\n",
	       self->data.command_join.channel);
}
void process_command_logout([[maybe_unused]] const struct payload *self)
{
	printf("Command: logout\n"
	       "  Arguments: []\n");
}
void process_message_direct(const struct payload *self)
{
	printf("Direct message to %s: %s\n",
	       self->data.message_direct.username,
	       self->data.message_direct.content);
}
void process_message_group(const struct payload *self)
{
	printf("Group message to %s: %s\n",
	       self->data.message_group.channel,
	       self->data.message_group.content);
}
void process_message_global(const struct payload *self)
{
	printf("Global message: %s\n",
	       self->data.message_global.content);
}


void destroy_command_login(const struct payload *self)
{
	free(self->data.command_login.username);
	free(self->data.command_login.password);
}
void destroy_command_join(const struct payload *self)
{
	free(self->data.command_join.channel);
}

void destroy_command_logout([[maybe_unused]] const struct payload *self)
{}

void destroy_message_direct(const struct payload *self)
{
	free(self->data.message_direct.username);
	free(self->data.message_direct.content);
}

void destroy_message_group(const struct payload *self)
{
	free(self->data.message_group.channel);
	free(self->data.message_group.content);
}

void destroy_message_global(const struct payload *self)
{
	free(self->data.message_global.content);
}

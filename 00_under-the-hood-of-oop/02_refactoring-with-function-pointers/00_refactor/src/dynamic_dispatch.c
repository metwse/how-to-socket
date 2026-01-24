#include "dynamic_dispatch.h"
#include "payload.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


struct payload_buffer *new_buffer()
{
	struct payload_buffer *buf = malloc(sizeof(struct payload_buffer));
	assert(buf);

	buf->process_base = buf->len = 0;
	buf->cap = 1;
	buf->payloads = malloc(sizeof(struct payload));
	assert(buf->payloads);

	return buf;
}

/* extract string until the next space */
static char *extract_token(const char *payload) {
	int end;
	char *token;

	for (end = 0; payload[end] != ' ' && payload[end] != '\0'; end++);

	if (end == 0) {
		return NULL;
	} else {
		token = malloc(sizeof(char) * (end + 1));
		assert(token);
		token[end] = '\0';

		for (int i = 0; i < end; i++)
			token[i] = payload[i];

		return token;
	}
}

/* gigantic function parses every type of payload
 *
 * NOTE: We made process and cleanup methods dynamic (via function pointers),
 * but we are still constructing payloads using static dispatch
 * (switch/if-else). Setting up function tables dynamically is the next level
 * of abstraction, which we will introduce in upcoming exercises.
 */
void push_payload(struct payload_buffer *buf, const char *payload)
{
	bool payload_valid = true;
	struct payload parsed;

	// still PAIN
	if (payload[0] == '/') {
		char command_name[7];

		int i;

		for (i = 0; i < 6 && payload[i + 1] != ' '; i++)
			command_name[i] = payload[i + 1];

		command_name[i] = '\0';

		if (strcmp("login", command_name) == 0) {
			char *username, *password;
			assert((username = extract_token(payload + 7)));
			assert((password = extract_token(
				payload + strlen(username) + 8)));

			parsed = (struct payload) {
				.process = process_command_login,
				.destroy = destroy_command_login,
				.data.command_login = {
					.username = username,
					.password = password
				}
			};
		} else if (strcmp("join", command_name) == 0) {
			char *channel;
			assert((channel = extract_token(payload + 6)));

			parsed = (struct payload) {
				.process = process_command_join,
				.destroy = destroy_command_join,
				.data.command_join = {
					.channel = channel,
				}
			};
		} else if (strcmp("logout", command_name) == 0) {
			parsed = (struct payload) {
				.process = process_command_logout,
				.destroy = destroy_command_logout,
			};
		} else {
			printf("Ignoring invalid command %s\n", command_name);
			payload_valid = false;
		}
	} else {
		// deduce message type by checking first character
		switch (payload[0]) {
		case '@':
			parsed.process = process_message_direct;
			parsed.destroy = destroy_message_direct;
			break;
		case '#':
			parsed.process = process_message_group;
			parsed.destroy = destroy_message_group;
			break;
		default:
			parsed.process = process_message_global;
			parsed.destroy = destroy_message_global;
		}

		char *receiver_name = "";

		// both group and direct messages have a receiver name
		if (parsed.process != process_message_global)
			assert((receiver_name = extract_token(payload + 1)));

		int content_len = strlen(payload) - strlen(receiver_name);

		char *content = malloc(sizeof(char) * (content_len + 1));
		assert(content);

		if (receiver_name[0] != '\0')
			strcpy(content, payload + strlen(receiver_name) + 2);
		else
			strcpy(content, payload);

		switch (payload[0]) {
		case '@':
			parsed.data.message_direct.content = content;
			parsed.data.message_direct.username = receiver_name;
			break;
		case '#':
			parsed.data.message_group.content = content;
			parsed.data.message_group.channel = receiver_name;
			break;
		default:
			parsed.data.message_global.content = content;
		}
	}

	if (payload_valid) {
		if (buf->cap == buf->len) {
			buf->cap *= 2;
			buf->payloads = realloc(buf->payloads,
			   buf->cap * sizeof(struct payload));

			assert(buf->payloads);
		}

		buf->payloads[buf->len++] = parsed;
	}
}

void process_next(struct payload_buffer *buf)
{
	assert(buf->process_base < buf->len);

	struct payload *payload = &buf->payloads[buf->process_base];
	payload->process(payload);

	buf->process_base += 1;
}

void destroy(struct payload_buffer *buf)
{
	for (int i = 0; i < buf->len; i++) {
		struct payload *payload = &buf->payloads[i];
		payload->destroy(payload);
	}

	free(buf->payloads);
	free(buf);
}


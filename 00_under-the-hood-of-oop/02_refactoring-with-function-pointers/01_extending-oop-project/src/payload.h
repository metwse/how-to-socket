/**
 * @file payload.h
 * @brief Payload methods.
 */


#ifndef PAYLOAD_H
#define PAYLOAD_H


#include <stdbool.h>


union payload_data {
	struct {
		char *username;
		char *password;
	} command_login;

	struct {
		char *channel;
	} command_join;

	struct {
		char *username;
		char *content;
	} message_direct;

	struct {
		char *channel;
		char *content;
	} message_group;

	struct {
		char *content;
	} message_global;
};

struct payload {
	void (*process)(const struct payload *self);
	void (*destroy)(const struct payload *self);
	union payload_data data;
};


/**
 * @brief Constructor method sets up function pointers and data fields of
 *        payloads.
 *
 * @param p Output for parsed payload
 * @param raw Raw payload string to parse
 */
bool parse_payload(struct payload *p, const char *raw);

void process_command_login(const struct payload *self);
void process_command_join(const struct payload *self);
void process_command_logout(const struct payload *self);
void process_message_direct(const struct payload *self);
void process_message_group(const struct payload *self);
void process_message_global(const struct payload *self);

void destroy_command_login(const struct payload *self);
void destroy_command_join(const struct payload *self);
void destroy_command_logout(const struct payload *self);
void destroy_message_direct(const struct payload *self);
void destroy_message_group(const struct payload *self);
void destroy_message_global(const struct payload *self);


#endif

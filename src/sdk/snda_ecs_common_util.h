/*
 * ProvideBy: SNDA Cloud Computing(http://www.grandcloud.cn).
 *
 * Header file of c sdk for Elastic Cloud Storage(ECS) of GrandCloud.
 *
 * This header file defines common utilities for ECS sdk api.
 */

#ifndef SNDA_ECS_COMMON_UTIL_H
#define SNDA_ECS_COMMON_UTIL_H


#include <string.h>
#include <ctype.h>

#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

#include "snda_ecs_sdk.h"


static inline int snda_ecs_is_xml_element_node_with_name(xmlNodePtr node, char* name) {
	return node && node->type == XML_ELEMENT_NODE && !xmlStrcmp(node->name, (const xmlChar*)name);
}

static inline int snda_ecs_is_xml_text_node_with_name(xmlNodePtr node, char* name) {
	return node && node->type == XML_TEXT_NODE && !xmlStrcmp(node->name, (const xmlChar*)name);
}

SNDAECSCommonPrefix* snda_ecs_to_common_prefix(const xmlNodePtr cpelementnode);


static inline void snda_ecs_set_error_info(SNDAECSHandler* handler, SNDAECSResult* ret) {
	int errmsgsize = strlen(handler->handlererrmsg);
	if (errmsgsize > 0) {
		ret->error->handlererrmsg = (char *)malloc(strlen(handler->handlererrmsg) + 1);
		strcpy(ret->error->handlererrmsg, handler->handlererrmsg);
	}
}

static inline int snda_ecs_is_start_with(const char* left, const char* expectedprefix) {
	if (strlen(left) < strlen(expectedprefix)) {
		return 0;
	}
	return strncmp(left, expectedprefix, strlen(expectedprefix)) == 0;
}

static inline void snda_ecs_to_lower_string(const char* left, char* lowercase) {
	unsigned int i = 0;
	for (; i < strlen(left); ++i) {
		char c = left[i];
		if (isupper(c)) {
			c = tolower(c);
		}
		lowercase[i] = c;
	}
	lowercase[i] = '\0';
}

static inline void snda_ecs_set_handler_error(SNDAECSResult* ret, const char* msg) {
	if (!(ret->error)) {
		ret->error = (SNDAECSHandlerError*)malloc(sizeof(SNDAECSHandlerError));
		ret->error->handlererrmsg = 0;
	} else {
		if (ret->error->handlererrmsg) {
			free(ret->error->handlererrmsg);
			ret->error->handlererrmsg = 0;
		}
	}

	ret->error->handlererrmsg = (char*)malloc(strlen(msg) + 1);
	strcpy(ret->error->handlererrmsg, msg);
}

char* snda_ecs_get_content_type(const char* filename, char* contenttype);

#endif  // SNDA_ECS_COMMON_UTIL_H

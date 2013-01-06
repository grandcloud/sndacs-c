/*
 * ProvideBy: SNDA Cloud Computing(http://www.grandcloud.cn).
 *
 * Implementation of c sdk for Elastic Cloud Storage of GrandCloud.
 */
#if defined(WIN32)
#define _CRT_SECURE_NO_DEPRECATE
#endif

#include <string.h>

#include "snda_ecs_sdk.h"
#include "snda_ecs_constants.h"
#include "snda_ecs_http_util.h"
#include "snda_ecs_common_util.h"

SNDAECSGetServiceResultContent* snda_ecs_init_get_service_result_content() {
	SNDAECSGetServiceResultContent* content =
		(SNDAECSGetServiceResultContent*)malloc(sizeof(SNDAECSGetServiceResultContent));
	if (content) {
		content->bucketname = 0;
		content->creationtime = 0;
		content->next = 0;
	}
	return content;
}

void snda_ecs_release_get_service_result_content(SNDAECSGetServiceResultContent* content) {
	if (content) {
		if (content->bucketname) {
			free(content->bucketname);
		}
		if (content->creationtime) {
			free(content->creationtime);
		}
		if (content->next) {
			snda_ecs_release_get_service_result_content(content->next);
		}
		free(content);
		content = 0;
	}
}


SNDAECSErrorCode snda_ecs_get_service(SNDAECSHandler* handler, const char* accesskey,
									  const char* secretkey, int ssl, SNDAECSResult* res) {
	char date[S_SNDA_ECS_DATE_LEN];
    char signature[S_SNDA_ECS_BASIC_MSG_TO_SIGNATURE_LEN];
    char authorheader[S_SNDA_ECS_AUTH_LEN];
    char dateheader[S_SNDA_ECS_DATE_HEADER_LEN];
    char url[S_SNDA_ECS_MAX_URL_LEN];
    struct curl_slist* headers = 0;
    CURLcode retcode = -1;

	get_current_date(date);

	basic_msg_to_signature(signature, "GET", NULL, NULL, date, NULL, "/");
	
	create_authorization_header(authorheader, accesskey, secretkey, signature);
	
	create_date_header(dateheader, date);

    headers = curl_slist_append(NULL, dateheader);
	headers = curl_slist_append(headers, authorheader);

    create_url(url, NULL, "/", ssl);

	snda_ecs_set_handler_attributes(handler,
			headers, url,
			0, 0, 0,
			snda_ecs_get_server_response_body,
			res->serverresponse->responsebody,
			SNDA_ECS_GET, SNDA_ECS_NOT_FOLLOW_LOCATION, 0);

    retcode = curl_easy_perform(handler->handler);

    curl_slist_free_all(headers);

    if (retcode != CURLE_OK) {
    	snda_ecs_set_error_info(handler, res);
    	return SNDA_ECS_HANDLER_ERROR;
    }

    curl_easy_getinfo(handler->handler, CURLINFO_RESPONSE_CODE, &(res->serverresponse->httpcode));

    return SNDA_ECS_SUCCESS;
}

static SNDAECSGetServiceResultContent* snda_ecs_to_get_service_result_content_node(xmlNodePtr cur) {
	SNDAECSGetServiceResultContent* node = 0;
    xmlNodePtr child = 0;

    if (!cur || !snda_ecs_is_xml_element_node_with_name(cur, "Bucket")) {
		return NULL;
	}
	node = snda_ecs_init_get_service_result_content();
	
    child = cur->xmlChildrenNode;
	for (; child; child = child->next) {
		char** ptr = 0;
		if (snda_ecs_is_xml_element_node_with_name(child, "Name")) {
			ptr = &(node->bucketname);
		} else if (snda_ecs_is_xml_element_node_with_name(child, "CreationDate")) {
			ptr = &(node->creationtime);
		} else {
			continue;
		}

		if (child->children->content) {
			char* value = (char*)(child->children->content);
			*ptr = (char*)calloc(strlen(value) + 1, sizeof(char));
			strcpy(*ptr, value);
		}
	}
	return node;
}


SNDAECSGetServiceResultContent* snda_ecs_to_get_service_result(SNDAECSResult* result) {
	xmlDocPtr doc = 0;
    xmlNodePtr cur = 0;
    SNDAECSGetServiceResultContent* header = 0;
	SNDAECSGetServiceResultContent* curnode = 0;

    if (!snda_ecs_is_result_with_content(result)) {
		return NULL;
	}

	doc = xmlParseMemory(result->serverresponse->responsebody->retbody,
			result->serverresponse->responsebody->retbodysize);
	if (!doc) {
		return NULL;
	}

    cur = xmlDocGetRootElement(doc);
	if (!cur || !snda_ecs_is_xml_element_node_with_name(cur, "ListAllMyBucketsResult")) {
		xmlFreeDoc(doc);
		return NULL;
	}

	cur = cur->xmlChildrenNode;
	if (!cur || !snda_ecs_is_xml_element_node_with_name(cur, "Buckets")) {
		xmlFreeDoc(doc);
		return NULL;
	}

	cur = cur->xmlChildrenNode;
	if (!cur || !snda_ecs_is_xml_element_node_with_name(cur, "Bucket")) {
		xmlFreeDoc(doc);
		return NULL;
	}

	for (; cur; cur = cur->next) {
		if (snda_ecs_is_xml_element_node_with_name(cur, "Bucket")) {
			if (!header) {
				header = snda_ecs_to_get_service_result_content_node(cur);
				curnode = header;
			} else {
				curnode->next = snda_ecs_to_get_service_result_content_node(cur);
				curnode = curnode->next;
			}
		}
	}

	xmlFreeDoc(doc);

	return header;
}


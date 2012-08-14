/*
 * ProvideBy: SNDA Cloud Computing(http://www.grandcloud.cn).
 * 
 * Implementation of c sdk for Elastic Cloud Storage of GrandCloud.
 */

#if defined(WIN32)
#define _CRT_SECURE_NO_DEPRECATE
#endif

#include <string.h>
#include <memory.h>

#include "snda_ecs_sdk.h"
#include "snda_ecs_constants.h"
#include "snda_ecs_common_util.h"

void snda_ecs_global_init() {
	CURLcode globalinitret = curl_global_init(CURL_GLOBAL_ALL);
	if (globalinitret != CURLE_OK) {
		printf("FATAL: curl global init failed with return code %d", globalinitret);
		exit(SNDA_ECS_CURL_GLOBAL_INIT_FAIL);
	}
	xmlInitParser();
}

void snda_ecs_global_uninit() {
	curl_global_cleanup();
	xmlCleanupParser();
}

SNDAECSHandler* snda_ecs_init_handler() {
    char* handlererrmsg = 0;
    SNDAECSHandler * handler = 0;
	CURL* curl = curl_easy_init();
	if (!curl) {
		return NULL;
	}

	handlererrmsg = (char *)calloc(CURL_ERROR_SIZE, sizeof(char));
	if (!handlererrmsg) {
		curl_easy_cleanup(curl);
		return NULL;
	}

	handler = malloc(sizeof(SNDAECSHandler));
	if (!handler) {
		curl_easy_cleanup(curl);
		free(handlererrmsg);
		return NULL;
	}
	handler->handler = curl;
	handler->handlererrmsg = handlererrmsg;
	curl_easy_setopt(handler->handler, CURLOPT_ERRORBUFFER, handler->handlererrmsg);

	return handler;
}

void snda_ecs_release_handler(SNDAECSHandler* handler) {
	if (handler) {
		if (handler->handler) {
			curl_easy_cleanup(handler->handler);
		}
		snda_ecs_free_char_ptr(handler->handlererrmsg);

		free(handler);
		handler = 0;
	}
}

SNDAECSHandlerError* snda_ecs_init_handler_error() {
	SNDAECSHandlerError* err = (SNDAECSHandlerError*)malloc(sizeof(SNDAECSHandlerError));
	if (err) {
		err->handlererrmsg = 0;
	}
	return err;
}

void snda_ecs_release_handler_error(SNDAECSHandlerError* error) {
	if (error) {
		snda_ecs_free_char_ptr(error->handlererrmsg);

		free(error);
		error = 0;
	}
}

SNDAECSServerResponseBody* snda_ecs_init_server_response_body() {
	SNDAECSServerResponseBody* body = (SNDAECSServerResponseBody*)malloc(sizeof(SNDAECSServerResponseBody));
	if (body) {
		body->retbodyremain = 0;
		body->retbodysize = 0;
		body->retbody = 0;
	}
	return body;
}

void snda_ecs_release_server_response_body(SNDAECSServerResponseBody* body) {
	if (body) {
		snda_ecs_free_char_ptr(body->retbody);

		free(body);
		body = 0;
	}
}

SNDAECSServerResponse* snda_ecs_init_server_response() {
	SNDAECSServerResponse* res = (SNDAECSServerResponse*)malloc(sizeof(SNDAECSServerResponse));
	if (res) {
		res->httpcode = 0;
		res->responsebody = snda_ecs_init_server_response_body();
	}
	return res;
}
void snda_ecs_release_server_response(SNDAECSServerResponse* response) {
	if (response) {
		if (response->responsebody) {
			snda_ecs_release_server_response_body(response->responsebody);
		}

		free(response);
		response = 0;
	}
}

SNDAECSResult* snda_ecs_init_result() {
	SNDAECSResult* res = (SNDAECSResult*)malloc(sizeof(SNDAECSResult));
	if (res) {
		res->error = snda_ecs_init_handler_error();
		res->serverresponse = snda_ecs_init_server_response();
	}
	return res;
}

void snda_ecs_relase_result(SNDAECSResult* result) {
	if (result) {
		if (result->error) {
			snda_ecs_release_handler_error(result->error);
		}
		if (result->serverresponse) {
			snda_ecs_release_server_response(result->serverresponse);
		}

		free(result);
		result = 0;
	}
}


SNDAECSErrorResponseContent* snda_ecs_init_error_response_content() {
	SNDAECSErrorResponseContent *content = (SNDAECSErrorResponseContent*)malloc(sizeof(SNDAECSErrorResponseContent));
	if (content) {
		content->code = 0;
		content->fullbody = 0;
		content->message = 0;
		content->requestid = 0;
		content->resource = 0;
	}
	return content;
}
void snda_ecs_release_error_response_content(SNDAECSErrorResponseContent* content) {
	if (content) {
		snda_ecs_free_char_ptr(content->code);
		snda_ecs_free_char_ptr(content->fullbody);
		snda_ecs_free_char_ptr(content->message);
		snda_ecs_free_char_ptr(content->requestid);
		snda_ecs_free_char_ptr(content->resource);

		free(content);
		content = 0;
	}
}

SNDAECSResult* snda_ecs_reset_result(SNDAECSResult* res) {
	if (res) {
		if (res->serverresponse) {
			snda_ecs_release_server_response(res->serverresponse);
		}
		if (res->error) {
			snda_ecs_release_handler_error(res->error);
		}
		res->serverresponse = snda_ecs_init_server_response();
		res->error = snda_ecs_init_handler_error();
	}
	return res;
}



SNDAECSGetBucketResultContent* snda_ecs_init_get_bucket_result_content() {
	SNDAECSGetBucketResultContent* content = (SNDAECSGetBucketResultContent*)malloc(sizeof(SNDAECSGetBucketResultContent));
	if (content) {
		content->bucketname = 0;
		content->delimiter = 0;
		content->marker = 0;
		content->nextmarker = 0;
		content->prefix = 0;
		content->istruncated = 0;
		content->maxkeys = 0;
		content->commonprefixes = 0;
		content->objects = 0;
	}
	return content;
}

void snda_ecs_release_get_bucket_result_content(SNDAECSGetBucketResultContent* content) {
	if (content) {
		snda_ecs_free_char_ptr(content->bucketname);
		snda_ecs_free_char_ptr(content->delimiter);
		snda_ecs_free_char_ptr(content->marker);
		snda_ecs_free_char_ptr(content->nextmarker);
		snda_ecs_free_char_ptr(content->prefix);

		if (content->commonprefixes) {
			snda_ecs_release_common_prefix(content->commonprefixes);
		}
		if (content->objects) {
			snda_ecs_release_object_content(content->objects);
		}

		free(content);
		content = 0;
	}
}

SNDAECSObjectContent* snda_ecs_init_object_content() {
	SNDAECSObjectContent* content = (SNDAECSObjectContent*)malloc(sizeof(SNDAECSObjectContent));
	if (content) {
		content->etag = 0;
		content->lastmodified = 0;
		content->objectname = 0;
		content->next = 0;
		content->size = 0L;
	}
	return content;
}

void snda_ecs_release_object_content(SNDAECSObjectContent* content) {
	if (content) {
		snda_ecs_free_char_ptr(content->etag);
		snda_ecs_free_char_ptr(content->lastmodified);
		snda_ecs_free_char_ptr(content->objectname);

		if (content->next) {
			snda_ecs_release_object_content(content->next);
		}

		free(content);
		content = 0;
	}
}

SNDAECSCommonPrefix* snda_ecs_init_common_prefix() {
	SNDAECSCommonPrefix* commonprefix = (SNDAECSCommonPrefix*)malloc(sizeof(SNDAECSCommonPrefix));
	if (commonprefix) {
		commonprefix->commonprefix = 0;
		commonprefix->next = 0;
	}
	return commonprefix;
}

void snda_ecs_release_common_prefix(SNDAECSCommonPrefix* commonprefix) {
	if (commonprefix) {
		snda_ecs_free_char_ptr(commonprefix->commonprefix);

		if (commonprefix->next) {
			snda_ecs_release_common_prefix(commonprefix->next);
		}
		free(commonprefix);
		commonprefix = 0;
	}
}

SNDAECSPair* snda_ecs_init_pair() {
	SNDAECSPair* pair = (SNDAECSPair*)malloc(sizeof(SNDAECSPair));
	if (pair) {
		pair->key = 0;
		pair->value = 0;
	}
	return pair;
}

void snda_ecs_release_pair(SNDAECSPair* pair) {
	if (pair) {
		snda_ecs_free_char_ptr(pair->key);
		snda_ecs_free_char_ptr(pair->value);

		free(pair);
		pair = 0;
	}
}

SNDAECSKVList* snda_ecs_init_k_v_list() {
	SNDAECSKVList* list = (SNDAECSKVList*)malloc(sizeof(SNDAECSKVList));
	if (list) {
		list->key = 0;
		list->value = 0;
		list->next = 0;
	}
	return list;
}

void snda_ecs_release_k_v_list(SNDAECSKVList* kvlist) {
	if (kvlist) {
		snda_ecs_free_char_ptr(kvlist->key);
		snda_ecs_free_char_ptr(kvlist->value);
		if (kvlist->next) {
			snda_ecs_release_k_v_list(kvlist->next);
		}

		free(kvlist);
		kvlist = 0;
	}
}

SNDAECSBucketLocation* snda_ecs_init_bucket_location() {
	SNDAECSBucketLocation* location = (SNDAECSBucketLocation*)malloc(sizeof(SNDAECSBucketLocation));
	if (location) {
		location->location = 0;
	}
	return location;
}

void snda_ecs_release_bucket_location(SNDAECSBucketLocation* bucketlocation) {
	if (bucketlocation) {
		if (bucketlocation->location) {
			free(bucketlocation->location);
		}
		free(bucketlocation);
		bucketlocation = 0;
	}
}

SNDAECSStringList* snda_ecs_init_string_list() {
	SNDAECSStringList* strlist = (SNDAECSStringList*)malloc(sizeof(SNDAECSStringList));
	if (strlist) {
		strlist->string = 0;
		strlist->next = 0;
	}
	return strlist;
}


void snda_ecs_release_string_list(SNDAECSStringList* stringlist) {
	if (stringlist) {
		snda_ecs_free_char_ptr(stringlist->string);

		snda_ecs_release_string_list(stringlist->next);

		free(stringlist);
		stringlist = 0;
	}
}

SNDAECSByteRange* snda_ecs_init_byte_range() {
	SNDAECSByteRange* byterange = (SNDAECSByteRange*)malloc(sizeof(SNDAECSByteRange));
	if (byterange) {
		byterange->first = 0L;
		byterange->last = 0L;
	}
	return byterange;
}

void snda_ecs_release_byte_range(SNDAECSByteRange* byterange) {
	if (byterange) {
		free(byterange);
		byterange = 0;
	}
}

SNDAECSUserObjectMeta* snda_ecs_init_user_object_meta() {
	SNDAECSUserObjectMeta* userobjectmeta = (SNDAECSUserObjectMeta*)malloc(sizeof(SNDAECSUserObjectMeta));
	if (userobjectmeta) {
		userobjectmeta->contentmd5 = 0;
		userobjectmeta->contenttype = 0;
		userobjectmeta->usermetas = 0;
	}
	return userobjectmeta;
}

void snda_ecs_release_user_object_meta(SNDAECSUserObjectMeta * userobjectmeta) {
	if (userobjectmeta) {
		snda_ecs_free_char_ptr(userobjectmeta->contentmd5);
		snda_ecs_free_char_ptr(userobjectmeta->contenttype);
		snda_ecs_release_k_v_list(userobjectmeta->usermetas);
		free(userobjectmeta);
		userobjectmeta = 0;
	}
}

SNDAECSObjectMeta* snda_ecs_init_object_meta() {
	SNDAECSObjectMeta* objectmeta = (SNDAECSObjectMeta*)malloc(sizeof(SNDAECSObjectMeta));
	if (objectmeta) {
		objectmeta->contentlength = 0;
		objectmeta->contenttype = 0;
		objectmeta->etag = 0;
		objectmeta->lastmodified = 0;
		objectmeta->usermetas = 0;
	}
	return objectmeta;
}
void snda_ecs_release_object_meta(SNDAECSObjectMeta* objectmeta) {
	if (objectmeta) {
		snda_ecs_free_char_ptr(objectmeta->contenttype);
		snda_ecs_free_char_ptr(objectmeta->etag);
		snda_ecs_free_char_ptr(objectmeta->lastmodified);

		snda_ecs_release_k_v_list(objectmeta->usermetas);

		free(objectmeta);
		objectmeta = 0;
	}
}

SNDAECSInitiateMultipartUploadResult* snda_ecs_init_initiate_multipart_upload_result() {
	SNDAECSInitiateMultipartUploadResult* ret =
			(SNDAECSInitiateMultipartUploadResult*)malloc(sizeof(SNDAECSInitiateMultipartUploadResult));
	if (ret) {
		ret->bucket = 0;
		ret->key = 0;
		ret->uploadid = 0;
	}
	return ret;
}

void snda_ecs_release_initiate_multipart_upload_result(SNDAECSInitiateMultipartUploadResult* ret) {
	if (ret) {
		snda_ecs_free_char_ptr(ret->bucket);
		snda_ecs_free_char_ptr(ret->key);
		snda_ecs_free_char_ptr(ret->uploadid);

		free(ret);
		ret = 0;
	}
}

SNDAECSMultipartsMeta* snda_ecs_init_multiparts_meta() {
	SNDAECSMultipartsMeta* partsmeta = (SNDAECSMultipartsMeta*)malloc(sizeof(SNDAECSMultipartsMeta));
	if (partsmeta) {
		partsmeta->partnumber = 0;
		partsmeta->etag = 0;
		partsmeta->next = 0;
	}
	return partsmeta;
}

void snda_ecs_release_multiparts_meta(SNDAECSMultipartsMeta* partsmeta) {
	if (partsmeta) {
		snda_ecs_free_char_ptr(partsmeta->etag);

		snda_ecs_release_multiparts_meta(partsmeta->next);

		free(partsmeta);
		partsmeta = 0;
	}
}

SNDAECSMultipartsPart* snda_ecs_init_multiparts_part() {
	SNDAECSMultipartsPart* part = (SNDAECSMultipartsPart*)malloc(sizeof(SNDAECSMultipartsPart));
	if (part) {
		part->etag = 0;
		part->lastmodified = 0;
		part->next = 0;
		part->partnumber = 0;
		part->size = 0L;
	}
    return part;
}

void snda_ecs_release_multiparts_part(SNDAECSMultipartsPart* part) {
	if (part) {
		snda_ecs_free_char_ptr(part->etag);
		snda_ecs_free_char_ptr(part->lastmodified);

		snda_ecs_release_multiparts_part(part->next);

		free(part);
		part = 0;
	}
}

SNDAECSMultipartsContent* snda_ecs_init_multiparts_content() {
	SNDAECSMultipartsContent* content = (SNDAECSMultipartsContent*)malloc(sizeof(SNDAECSMultipartsContent));
	if (content) {
		content->bucket = 0;
		content->key = 0;
		content->uploadid = 0;
		content->parts = 0;
		content->istruncated = 0;
		content->maxparts = 0;
		content->nextpartnumbermarker = 0;
		content->partnumbermarker = 0;
	}
	return content;
}

void snda_ecs_release_multiparts_content(SNDAECSMultipartsContent* content) {
	if (content) {
		snda_ecs_free_char_ptr(content->bucket);
		snda_ecs_free_char_ptr(content->key);
		snda_ecs_free_char_ptr(content->uploadid);

		snda_ecs_release_multiparts_part(content->parts);

		free(content);
		content = 0;
	}
}

SNDAECSMultipartUpload* snda_ecs_init_multipart_upload() {
	SNDAECSMultipartUpload* upload = (SNDAECSMultipartUpload*)malloc(sizeof(SNDAECSMultipartUpload));
	if (upload) {
		upload->key = 0;
		upload->initiatedtime = 0;
		upload->uploadid = 0;
		upload->next = 0;
	}
	return upload;
}

void snda_ecs_release_multipart_upload(SNDAECSMultipartUpload* upload) {
	if (upload) {
		snda_ecs_free_char_ptr(upload->key);
		snda_ecs_free_char_ptr(upload->initiatedtime);
		snda_ecs_free_char_ptr(upload->uploadid);

		snda_ecs_release_multipart_upload(upload->next);

		free(upload);
		upload = 0;
	}
}

SNDAECSMultipartUploadsContent* snda_ecs_init_multipart_uploads_content() {
	SNDAECSMultipartUploadsContent* content =
			(SNDAECSMultipartUploadsContent*)malloc(sizeof(SNDAECSMultipartUploadsContent));
	if (content) {
		content->bucket = 0;
		content->commonprefixes = 0;
		content->delimiter = 0;
		content->istruncated = 0;
		content->keymarker = 0;
		content->maxuploads = 0;
		content->nextkeymarker = 0;
		content->nextuploadidmarker = 0;
		content->prefix = 0;
		content->upload = 0;
		content->uploadidmarker = 0;
	}
	return content;
}

void snda_ecs_release_multipart_uploads_content(SNDAECSMultipartUploadsContent* content) {
	if (content) {
		snda_ecs_free_char_ptr(content->bucket);
		snda_ecs_free_char_ptr(content->delimiter);
		snda_ecs_free_char_ptr(content->keymarker);
		snda_ecs_free_char_ptr(content->nextkeymarker);
		snda_ecs_free_char_ptr(content->nextuploadidmarker);
		snda_ecs_free_char_ptr(content->prefix);
		snda_ecs_free_char_ptr(content->uploadidmarker);

		snda_ecs_release_multipart_upload(content->upload);
		snda_ecs_release_common_prefix(content->commonprefixes);

		free(content);
		content = 0;
	}
}

int snda_ecs_is_result_with_content(SNDAECSResult* result) {
	return result &&
		result->serverresponse &&
		result->serverresponse->responsebody &&
		result->serverresponse->responsebody->retbody &&
		result->serverresponse->responsebody->retbodysize > 0;
}

char * snda_ecs_get_full_response(SNDAECSResult* ret) {
    if (!snda_ecs_is_result_with_content(ret)) {
		return NULL;
	} else {
		char * result = (char*)calloc(ret->serverresponse->responsebody->retbodysize + 1, sizeof(char));
		memcpy(result,ret->serverresponse->responsebody->retbody,
		ret->serverresponse->responsebody->retbodysize);
		return result;
	}
}

SNDAECSErrorResponseContent* snda_ecs_to_error_response(SNDAECSResult* result) {
	int i = 0;    
    xmlDocPtr doc = 0;
    xmlNodePtr cur = 0;
    SNDAECSErrorResponseContent* content = 0;

    if (!snda_ecs_is_result_with_content(result)) {
		return NULL;
	}

	// we may have headers in response
	for (; i < result->serverresponse->responsebody->retbodysize; ++i) {
		if (((char*)(result->serverresponse->responsebody->retbody))[i] == '<') {
			break;
		}
	}

	if (i >= result->serverresponse->responsebody->retbodysize) {
		return NULL;
	}

	doc = xmlParseMemory((char*)(result->serverresponse->responsebody->retbody) + i,
			result->serverresponse->responsebody->retbodysize - i);
	if (!doc) {
		return NULL;
	}

	cur = xmlDocGetRootElement(doc);
	if (!cur) {
		xmlFreeDoc(doc);
		return NULL;
	}

	if (!snda_ecs_is_xml_element_node_with_name(cur, "Error")) {
		xmlFreeDoc(doc);
		return NULL;
	}

	content = snda_ecs_init_error_response_content();
	for (cur = cur->xmlChildrenNode; cur; cur = cur->next) {
		char** ptr = 0;
        char* value = (char*)(cur->children->content);

		if (snda_ecs_is_xml_element_node_with_name(cur, "Code")) {
			ptr = &(content->code);
		} else if (snda_ecs_is_xml_element_node_with_name(cur, "Message")) {
			ptr = &(content->message);
		} else if (snda_ecs_is_xml_element_node_with_name(cur, "Resource")) {
			ptr = &(content->resource);
		} else if (snda_ecs_is_xml_element_node_with_name(cur, "RequestId")) {
			ptr = &(content->requestid);
		} else {
			continue;
		}

		*ptr = (char*)calloc(strlen(value) + 1, sizeof(char));
		strcpy(*ptr, value);
	}

	xmlFreeDoc(doc);

	content->fullbody = (char*)calloc(result->serverresponse->responsebody->retbodysize + 1, sizeof(char));
	memcpy(content->fullbody,
		result->serverresponse->responsebody->retbody,
		result->serverresponse->responsebody->retbodysize);

	return content;
}








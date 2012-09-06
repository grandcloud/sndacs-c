/*
 * ProvideBy: SNDA Cloud Computing(http://www.grandcloud.cn).
 *
 * Header file of c sdk for Elastic Cloud Storage(ECS) of GrandCloud.
 *
 * This header file defines http utl for ECS sdk api.
 */
#if defined(WIN32)
#define _CRT_SECURE_NO_DEPRECATE
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

#include "snda_ecs_http_util.h"
#include "snda_ecs_constants.h"
#include "snda_ecs_encode.h"
#include "snda_ecs_sdk.h"
#include "snda_ecs_common_util.h"

char* create_url(char* urlbuff,
		const char* region,
		const char* resource,
		int usessl)
{
	const char* snda_ecs_default_hostname = "storage.grandcloud.cn";
	const char* snda_ecs_huadong1_hostname = "storage-huadong-1.grandcloud.cn";
	const char* snda_ecs_huabei1_hostname = "storage-huabei-1.grandcloud.cn";
	const char* snda_ecs_region_huadong1 = "huadong-1";
	const char* snda_ecs_region_huabei1 = "huabei-1";

	const char* host = snda_ecs_default_hostname;
	char* ptr = urlbuff;
	char* prefix = "http://";
	int port = 80;
	if (usessl) {
		prefix = "https://";
		port = 443;
	}
	
	if (region) {
		if (!strcmp(region, snda_ecs_region_huadong1)) {
			host = snda_ecs_huadong1_hostname;
		} else if (!strcmp(region, snda_ecs_region_huabei1)) {
			host = snda_ecs_huabei1_hostname;
		}
	}

	ptr += sprintf(ptr, "%s%s:%d", prefix, host, port);
	if (resource) {
		sprintf(ptr, "%s", resource);
	} else {
		sprintf(ptr, "%s", "/");
	}

	return urlbuff;
}

char* create_date_header(char* dateheaderbuff, char* datebuff) {
	sprintf(dateheaderbuff, "Date: %s", datebuff);
	return dateheaderbuff;
}

char* create_authorization_header(char* authbuff,
		const char* accesskey,
		const char * secretkey,
		const char* msgtosignature)
{
	char* signature = 0;
	char sha1buff[S_SNDA_ECS_SHA1_LEN] = { 0 };
	int sha1_len = hmac_sha1(secretkey, msgtosignature, strlen(msgtosignature), sha1buff);

	assert (sha1_len == S_SNDA_ECS_SHA1_LEN);

	signature = base64_encode(sha1buff, sha1_len);

	sprintf(authbuff, "Authorization: SNDA %s:%s", accesskey, signature);

	free(signature);

	return authbuff;
}

char* get_current_date(char* datebuff)
{
	int offset = 0;
    time_t now = time(0);
    sprintf(datebuff, "%s", asctime(gmtime(&now)));
	offset = strlen(datebuff);
	if (datebuff[offset - 1] == '\n') {
		offset -= 1;
	}
	sprintf(datebuff + offset, "%s", " UTC");
	return datebuff;
}

char* basic_msg_to_signature(char* msgtosignature,
		const char* httpverb,
		const char* contentmd5,
		const char* contenttype,
		const char* date,
		const char* canonicalizedheaders,
		const char* urlresource)
{
	sprintf(msgtosignature, "%s\n", httpverb);
	if (contentmd5) {
		sprintf(msgtosignature + strlen(msgtosignature), "%s", contentmd5);
	}
	sprintf(msgtosignature + strlen(msgtosignature), "\n");

	if (contenttype) {
		sprintf(msgtosignature + strlen(msgtosignature), "%s", contenttype);
	}
	sprintf(msgtosignature + strlen(msgtosignature), "\n");

	sprintf(msgtosignature + strlen(msgtosignature), "%s\n", date);

	if (canonicalizedheaders) {
		sprintf(msgtosignature + strlen(msgtosignature), "%s", canonicalizedheaders);
	}
	//printf("canonicalizedheaders:%s\n",canonicalizedheaders);
	sprintf(msgtosignature + strlen(msgtosignature), "%s", urlresource);

	return msgtosignature;
}


size_t snda_ecs_get_server_response_body(void *ptr, size_t size, size_t number, void *stream)
{
	SNDAECSServerResponseBody *res = (SNDAECSServerResponseBody *)stream;
	int comedatasize = size * number;
	
	// the first day, res->retbodyremain is init to be 0 and res->retbody is set to be NULL.
	if (res->retbodyremain < comedatasize) {
		int moresize = S_SNDA_ECS_RET_BODY_ONE_TIME_ALLOCATE_LEN > comedatasize ?
				S_SNDA_ECS_RET_BODY_ONE_TIME_ALLOCATE_LEN :
				comedatasize;
		int nextsize = res->retbodysize + moresize;
		res->retbody = realloc(res->retbody, nextsize);
		res->retbodyremain = moresize;
	}
    memcpy((char*)(res->retbody) + res->retbodysize, ptr, comedatasize);
    res->retbodysize += comedatasize;
    res->retbodyremain -= comedatasize;
    return comedatasize;
}

size_t snda_ecs_put_xml_body(void *ptr, size_t size, size_t nmemb, void *stream) {
	return snda_ecs_put_mem_body(ptr, size, nmemb, stream);	
}
size_t snda_ecs_put_mem_body(void *ptr, size_t size, size_t nmemb, void *stream)
{
	SNDAECSReadBuff *buff = (SNDAECSReadBuff*)stream;
	long remain = buff->datasize - buff->consumed;
	long thistimeconsumed = remain > nmemb * size ? nmemb * size : remain;
    if (remain == 0) {
    	return 0;
    }
    
    memcpy(ptr, (char*)(buff->databuff) + buff->consumed,  thistimeconsumed);
    buff->consumed += thistimeconsumed;

    return thistimeconsumed;
}

size_t snda_ecs_put_object_body(void *ptr, size_t size, size_t nmemb, void *stream) {
	SNDAECSReadBuff *buff = (SNDAECSReadBuff*)stream;

	long remain = buff->datasize - buff->consumed;
	long thistimeconsumed = remain > nmemb * size ? nmemb * size : remain;
	FILE* fd = 0;
	long actualconsumed = 0;
	
	if (remain == 0) {
		return 0;
	}
	

	fd = (FILE*)(buff->databuff);

	actualconsumed = fread(ptr, sizeof(char), thistimeconsumed, fd);
	buff->consumed += actualconsumed;
	return actualconsumed;
}

size_t snda_ecs_write_fun(void *ptr, size_t size, size_t number, void *stream) {
	int comedatasize = size * number;
	FILE* output = (FILE*) stream;
	size_t actualwrite = 0;
	do {
		actualwrite += fwrite((char*)ptr + actualwrite, sizeof(char),
				comedatasize - actualwrite, output);
	} while (actualwrite < comedatasize);

	return comedatasize;
}

void snda_ecs_set_handler_attributes(
		SNDAECSHandler* handler, struct curl_slist* headers, const char* url,
		CallbackFunPtr readfun, void* readerptr, long readerdatasize,
		CallbackFunPtr writefun, void* writerptr, SNDAECSHandleType type,
		SNDAECSFollowLocation followlocation, long maxredirects)
{
	curl_easy_reset(handler->handler);              // reset for reusable

	curl_easy_setopt(handler->handler, CURLOPT_ERRORBUFFER, handler->handlererrmsg);
	curl_easy_setopt(handler->handler, CURLOPT_HTTPHEADER, headers);
	curl_easy_setopt(handler->handler, CURLOPT_URL, url);

	curl_easy_setopt(handler->handler, CURLOPT_WRITEFUNCTION, writefun);
	curl_easy_setopt(handler->handler, CURLOPT_WRITEDATA, writerptr);

	if (followlocation == SNDA_ECS_FOLLOW_LOCATION) {
		curl_easy_setopt(handler->handler, CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(handler->handler, CURLOPT_MAXREDIRS, maxredirects);
	}

	switch (type) {
	case SNDA_ECS_PUT_RESPONSE_HEAD:
		curl_easy_setopt(handler->handler, CURLOPT_HEADER, 1);
	case SNDA_ECS_PUT:
		curl_easy_setopt(handler->handler, CURLOPT_READFUNCTION, readfun);
		curl_easy_setopt(handler->handler, CURLOPT_READDATA, readerptr);
		curl_easy_setopt(handler->handler, CURLOPT_UPLOAD, 1L);
		curl_easy_setopt(handler->handler, CURLOPT_INFILESIZE_LARGE, (curl_off_t)readerdatasize);
		break;
	case SNDA_ECS_GET:
		curl_easy_setopt(handler->handler, CURLOPT_HTTPGET, 1L);
		break;
	case SNDA_ECS_DELETE:
		curl_easy_setopt(handler->handler, CURLOPT_CUSTOMREQUEST, "DELETE");
		break;
	case SNDA_ECS_HEAD:
		curl_easy_setopt(handler->handler, CURLOPT_HEADER, 1);
		curl_easy_setopt(handler->handler, CURLOPT_NOBODY, 1);
		curl_easy_setopt(handler->handler, CURLOPT_CUSTOMREQUEST, "HEAD");
		break;
	case SNDA_ECS_POST:
		curl_easy_setopt(handler->handler, CURLOPT_CUSTOMREQUEST, "POST");
		curl_easy_setopt(handler->handler, CURLOPT_READFUNCTION, readfun);
		curl_easy_setopt(handler->handler, CURLOPT_READDATA, readerptr);
		curl_easy_setopt(handler->handler, CURLOPT_UPLOAD, 1L);
		curl_easy_setopt(handler->handler, CURLOPT_INFILESIZE_LARGE, (curl_off_t)readerdatasize);
		break;
	default:
		break;
	}
}

static int snda_ecs_str_comparator(const void* left, const void* right) {
	return strcmp((*((SNDAECSKVList* const *)left))->key, (*((SNDAECSKVList* const *)right))->key);
}

static SNDAECSKVList* snda_ecs_key_to_lower(const SNDAECSKVList *left) {
	SNDAECSKVList* head = 0;
	SNDAECSKVList* cur = 0;
	const SNDAECSKVList* p = left;

	for (; p; p = p->next) {
		if (!cur) {
			cur = snda_ecs_init_k_v_list();
			head = cur;
		} else {
			cur->next = snda_ecs_init_k_v_list();
			cur = cur->next;
		}
		cur->key = calloc(strlen(p->key) + 1, sizeof(char));
		snda_ecs_to_lower_string(p->key, cur->key);
		cur->value = calloc(strlen(p->value) + 1, sizeof(char));
		memcpy(cur->value, p->value, strlen(p->value));
	}
	return head;
}

static char* snda_ecs_to_canonicalized_snda_headers(const SNDAECSKVList * header) {
	const char* snda_canonicalized_prefix = "x-snda-";
	int canonicalizedheaders = 0;
	int headermaxsize = 0;
	SNDAECSKVList** pairarr = 0;
	SNDAECSKVList** p = 0;
	char *headerstring = 0;
	char *lastkey = 0;
	int i = 0;

	SNDAECSKVList* keylowercasepairs = 0;
	SNDAECSKVList* pupet = 0;
	
	if (!header) {
		return NULL;
	}

	keylowercasepairs = snda_ecs_key_to_lower(header);
	pupet = keylowercasepairs;

	for (; pupet; pupet = pupet->next) {
		if (snda_ecs_is_start_with(pupet->key, snda_canonicalized_prefix)) {
			++canonicalizedheaders;
		}
	}

	if (!canonicalizedheaders) {
		snda_ecs_release_k_v_list(keylowercasepairs);
		return NULL;
	}
	
	pairarr = (SNDAECSKVList**)malloc(canonicalizedheaders * sizeof(SNDAECSKVList*));
	p = pairarr;
	for (pupet = keylowercasepairs; pupet; pupet = pupet->next) {
		if (snda_ecs_is_start_with(pupet->key, snda_canonicalized_prefix)) {
			//headermaxsize += strlen(pupet->key) + strlen(pupet->value) + 1;
			//note: consider '\n' and ':' --modified by andremouche
			headermaxsize += strlen(pupet->key) + strlen(pupet->value) + 2; 
			*(p++) = pupet;
		}
	}

	qsort(pairarr, canonicalizedheaders, sizeof(SNDAECSKVList*), snda_ecs_str_comparator);

	headerstring = calloc(headermaxsize + 1, sizeof(char));
	for (; i < canonicalizedheaders; ++i) {
		if (!lastkey) {
			sprintf(headerstring, "%s:%s", pairarr[i]->key, pairarr[i]->value);
			lastkey = pairarr[i]->key;
			continue;
		}

		if (!strcmp(lastkey, pairarr[i]->key)) {
			sprintf(headerstring + strlen(headerstring), ",%s", pairarr[i]->value);
			continue;
		}
		sprintf(headerstring + strlen(headerstring), "\n%s:%s", pairarr[i]->key, pairarr[i]->value);
		lastkey = pairarr[i]->key;
	}
	//headerstring[strlen(headerstring)] = '\n';
	//note: append '\0' at the end of the string --modified by andremouche(2012-07-28)
	sprintf(headerstring + strlen(headerstring), "\n"); 
	free(pairarr);
	snda_ecs_release_k_v_list(keylowercasepairs);
	return headerstring;
}

static SNDAECSStringList* snda_ecs_create_user_object_headers(const SNDAECSUserObjectMeta * const userobjectmeta)
{
	SNDAECSStringList* stringlist = snda_ecs_init_string_list();
	SNDAECSStringList* cur = stringlist;
	SNDAECSKVList * p = 0;
	const char* contentmd5prefix = "Content-MD5: ";
	const char* contenttypeprefix = "Content-Type: ";

	if (userobjectmeta->contentmd5) {
		cur->string = calloc(strlen(contentmd5prefix) + strlen(userobjectmeta->contentmd5) + 1, sizeof(char));
		sprintf(cur->string, "%s%s", contentmd5prefix, userobjectmeta->contentmd5);
	}

	if (userobjectmeta->contenttype) {
		if (cur->string) {
			cur->next = snda_ecs_init_string_list();
			cur = cur->next;
		}
		
		cur->string = calloc(strlen(contenttypeprefix) + strlen(userobjectmeta->contenttype) + 1, sizeof(char));
		sprintf(cur->string, "%s%s", contenttypeprefix, userobjectmeta->contenttype);
	}

	p = userobjectmeta->usermetas;
	for (; p; p = p->next) {
		if (p->key) {
			if (cur->string) {
				cur->next = snda_ecs_init_string_list();
				cur = cur->next;
			}
			cur->string = calloc(strlen(p->key) + strlen(": ") + strlen(p->value) + 1, sizeof(char));
			sprintf(cur->string, "%s: %s", p->key, p->value);
		}
	}

	if (!(stringlist->string)) {
		snda_ecs_release_string_list(stringlist);
		stringlist = 0;
	}
	return stringlist;
}


void snda_ecs_get_common_opt_attributes(const char* accesskey,
		 const char* secretkey, const char* bucketname, const char* subresource, const char* subresourceneedtosign,
		 const char* contentmd5, const char* contenttype, const char* canonicalizedheaders,
		 const char* region, int ssl, SNDAECSHandleType type,
		 struct curl_slist** headersptr, char* url)
{
	const char* root = "/";
	int urlresourcebuffsize = strlen(root) + strlen(bucketname) + 1;

	int resourcetosignsize = urlresourcebuffsize;
    int urlresourcebuffsizefull = urlresourcebuffsize;

    char* urlresource = 0;
    char* resourcetosign = 0;
    char* urlresourcefull = 0;
    char* httpverb = "PUT";
    char date[S_SNDA_ECS_DATE_LEN];
    char signature[S_SNDA_ECS_BASIC_MSG_TO_SIGNATURE_LEN];
    char authorheader[S_SNDA_ECS_AUTH_LEN];
    char dateheader[S_SNDA_ECS_DATE_HEADER_LEN];
    struct curl_slist* headers = 0;

	if (subresourceneedtosign) {
		resourcetosignsize += strlen(subresourceneedtosign);
	}
	
	if (subresource) {
		urlresourcebuffsizefull += strlen(subresource);
	}
    urlresource = (char*)malloc(urlresourcebuffsize);
    resourcetosign = (char*)malloc(resourcetosignsize);
    urlresourcefull = (char*)malloc(urlresourcebuffsizefull);

	sprintf(urlresource, "/%s", bucketname);
	sprintf(resourcetosign, "%s", urlresource);
	if (subresourceneedtosign) {
		sprintf(resourcetosign + strlen(resourcetosign), "%s", subresourceneedtosign);
	}
	sprintf(urlresourcefull, "%s", urlresource);
	if (subresource) {
		sprintf(urlresourcefull + strlen(urlresourcefull), "%s", subresource);
	}

	create_url(url, region, urlresourcefull, ssl);

	switch (type) {
	case SNDA_ECS_GET:
		httpverb = "GET";
		break;
	case SNDA_ECS_DELETE:
		httpverb = "DELETE";
		break;
	case SNDA_ECS_HEAD:
		httpverb = "HEAD";
		break;
	case SNDA_ECS_POST:
		httpverb = "POST";
		break;
	default:
		break;
	}

    get_current_date(date);

	basic_msg_to_signature(signature, httpverb, contentmd5, contenttype, date, canonicalizedheaders, resourcetosign);
	//printf("basic message to signature:\n%s\n",signature);
	create_authorization_header(authorheader, accesskey, secretkey, signature);
	create_date_header(dateheader, date);

    headers = curl_slist_append(NULL, dateheader);
	headers = curl_slist_append(headers, authorheader);
	*headersptr = headers;
    snda_ecs_free_char_ptr(urlresource);
    snda_ecs_free_char_ptr(resourcetosign);
    snda_ecs_free_char_ptr(urlresourcefull);
}


SNDAECSErrorCode snda_ecs_common_opt(SNDAECSHandler* handler, const char* accesskey, const char* secretkey,
		const char* bucketname, const char* region, const char* subresource, const char* subresourcetosign, int ssl, SNDAECSByteRange* byterange,
		const SNDAECSUserObjectMeta* userobjectmeta, SNDAECSHandleType type, SNDAECSFollowLocation followlocation, long maxredirects,
		CallbackFunPtr readfun, void* inputstream, long inputlength, CallbackFunPtr writefun, void* outputstream,
		SNDAECSResult* ret)
{
	char* contentmd5 = 0;
	char* contenttype = 0;
	char* canonicalizedheaders = 0;
    char url[S_SNDA_ECS_MAX_URL_LEN];
	char byterangeheader[S_SNDA_ECS_BYTE_RANGE_HEADER_LEN];
    struct curl_slist* headers = 0;
    SNDAECSStringList* p = 0;
    CURLcode retcode = -1;

	if (userobjectmeta) {
		contentmd5 = userobjectmeta->contentmd5;
		contenttype = userobjectmeta->contenttype;
		canonicalizedheaders = snda_ecs_to_canonicalized_snda_headers(userobjectmeta->usermetas);
		//printf("canonicalizedheaders:%s\n",canonicalizedheaders);
	}

	snda_ecs_get_common_opt_attributes(accesskey, secretkey,
			bucketname, subresource, subresourcetosign,
			contentmd5, contenttype, canonicalizedheaders,
			region, ssl, type,
			&headers, url);

	if (byterange) {
		sprintf(byterangeheader, "Range: bytes=%ld-%ld", byterange->first, byterange->last);
		headers = curl_slist_append(headers, byterangeheader);
	}

	if (userobjectmeta) {
		p = snda_ecs_create_user_object_headers(userobjectmeta);
		for (; p; p = p->next) {
			if (p->string) {
				headers = curl_slist_append(headers, p->string);
			}
		}
	}

	snda_ecs_set_handler_attributes(handler,
				headers, url,
				readfun, inputstream, inputlength,
				writefun,
				outputstream,
				type, followlocation, maxredirects);

	retcode = curl_easy_perform(handler->handler);

	curl_slist_free_all(headers);
	snda_ecs_release_string_list(p);
	//printf("canonicalizedheaders:%s %d\n",canonicalizedheaders,strlen(canonicalizedheaders));
	snda_ecs_free_char_ptr(canonicalizedheaders);

	if (retcode != CURLE_OK) {
		snda_ecs_set_error_info(handler, ret);
		return SNDA_ECS_HANDLER_ERROR;
	}

	curl_easy_getinfo(handler->handler, CURLINFO_RESPONSE_CODE, &(ret->serverresponse->httpcode));

	return SNDA_ECS_SUCCESS;
}


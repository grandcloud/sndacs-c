/*
 * ProvideBy: SNDA Cloud Computing(http://www.grandcloud.cn).
 *
 * Header file of c sdk for Elastic Cloud Storage(ECS) of GrandCloud.
 *
 * This header file defines http utl for ECS sdk api.
 */

#ifndef SNDA_ECS_HTTP_URL_H
#define SNDA_ECS_HTTP_URL_H

#include "snda_ecs_sdk.h"


/**
 * @param urlbuff: user given container to hold target url string,
 * 				   make sure this buffer is big enough to hold target url strlen.
 *                 S_SNDA_ECS_URL_MAX_LEN defined in <snda_ecs_constants.h> is enough.
 * @return: argument urlbuff will be returned, after invoke this method,
 * 			urlbuff will contains a string terminated by '\0'
 */
char* create_url(char* urlbuff, const char* region, const char* resource, int usessl);

/**
 * @param authbuff: user given container to hold target authorization string,
 * 					make sure this buffer is big enough to hold a authorization
 * 					S_SNDA_ECS_AUTH_LEN defined in <snda_ecs_constants.h> is appropriate.
 * @param accesskey:
 * @param secretkey:
 * @param msgtosignature: string to signature, describe in part 4.3 in Grand Cloud Storage Developer Guide
 * @return: argument authbuff will be returned, after invoke this method,
 * 			authbuff will contains a string terminated by '\0'
 */
char* create_authorization_header(char* authbuff,
		const char* accesskey,
		const char* secretkey,
		const char* msgtosignature);

/**
 * @param dateheaderbuff user given container to hold target date header string,
 * 		                 make sure buffer capacity is big enough,
 * 		                 S_SNDA_ECS_DATE_HEADER_LEN defined in <snda_ecs_constants.h> is appropriate.
 * @param datebuff proper date format described in RFC822
 * @return a ptr point to dateheaderbuff will be returned with a string terminated by '\0'
 */
char* create_date_header(char* dateheaderbuff, char* datebuff);

/**
 * @param  datebuff user given buffer to hold date format described in RFC822,
 * 		   make sure buffer capacity is big enough,
 * 		   S_SNDA_ECS_DATE_LEN defined in <snda_ecs_constants.h> is appropriate.
 * @return a ptr point to datebuff will be returned with a string terminated by '\0'
 */
char* get_current_date(char* datebuff);


/**
 * @msgtosignature user given buffer to hold target message to signature,
 * 				   make sure buffer capacity is big enough,
 *                 S_SNDA_ECS_BASIC_MSG_TO_SIGNATURE_LEN defined in <snda_ecs_constants.h> is appropriate.
 * @param httpverb  PUT GET POST DELETE
 * @param contentmd5
 * @param contenttype
 * @param date
 * @param canonicalizedheaders
 * @param urlresource
 * @return argument msgtosignature will be returned, contains a string terminated by '\0'
 */
char* basic_msg_to_signature(char* msgtosignature,
		const char* httpverb,
		const char* contentmd5,
		const char* contenttype,
		const char* date,
		const char* canonicalizedheaders,
		const char* urlresource);


typedef struct SNDAECSReadBuff {
	void* databuff;
	long datasize;
	long consumed;
} SNDAECSReadBuff;

size_t snda_ecs_get_server_response_body(void *ptr, size_t size, size_t number, void *stream);

size_t snda_ecs_put_xml_body(void *ptr, size_t size, size_t nmemb, void *stream);

size_t snda_ecs_put_object_body(void *ptr, size_t size, size_t nmemb, void *stream);

size_t snda_ecs_write_fun(void *ptr, size_t size, size_t number, void *stream);

typedef enum SNDAECSHandleType{
	SNDA_ECS_PUT,
	SNDA_ECS_GET,
	SNDA_ECS_DELETE,
	SNDA_ECS_HEAD,
	SNDA_ECS_POST,
	SNDA_ECS_PUT_RESPONSE_HEAD,
} SNDAECSHandleType;



void snda_ecs_set_handler_attributes(SNDAECSHandler* handler,
		struct curl_slist* headers, const char* url,
		CallbackFunPtr readfun, void* readerptr, long readerdatasize,
		CallbackFunPtr writefun, void* writerptr,
		SNDAECSHandleType type, SNDAECSFollowLocation followlocation, long maxredirects);


void snda_ecs_get_common_opt_attributes(const char* accesskey,
		 const char* secretkey, const char* bucketname, const char* subresource, const char* subresourceneedtosign,
		 const char* contentmd5, const char* contenttype, const char* canonicalizedheaders,
		 const char* region, int ssl, SNDAECSHandleType type,
		 struct curl_slist** headersptr, char* url);

SNDAECSErrorCode snda_ecs_common_opt(SNDAECSHandler* handler, const char* accesskey, const char* secretkey,
		const char* bucketname, const char* region, const char* subresource, const char* subresourceneedtosign, int ssl, SNDAECSByteRange* byterange,
		const SNDAECSUserObjectMeta* userobjectmeta, SNDAECSHandleType type, SNDAECSFollowLocation followlocation, long maxredirects,
		CallbackFunPtr readfun, void* inputstream, long inputlength, CallbackFunPtr writefun, void* outputstream,
		SNDAECSResult* ret);

#endif /*SNDA_ECS_HTTP_URL_H*/


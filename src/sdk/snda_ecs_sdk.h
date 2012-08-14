/*
 * ProvideBy: SNDA Cloud Computing(http://www.grandcloud.cn).
 *
 * Header file of c sdk for Elastic Cloud Storage of GrandCloud.
 *
 * This head file defines user api for Elastic Cloud Storage(ECS) of GrandCloud
 */

#ifndef SNDA_ECS_SDK_H
#define SNDA_ECS_SDK_H

#if defined(WIN32) && !defined(__cplusplus)
#define inline __inline
#endif




#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

#include <curl/curl.h>




typedef struct SNDAECSHandler {
	CURL* handler;
	// when perfom return non zero value,
	// there will be human readable error message in this buffer
	char* handlererrmsg;
} SNDAECSHandler;
/**
 * @brief this call create an SNDAECSHandler, this call MUST have a corresponding call
 *        to release_snda_ecs_handler(SNDAECSHandler*) when the operation is complete.
 * @return NULL when error happened, or return a SNDAECSHandler pointer
 */
SNDAECSHandler* snda_ecs_init_handler();
/**
 * @brief release resource of SNDAECSHandler
 */
void snda_ecs_release_handler(SNDAECSHandler* handler);


typedef struct SNDAECSHandlerError {
	char * handlererrmsg;
} SNDAECSHandlerError;
SNDAECSHandlerError* snda_ecs_init_handler_error();
void snda_ecs_release_handler_error(SNDAECSHandlerError* error);


typedef struct SNDAECSServerResponseBody {
	int retbodyremain;
	int retbodysize;
	void* retbody;
} SNDAECSServerResponseBody;
SNDAECSServerResponseBody* snda_ecs_init_server_response_body();
void snda_ecs_release_server_response_body(SNDAECSServerResponseBody* body);


typedef struct SNDAECSServerResponse {
	int httpcode;

	SNDAECSServerResponseBody* responsebody;
} SNDAECSServerResponse;
SNDAECSServerResponse* snda_ecs_init_server_response();
void snda_ecs_release_server_response(SNDAECSServerResponse* response);


typedef struct SNDAECSResult {
	SNDAECSHandlerError* error;
	SNDAECSServerResponse * serverresponse;
} SNDAECSResult;
SNDAECSResult* snda_ecs_init_result();
void snda_ecs_relase_result(SNDAECSResult* ret);


typedef struct SNDAECSErrorResponseContent {
	char* code;
	char* message;
	char* resource;
	char* requestid;
	char* fullbody;
} SNDAECSErrorResponseContent;
SNDAECSErrorResponseContent* snda_ecs_init_error_response_content();
void snda_ecs_release_error_response_content(SNDAECSErrorResponseContent* content);


typedef struct SNDAECSGetServiceResultContent {
     char* bucketname;
     char* creationtime;
     struct SNDAECSGetServiceResultContent *next;
} SNDAECSGetServiceResultContent;
SNDAECSGetServiceResultContent* snda_ecs_init_get_service_result_content();
void snda_ecs_release_get_service_result_content(SNDAECSGetServiceResultContent* content);


typedef struct SNDAECSBucketLocation {
	char* location;
} SNDAECSBucketLocation;
SNDAECSBucketLocation* snda_ecs_init_bucket_location();
void snda_ecs_release_bucket_location(SNDAECSBucketLocation* bucketlocation);


typedef struct SNDAECSObjectContent {
	char* objectname;
	char* lastmodified;
	char* etag;
	long size;

	struct SNDAECSObjectContent* next;
} SNDAECSObjectContent;
SNDAECSObjectContent* snda_ecs_init_object_content();
void snda_ecs_release_object_content(SNDAECSObjectContent* content);


typedef struct SNDAECSCommonPrefix {
	char* commonprefix;
	struct SNDAECSCommonPrefix* next;
} SNDAECSCommonPrefix;
SNDAECSCommonPrefix* snda_ecs_init_common_prefix();
void snda_ecs_release_common_prefix(SNDAECSCommonPrefix* commonprefix);


typedef struct SNDAECSGetBucketResultContent {
	char* bucketname;
	char* delimiter;
	char* prefix;
	char* marker;
	char* nextmarker;
	int maxkeys;
	int istruncated;

	SNDAECSObjectContent* objects;
	SNDAECSCommonPrefix* commonprefixes;
} SNDAECSGetBucketResultContent;
SNDAECSGetBucketResultContent* snda_ecs_init_get_bucket_result_content();
void snda_ecs_release_get_bucket_result_content(SNDAECSGetBucketResultContent* content);


typedef struct SNDAECSPair {
	char* key;
	char* value;
} SNDAECSPair;
SNDAECSPair* snda_ecs_init_pair();
void snda_ecs_release_pair(SNDAECSPair* pair);


typedef struct SNDAECSKVList {
	char* key;
	char* value;
	struct SNDAECSKVList * next;
} SNDAECSKVList;
SNDAECSKVList* snda_ecs_init_k_v_list();
void snda_ecs_release_k_v_list(SNDAECSKVList* kvlist);


typedef struct SNDAECSUserObjectMeta {
	char* contentmd5;
	char* contenttype;

	SNDAECSKVList * usermetas;
} SNDAECSUserObjectMeta;
SNDAECSUserObjectMeta* snda_ecs_init_user_object_meta();
void snda_ecs_release_user_object_meta(SNDAECSUserObjectMeta * userobjectmeta);


typedef struct SNDAECSStringList {
	char* string;
	struct SNDAECSStringList * next;
} SNDAECSStringList;
SNDAECSStringList* snda_ecs_init_string_list();
void snda_ecs_release_string_list(SNDAECSStringList* stringlist);


/**
 * Used in head object to downloads the specified range bytes of an object.
 * For more information about the HTTP Range header, go to
 * http://www.w3.org/Protocols/rfc2616/rfc2616-sec14.html#sec14.35.
 */
typedef struct SNDAECSByteRange {
	long first;
	long last;
} SNDAECSByteRange;
SNDAECSByteRange* snda_ecs_init_byte_range();
void snda_ecs_release_byte_range(SNDAECSByteRange* byterange);


typedef struct SNDAECSObjectMeta {
	char* etag;
	char* lastmodified;
	char* contenttype;
	long contentlength;

	SNDAECSKVList * usermetas;
} SNDAECSObjectMeta;
SNDAECSObjectMeta* snda_ecs_init_object_meta();
void snda_ecs_release_object_meta(SNDAECSObjectMeta* objectmeta);


typedef struct SNDAECSInitiateMultipartUploadResult {
	char* bucket;
	char* key;
	char* uploadid;
} SNDAECSInitiateMultipartUploadResult;
SNDAECSInitiateMultipartUploadResult* snda_ecs_init_initiate_multipart_upload_result();
void snda_ecs_release_initiate_multipart_upload_result(SNDAECSInitiateMultipartUploadResult* ret);


typedef struct SNDAECSMultipartsMeta {
	int partnumber;
	char* etag;
	struct SNDAECSMultipartsMeta* next;
} SNDAECSMultipartsMeta;
SNDAECSMultipartsMeta* snda_ecs_init_multiparts_meta();
void snda_ecs_release_multiparts_meta(SNDAECSMultipartsMeta* partsmeta);


typedef struct SNDAECSMultipartsPart {
	int partnumber;
	long size;
	char* lastmodified;
	char* etag;

	struct SNDAECSMultipartsPart* next;
} SNDAECSMultipartsPart;
SNDAECSMultipartsPart* snda_ecs_init_multiparts_part();
void snda_ecs_release_multiparts_part(SNDAECSMultipartsPart* part);


typedef struct SNDAECSMultipartsContent {
	char* bucket;
	char* key;
	char* uploadid;
	int maxparts;
	int istruncated;
	int partnumbermarker;
	int nextpartnumbermarker;

	SNDAECSMultipartsPart* parts;
} SNDAECSMultipartsContent;
SNDAECSMultipartsContent* snda_ecs_init_multiparts_content();
void snda_ecs_release_multiparts_content(SNDAECSMultipartsContent* content);


typedef struct SNDAECSMultipartUpload {
	char* key;
	char* uploadid;
	char* initiatedtime;
	struct SNDAECSMultipartUpload* next;
} SNDAECSMultipartUpload;
SNDAECSMultipartUpload* snda_ecs_init_multipart_upload();
void snda_ecs_release_multipart_upload(SNDAECSMultipartUpload* upload);


typedef struct SNDAECSMultipartUploadsContent {
	char* bucket;
	char* delimiter;
	char* prefix;
	char* keymarker;
	char* uploadidmarker;
	char* nextkeymarker;
	char* nextuploadidmarker;
	int istruncated;
	int maxuploads;
	SNDAECSMultipartUpload* upload;
	SNDAECSCommonPrefix* commonprefixes;
} SNDAECSMultipartUploadsContent;
SNDAECSMultipartUploadsContent* snda_ecs_init_multipart_uploads_content();
void snda_ecs_release_multipart_uploads_content(SNDAECSMultipartUploadsContent* content);


/**
 * This function gets called many times with consuming size*number bytes each time
 * @brief user can define their callback function to get or put there function
 * @param ptr pointer where retrieve data from when get or where write data to when put
 * @param stream pointer to user stream
 * @param size * number data size need to consumed each time
 */
typedef size_t (*CallbackFunPtr)(void *ptr, size_t size, size_t number, void *stream);

/**
 * @brief user must invoke this method to setup the environment
 *       which snda ecs c sdk required before invoke any other api.
 */
void snda_ecs_global_init();

/**
 * @brief user must invoke this method to cleanup the environment
 *       which snda ecs c sdk required when finished invoking any other api.
 */
void snda_ecs_global_uninit();

int snda_ecs_is_result_with_content(SNDAECSResult* result);

/**
 * @brief clear all data in ret, user must invoke this call before reuse SNDAECSResult
 * @return a pointer to ret
 */
SNDAECSResult* snda_ecs_reset_result(SNDAECSResult* ret);

/* ================================================================================================== *
 * ============================================= SNDA =============================================== *
 * ================================= GrandCloud Elastic CloudStorage ================================ *
 * ==================================== Error Response Retriever ==================================== *
 * ================================================================================================== */
SNDAECSErrorResponseContent* snda_ecs_to_error_response(SNDAECSResult* ret);


/**
 * TODO improvement
 */
typedef enum SNDA_ECS_ERROR_CODE
{
	SNDA_ECS_ERROR = -1,
    SNDA_ECS_SUCCESS,

    SNDA_ECS_CURL_GLOBAL_INIT_FAIL,

    SNDA_ECS_BAD_MULTIPARTS_METAS,

    /**
     * client unknown error
     */
    SNDA_ECS_HANDLER_ERROR = 0x2000,
} SNDAECSErrorCode;

typedef enum SNDAECSFollowLocation {
	SNDA_ECS_NOT_FOLLOW_LOCATION,
	SNDA_ECS_FOLLOW_LOCATION,
} SNDAECSFollowLocation;

/* ================================================================================================== *
 * ============================================= SNDA =============================================== *
 * ================================= GrandCloud Elastic CloudStorage ================================ *
 * ========================================== service api =========================================== *
 * ================================================================================================== */
/**
 * @input handler snda ecs handler, user should invoke snda_ecs_init_handler() to get a handler,
 *                and invoke snda_ecs_release_handler(SNDAECSHandler*) to release resource, a handler
 *                is reusable, but can only be reused among single thread.
 * @input accesskey
 * @input secretkey
 * @input ssl     whether to use https
 * @output ret    SNDAECSResult* created from snda_ecs_init_result(),
 * 		          if you want to reuse this pointer, MAKE SURE invoke snda_ecs_reset_result(SNDAECSResult*)
 *                to reset this pointer to initial status.
 * @return SNDAECSErrorCode
 * 		    return SNDA_ECS_SUCCESS when get response from snda ecs server,
 *          others mean there are something unexpected happened.
 *          when return SNDA_ECS_SUCCESS, user can get snda ecs server response code from
 *          ${ret->serverresponse->httpcode}, if it is not 20x, user can get detail error response
 *          SNDAECSErrorResponseContent by invoke snda_ecs_to_error_response(SNDAECSResult*),
 *          else if there should be value in the response, user can invoke
 *          snda_ecs_to_get_service_result(SNDAECSResult*) to get SNDAECSGetServiceResultContent* value structure.
 *          when return is not SNDA_ECS_SUCCESS, that means there must be something unexpected happened,
 *          user can get error message from ${ret->error->handlererrmsg} to see what the error message is.
 */

/**
 * get service
 * @param SNDAECSHandler* handler, the handler you had initialized by invoking snda_ecs_init_handler()
 * @param const char* accesskey,your accessKey
 * @param const char* secretkey,your secretKey
 * @param int ssl,whether to use https
 * @param SNDAECSResult* ret,SNDAECSResult* created from snda_ecs_init_result(), if you want to reuse this pointer, MAKE SURE invoke snda_ecs_reset_result(SNDAECSResult*)
 *                to reset this pointer to initial status.
 * return SNDAECSErrorCode
 */
SNDAECSErrorCode snda_ecs_get_service(SNDAECSHandler* handler, const char* accesskey,
		const char* secretkey, int ssl, SNDAECSResult* ret);
/**
 * @input ret
 *         a pointer to SNDAECSResult return from invoking of snda_ecs_get_service(..)
 * @return SNDAECSGetServiceResultContent
 *         a pointer to get service result content structure,
 * 		   user MUST remember to release related resources by invoking
 *         snda_ecs_release_get_service_result_content(SNDAECSGetServiceResultContent*)
 */

SNDAECSGetServiceResultContent* snda_ecs_to_get_service_result(SNDAECSResult* ret);

/* ================================================================================================== *
 * ============================================= SNDA =============================================== *
 * ================================= GrandCloud Elastic CloudStorage ================================ *
 * =========================================== bucket api =========================================== *
 * ================================================================================================== */
/**
 * @input handler
 * @input accesskey
 * @input secretkey
 * @input bucketname
 * @input region currently support "huadong-1", "huabei-1"
 * @input ssl
 * @output ret
 * @return SNDAECSErrorCode SNDA_ECS_SUCCESS means we get response from snda ecs server.
 *                          user should check http response code by ${ret->serverresponse->httpcode},
 *                          if it is 20x, that means put bucket successfully,
 *                          else use can check detail error response by invoke
 *                          snda_ecs_to_error_response(SNDAECSResult*);
 */

/**
 * put bucket
 * @param SNDAECSHandler* handler, the handler you had initialized by invoking snda_ecs_init_handler()
 * @param const char* accesskey,your accessKey
 * @param const char* secretkey,your secretKey
 * @param const char* bucketname,your bucketname
 * @param const char* region,region of your bucket,region currently support "huadong-1", "huabei-1"
 * @param int ssl,whether to use https
 * @param SNDAECSResult* ret,SNDAECSResult* created from snda_ecs_init_result(), if you want to reuse this pointer, MAKE SURE invoke snda_ecs_reset_result(SNDAECSResult*)
 *                to reset this pointer to initial status.
 * return SNDAECSErrorCode
 */
SNDAECSErrorCode snda_ecs_put_bucket(SNDAECSHandler* handler, const char* accesskey,
		const char* secretkey, const char* bucketname, const char* region, int ssl, SNDAECSResult* ret);

/**
 * Put bucket policy
 * @param SNDAECSHandler* handler, the handler you had
 *   	   initialized by invoking snda_ecs_init_handler()
 * @param const char* accesskey,your accessKey
 * @param const char* secretkey,your secretKey
 * @param const char* bucketname,your bucketname
 * @param const char* policy,your bucket policy
 * @param int ssl,whether to use https
 * @param SNDAECSResult* ret,SNDAECSResult* created from
 *         snda_ecs_init_result(), if you want to reuse this
 *         pointer, MAKE SURE invoke snda_ecs_reset_result
 *         (SNDAECSResult*) to reset this pointer to initial status.
 * return SNDAECSErrorCode
 */
SNDAECSErrorCode snda_ecs_put_bucket_policy(SNDAECSHandler* handler, const char* accesskey,
		const char* secretkey, const char* bucketname, const char* policy, int ssl, SNDAECSResult* ret);

/**
 * Get bucket policy
 * @param SNDAECSHandler* handler, the handler you had
 *   	   initialized by invoking snda_ecs_init_handler()
 * @param const char* accesskey,your accessKey
 * @param const char* secretkey,your secretKey
 * @param const char* bucketname,your bucketname
 * @param int ssl,whether to use https
 * @param SNDAECSResult* ret,SNDAECSResult* created from
 *         snda_ecs_init_result(), if you want to reuse this
 *         pointer, MAKE SURE invoke snda_ecs_reset_result
 *         (SNDAECSResult*) to reset this pointer to initial status.
 * return SNDAECSErrorCode
 */
SNDAECSErrorCode snda_ecs_get_bucket_policy(SNDAECSHandler* handler, const char* accesskey,
		const char* secretkey, const char* bucketname, int ssl, SNDAECSResult* ret);

/**
 * Delete bucket policy
 * @param SNDAECSHandler* handler, the handler you had
 *   	   initialized by invoking snda_ecs_init_handler()
 * @param const char* accesskey,your accessKey
 * @param const char* secretkey,your secretKey
 * @param const char* bucketname,your bucketname
 * @param int ssl,whether to use https
 * @param SNDAECSResult* ret,SNDAECSResult* created from
 *         snda_ecs_init_result(), if you want to reuse this
 *         pointer, MAKE SURE invoke snda_ecs_reset_result
 *         (SNDAECSResult*) to reset this pointer to initial status.
 * return SNDAECSErrorCode
 */
SNDAECSErrorCode snda_ecs_delete_bucket_policy(SNDAECSHandler* handler, const char* accesskey,
		const char* secretkey, const char* bucketname, int ssl, SNDAECSResult* ret);

/**
 * Get bucket location
 * @param SNDAECSHandler* handler, the handler you had
 *   	   initialized by invoking snda_ecs_init_handler()
 * @param const char* accesskey,your accessKey
 * @param const char* secretkey,your secretKey
 * @param const char* bucketname,your bucketname
 * @param int ssl,whether to use https
 * @param SNDAECSResult* ret,SNDAECSResult* created from
 *         snda_ecs_init_result(), if you want to reuse this
 *         pointer, MAKE SURE invoke snda_ecs_reset_result
 *         (SNDAECSResult*) to reset this pointer to initial status.
 * return SNDAECSErrorCode
 */
SNDAECSErrorCode snda_ecs_get_bucket_location(SNDAECSHandler* handler, const char* accesskey,
		const char* secretkey, const char* bucketname, int ssl, SNDAECSResult* ret);

SNDAECSBucketLocation* snda_ecs_to_bucket_location(SNDAECSResult* ret);

/**
 * Delete bucket
 * @param SNDAECSHandler* handler, the handler you had
 *   	   initialized by invoking snda_ecs_init_handler()
 * @param const char* accesskey,your accessKey
 * @param const char* secretkey,your secretKey
 * @param const char* bucketname,your bucketname
 * @param const char* region,region of your bucket,region
 *         currently support "huadong-1", "huabei-1"
 * @param int ssl,whether to use https
 * @param SNDAECSFollowLocation followlocation,whether to
 *         follow any "Location: " header that the server
 *         sends as part of the HTTP header
 * @param long maxredirects,the maximum amount of HTTP
 *         redirections to follow. Use this option alongside
 *         followlocation.
 * @param SNDAECSResult* ret,SNDAECSResult* created from
 *         snda_ecs_init_result(), if you want to reuse this
 *         pointer, MAKE SURE invoke snda_ecs_reset_result
 *         (SNDAECSResult*) to reset this pointer to initial status.
 * return SNDAECSErrorCode
 */
SNDAECSErrorCode snda_ecs_delete_bucket(SNDAECSHandler* handler, const char* accesskey,
		const char* secretkey, const char* bucketname, const char* region, int ssl,
		SNDAECSFollowLocation followlocation, long maxredirects,
		SNDAECSResult* ret);

/**
 * Get bucket
 * @param SNDAECSHandler* handler, the handler you had
 *   	  initialized by invoking snda_ecs_init_handler()
 * @param const char* accesskey,your accessKey
 * @param const char* secretkey,your secretKey
 * @param const char* bucketname,your bucketname
 * @param const char* prefix,the prefix parameter to the
 *   	  key of the object you want to retrieve
 * @param const char* marker,the key to start with
 * @param const char* delimiter,the character your use to
 * 		  group keys
 * @param int maxkeys,the maximum number of keys returned
 *        in the response body
 * @param const char* region,region of your bucket,region
 *        currently support "huadong-1", "huabei-1"
 * @param int ssl,whether to use https
 * @param SNDAECSFollowLocation followlocation,whether to
 *        follow any "Location: " header that the server
 *        sends as part of the HTTP header
 * @param long maxredirects,the maximum amount of HTTP
 *        redirections to follow. Use this option alongside
 *        followlocation.
 * @param SNDAECSResult* ret,SNDAECSResult* created from
 *        snda_ecs_init_result(), if you want to reuse this
 *        pointer, MAKE SURE invoke snda_ecs_reset_result
 *        (SNDAECSResult*) to reset this pointer to initial status.
 * return SNDAECSErrorCode
 */
SNDAECSErrorCode snda_ecs_get_bucket(SNDAECSHandler* handler,
		const char* accesskey, const char* secretkey, const char* bucketname,
		const char* prefix, const char* marker, const char* delimiter,
		int maxkeys, const char* region, int ssl,
		SNDAECSFollowLocation followlocation, long maxredirects,
		SNDAECSResult* ret);

SNDAECSGetBucketResultContent* snda_ecs_to_get_bucket_result(SNDAECSResult* ret);

/* ================================================================================================== *
 * ============================================= SNDA =============================================== *
 * ================================= GrandCloud Elastic CloudStorage ================================ *
 * =========================================== object api =========================================== *
 * ================================================================================================== */
static inline void snda_ecs_free_char_ptr(char* pointer) {
	if (pointer) {
		free(pointer);
		pointer = NULL;
	}
}

static inline void snda_ecs_copy_string(char** str1, const void* str2) {
	if (str2) {
		snda_ecs_free_char_ptr(*str1);
		*str1 = calloc(strlen((char*)str2) + 2, sizeof(char));
		memcpy(*str1, str2, strlen((char*)str2)+1);
		//str1[strlen((char*)str2)] = '\0';
	}
}

static inline void snda_ecs_set_object_md5(SNDAECSUserObjectMeta* const objectmeta, const char* contentmd5) {
	snda_ecs_copy_string(&(objectmeta->contentmd5), contentmd5);
}
static inline void snda_ecs_set_object_type(SNDAECSUserObjectMeta* const objectmeta, const char* contenttype) {
	snda_ecs_copy_string(&(objectmeta->contenttype), contenttype);
}

void snda_ecs_add_object_user_metas(SNDAECSUserObjectMeta* const objectmeta, const char* key, const char* value);

/**
 * Put Object
 * @param SNDAECSHandler* handler, the handler you had
 *   	   initialized by invoking snda_ecs_init_handler()
 * @param const char* accesskey,your accessKey
 * @param const char* secretkey,your secretKey
 * @param const char* bucketname,your bucketname
 * @param const char* objectname,your object name
 * @param CallbackFunPtr readFun, used as CURLOPT_READDATA,usually is snda_ecs_put_object_body
 * @param void* inputstream, data stream for upload ,usually a pointer of file opened with "rb"
 * @param long contentlength,the size of the object,in bytes
 * @param const SNDAECSUserObjectMeta* userobjectmeta,used in request headers
 * @param const char* region,region of your bucket,region
 *        currently support "huadong-1", "huabei-1"
 * @param int ssl,whether to use https
 * @param SNDAECSResult* ret,SNDAECSResult* created from
 *         snda_ecs_init_result(), if you want to reuse this
 *         pointer, MAKE SURE invoke snda_ecs_reset_result
 *         (SNDAECSResult*) to reset this pointer to initial status.
 * return SNDAECSErrorCode
 */
SNDAECSErrorCode snda_ecs_put_object(SNDAECSHandler* handler, const char* accesskey,
		const char* secretkey, const char* bucketname, const char* objectname,
		CallbackFunPtr readFun, void* inputstream, long contentlength,
		const SNDAECSUserObjectMeta* userobjectmeta, const char* region, int ssl,
		SNDAECSResult* ret);

/**
 * Delete Object
 * @param SNDAECSHandler* handler, the handler you had
 *   	   initialized by invoking snda_ecs_init_handler()
 * @param const char* accesskey,your accessKey
 * @param const char* secretkey,your secretKey
 * @param const char* bucketname,your bucketname
 * @param const char* objectname,your object name
 * @param const char* region,region of your bucket,region
 *        currently support "huadong-1", "huabei-1"
 * @param int ssl,whether to use https
 * @param SNDAECSFollowLocation followlocation,whether to
 *         follow any "Location: " header that the server
 *         sends as part of the HTTP header
 * @param long maxredirects,the maximum amount of HTTP
 *         redirections to follow. Use this option alongside
 *         followlocation.
 * @param SNDAECSResult* ret,SNDAECSResult* created from
 *         snda_ecs_init_result(), if you want to reuse this
 *         pointer, MAKE SURE invoke snda_ecs_reset_result
 *         (SNDAECSResult*) to reset this pointer to initial status.
 * return SNDAECSErrorCode
 */
SNDAECSErrorCode snda_ecs_delete_object(SNDAECSHandler* handler, const char* accesskey,
		const char* secretkey, const char* bucketname, const char* objectname, const char* region, int ssl,
		SNDAECSFollowLocation followlocation, long maxredirects,
		SNDAECSResult* ret);


/**
 * Get Object
 * @param SNDAECSHandler* handler, the handler you had
 *   	   initialized by invoking snda_ecs_init_handler()
 * @param const char* accesskey,your accessKey
 * @param const char* secretkey,your secretKey
 * @param const char* bucketname,your bucketname
 * @param const char* objectname,your object name
 * @param SNDAECSByteRange* byterange,the specified range bytes of the object.
 * @param CallbackFunPtr writeFun,used as CURLOPT_READFUNCTION,usually is snda_ecs_write_fun();
 * @param  void* outputstream, usually a pointer of file opend with "wb";
 * @param const char* region,region of your bucket,region
 *        currently support "huadong-1", "huabei-1"
 * @param int ssl,whether to use https
 * @param SNDAECSFollowLocation followlocation,whether to
 *         follow any "Location: " header that the server
 *         sends as part of the HTTP header
 * @param long maxredirects,the maximum amount of HTTP
 *         redirections to follow. Use this option alongside
 *         followlocation.
 * @param SNDAECSResult* ret,SNDAECSResult* created from
 *         snda_ecs_init_result(), if you want to reuse this
 *         pointer, MAKE SURE invoke snda_ecs_reset_result
 *         (SNDAECSResult*) to reset this pointer to initial status.
 * return SNDAECSErrorCode
 */
SNDAECSErrorCode snda_ecs_get_object(SNDAECSHandler* handler, const char* accesskey,
		const char* secretkey, const char* bucketname, const char* objectname, SNDAECSByteRange* byterange,
		CallbackFunPtr writeFun, void* outputstream, const char* region, int ssl,
		SNDAECSFollowLocation followlocation, long maxredirects,
		SNDAECSResult* ret);

/**
 * Head Object
 * @param SNDAECSHandler* handler, the handler you had
 *   	   initialized by invoking snda_ecs_init_handler()
 * @param const char* accesskey,your accessKey
 * @param const char* secretkey,your secretKey
 * @param const char* bucketname,your bucketname
 * @param const char* objectname,your object name
 * @param SNDAECSByteRange* byterange,the specified range bytes of the object.
 * @param const char* region,region of your bucket,region
 *        currently support "huadong-1", "huabei-1"
 * @param int ssl,whether to use https
 * @param SNDAECSFollowLocation followlocation,whether to
 *         follow any "Location: " header that the server
 *         sends as part of the HTTP header
 * @param long maxredirects,the maximum amount of HTTP
 *         redirections to follow. Use this option alongside
 *         followlocation.
 * @param SNDAECSResult* ret,SNDAECSResult* created from
 *         snda_ecs_init_result(), if you want to reuse this
 *         pointer, MAKE SURE invoke snda_ecs_reset_result
 *         (SNDAECSResult*) to reset this pointer to initial status.
 * return SNDAECSErrorCode
 */
SNDAECSErrorCode snda_ecs_head_object(SNDAECSHandler* handler, const char* accesskey,
		const char* secretkey, const char* bucketname, const char* objectname, SNDAECSByteRange* byterange,
		const char* region, int ssl, SNDAECSFollowLocation followlocation, long maxredirects,
		SNDAECSResult* ret);

SNDAECSObjectMeta* snda_ecs_to_object_meta(SNDAECSResult* ret);

/**
 * Copy Object
 * @param SNDAECSHandler* handler, the handler you had
 *   	   initialized by invoking snda_ecs_init_handler()
 * @param const char* accesskey,your accessKey
 * @param const char* secretkey,your secretKey
 * @param const char* destbucketname,the name of the destination bucket
 * @param const char* destobjectname,the key of the destination object
 * @param const char * region,your destination bucket's region
 * @param const char* srcbucketname,the name of the source bucket
 * @param const char* srcobjectname,the key of the source object
 * @param const SNDAECSUserObjectMeta* userobjectmeta,used in request headers
 * @param int ssl,whether to use https
 * @param SNDAECSResult* ret,SNDAECSResult* created from
 *         snda_ecs_init_result(), if you want to reuse this
 *         pointer, MAKE SURE invoke snda_ecs_reset_result
 *         (SNDAECSResult*) to reset this pointer to initial status.
 * return SNDAECSErrorCode
 */
SNDAECSErrorCode snda_ecs_copy_object(SNDAECSHandler* handler, const char* accesskey,
					 const char* secretkey, const char* destbucketname,
					 const char* destobjectname,const char * region,const char *srcbucketname,
					 const char * srcobjectname,const SNDAECSUserObjectMeta* userobjectmeta,
					 int ssl,SNDAECSResult* ret);
/* ================================================================================================== *
 * ============================================= SNDA =============================================== *
 * ================================= GrandCloud Elastic CloudStorage ================================ *
 * ==================================== multipart upload api ======================================== *
 * ================================================================================================== */

/**
 * Initiate Multipart upload
 * @param SNDAECSHandler* handler, the handler you had
 *   	   initialized by invoking snda_ecs_init_handler()
 * @param const char* accesskey,your accessKey
 * @param const char* secretkey,your secretKey
 * @param const char* bucketname,your bucketname
 * @param const char* objectname,your object name
 * @param const char* region,region of your bucket,region
 *         currently support "huadong-1", "huabei-1"
 * @param const SNDAECSUserObjectMeta* userobjectmeta,used
 * 		   in request headers
 * @param int ssl,whether to use https
 * @param SNDAECSFollowLocation followlocation,whether to
 *         follow any "Location: " header that the server
 *         sends as part of the HTTP header
 * @param long maxredirects,the maximum amount of HTTP
 *         redirections to follow. Use this option alongside
 *         followlocation.
 * @param SNDAECSResult* ret,SNDAECSResult* created from
 *         snda_ecs_init_result(), if you want to reuse this
 *         pointer, MAKE SURE invoke snda_ecs_reset_result
 *         (SNDAECSResult*) to reset this pointer to initial status.
 * return SNDAECSErrorCode
 */
SNDAECSErrorCode snda_ecs_initiate_multipart_upload(SNDAECSHandler* handler, const char* accesskey,
		const char* secretkey, const char* bucketname, const char* objectname,
		const SNDAECSUserObjectMeta* userobjectmeta, const char* region, int ssl,
		SNDAECSFollowLocation followlocation, long maxredirects,
		SNDAECSResult* ret);
SNDAECSInitiateMultipartUploadResult* snda_ecs_to_initiate_multipart_upload_result(SNDAECSResult* ret);

/**
 * List_multipart_uploads
 * @param SNDAECSHandler* handler, the handler you had
 *   	  initialized by invoking snda_ecs_init_handler()
 * @param const char* accesskey,your accessKey
 * @param const char* secretkey,your secretKey
 * @param const char* bucketname,your bucketname
 * @param const char* prefix,the prefix parameter to the
 *   	  key of the multipart upload you want to retrieve
 * @param const char* keymarker,the key to start with
 * @param const char* uploadidmarker,the uploadid to start with
 * @param const char* delimiter,the character your use to
 * 		  group keys
 * @param int maxuploads,the maximum number of keys returned
 *        in the response body
 * @param const char* region,region of your bucket,region
 *        currently support "huadong-1", "huabei-1"
 * @param int ssl,whether to use https
 * @param SNDAECSFollowLocation followlocation,whether to
 *        follow any "Location: " header that the server
 *        sends as part of the HTTP header
 * @param long maxredirects,the maximum amount of HTTP
 *        redirections to follow. Use this option alongside
 *        followlocation.
 * @param SNDAECSResult* ret,SNDAECSResult* created from
 *        snda_ecs_init_result(), if you want to reuse this
 *        pointer, MAKE SURE invoke snda_ecs_reset_result
 *        (SNDAECSResult*) to reset this pointer to initial status.
 * return SNDAECSErrorCode
 */
SNDAECSErrorCode snda_ecs_list_multipart_uploads(SNDAECSHandler* handler, const char* accesskey, const char* secretkey,
		const char* bucketname, const char* prefix, const char* keymarker, const char* uploadidmarker,
		const char* delimiter, int maxuploads, const char* region, int ssl,
		SNDAECSFollowLocation followlocation, long maxredirects,
		SNDAECSResult* ret);
SNDAECSMultipartUploadsContent* snda_ecs_to_multipart_uploads_content(SNDAECSResult* ret);


/**
 * Abort_multipart_uploads
 * @param SNDAECSHandler* handler, the handler you had
 *   	  initialized by invoking snda_ecs_init_handler()
 * @param const char* accesskey,your accessKey
 * @param const char* secretkey,your secretKey
 * @param const char* bucketname,your bucketname
 * @param const char* objectname,your object name
 * @param const char* uploadid,your uploadid for
 *  	   the multipart upload
 * @param const char* region,region of your bucket,region
 *        currently support "huadong-1", "huabei-1"
 * @param int ssl,whether to use https
 * @param SNDAECSFollowLocation followlocation,whether to
 *        follow any "Location: " header that the server
 *        sends as part of the HTTP header
 * @param long maxredirects,the maximum amount of HTTP
 *        redirections to follow. Use this option alongside
 *        followlocation.
 * @param SNDAECSResult* ret,SNDAECSResult* created from
 *        snda_ecs_init_result(), if you want to reuse this
 *        pointer, MAKE SURE invoke snda_ecs_reset_result
 *        (SNDAECSResult*) to reset this pointer to initial status.
 * return SNDAECSErrorCode
 */
SNDAECSErrorCode snda_ecs_abort_multipart_upload(SNDAECSHandler* handler, const char* accesskey,
		const char* secretkey, const char* bucketname, const char* objectname,
		const char* uploadid, const char* region, int ssl,
		SNDAECSFollowLocation followlocation, long maxredirects,
		SNDAECSResult* ret);

/**
 * Upload Part
 * @param SNDAECSHandler* handler, the handler you had
 *   	   initialized by invoking snda_ecs_init_handler()
 * @param const char* accesskey,your accessKey
 * @param const char* secretkey,your secretKey
 * @param const char* bucketname,your bucketname
 * @param const char* objectname,your object name
 * @param const char* uploadid,your uploadid for multipart upload
 * @param int partnumber,partnumber of this part
 * @param CallbackFunPtr readFun, used as CURLOPT_READDATA,usually is snda_ecs_put_object_body
 * @param void* inputstream, data stream for upload ,usually a pointer of file opened with "rb"
 * @param long contentlength,the size of the object,in bytes
 * @param const char* contentmd5,contentmd5 of this part(can be null)
 * @param const char* region,region of your bucket,region
 *        currently support "huadong-1", "huabei-1"
 * @param int ssl,whether to use https
 * @param SNDAECSResult* ret,SNDAECSResult* created from
 *         snda_ecs_init_result(), if you want to reuse this
 *         pointer, MAKE SURE invoke snda_ecs_reset_result
 *         (SNDAECSResult*) to reset this pointer to initial status.
 * return SNDAECSErrorCode
 */
SNDAECSErrorCode snda_ecs_upload_part(SNDAECSHandler* handler, const char* accesskey,
		const char* secretkey, const char* bucketname, const char* objectname,
		const char* uploadid, int partnumber, CallbackFunPtr readFun, void* inputstream,
		long contentlength, const char* contentmd5, const char* region, int ssl,
		SNDAECSResult* ret);
/**
 * @output etag user allocated buffer to hold this part's etag like "\"46bfaf06368c5b456342fa57332cee98\""
 */
char* snda_ecs_to_upload_part_etag(char* etag, SNDAECSResult* ret);


/**
 * List_Parts
 * @param SNDAECSHandler* handler, the handler you had
 *   	  initialized by invoking snda_ecs_init_handler()
 * @param const char* accesskey,your accessKey
 * @param const char* secretkey,your secretKey
 * @param const char* bucketname,your bucketname
 * @param const char* objectname,the object name of the
 *         multipart upload
 * @param const char* uploadid,the uploadid of the multipart
 *  	   upload
 * @param const char* partnumbermarker,the part to start with
 * @param int maxparts,the maximum number of parts returned
 *        in the response body
 * @param const char* region,region of your bucket,region
 *        currently support "huadong-1", "huabei-1"
 * @param int ssl,whether to use https
 * @param SNDAECSFollowLocation followlocation,whether to
 *        follow any "Location: " header that the server
 *        sends as part of the HTTP header
 * @param long maxredirects,the maximum amount of HTTP
 *        redirections to follow. Use this option alongside
 *        followlocation.
 * @param SNDAECSResult* ret,SNDAECSResult* created from
 *        snda_ecs_init_result(), if you want to reuse this
 *        pointer, MAKE SURE invoke snda_ecs_reset_result
 *        (SNDAECSResult*) to reset this pointer to initial status.
 * return SNDAECSErrorCode
 */
SNDAECSErrorCode snda_ecs_list_parts(SNDAECSHandler* handler, const char* accesskey, const char* secretkey,
		const char* bucketname, const char* objectname, const char* uploadid,
		int partnumbermarker, int maxparts, const char* region, int ssl,
		SNDAECSFollowLocation followlocation, long maxredirects,
		SNDAECSResult* ret);
SNDAECSMultipartsContent* snda_ecs_to_multipart_parts(SNDAECSResult* ret);

/**
 * Complete multipart upload
 * @param SNDAECSHandler* handler, the handler you had
 *   	  initialized by invoking snda_ecs_init_handler()
 * @param const char* accesskey,your accessKey
 * @param const char* secretkey,your secretKey
 * @param const char* bucketname,your bucketname
 * @param const char* objectname,the object name of the
 *         multipart upload
 * @param const char* uploadid,the uploadid of the multipart
 *  	   upload
 * @param const SNDAECSMultipartsMeta* partsmeta,partsmetas of
 *         the multipart upload
 * @param const char* region,region of your bucket,region
 *        currently support "huadong-1", "huabei-1"
 * @param int ssl,whether to use https
 * @param SNDAECSFollowLocation followlocation,whether to
 *        follow any "Location: " header that the server
 *        sends as part of the HTTP header
 * @param long maxredirects,the maximum amount of HTTP
 *        redirections to follow. Use this option alongside
 *        followlocation.
 * @param SNDAECSResult* ret,SNDAECSResult* created from
 *        snda_ecs_init_result(), if you want to reuse this
 *        pointer, MAKE SURE invoke snda_ecs_reset_result
 *        (SNDAECSResult*) to reset this pointer to initial status.
 * return SNDAECSErrorCode
 */
SNDAECSErrorCode snda_ecs_complete_multipart_upload(SNDAECSHandler* handler, const char* accesskey,
		const char* secretkey, const char* bucketname, const char* objectname,
		const char* uploadid, const SNDAECSMultipartsMeta* partsmeta, const char* region, int ssl,
		SNDAECSFollowLocation followlocation, long maxredirects,
		SNDAECSResult* ret);

/**
 * Upload Part - Copy 
 * @param SNDAECSHandler* handler, the handler you had
 *   	   initialized by invoking snda_ecs_init_handler()
 * @param const char* accesskey,your accessKey
 * @param const char* secretkey,your secretKey
 * @param const char* bucketname,the name of the destination bucket
 * @param const char* objectname,the key of the destination object
 * @param const char* uploadid,your uploadid for multipart upload
 * @param int partnumber,partnumber of this part
 * @param const char * region,your destination bucket's region
 * @param const SNDAECSUserObjectMeta* userobjectmeta,used in request headers
 * @param const char* srcbucketname,the name of the source bucket
 * @param const char* srcobjectname,the key of the source object
 * @param int ssl,whether to use https
 * @param SNDAECSResult* ret,SNDAECSResult* created from
 *         snda_ecs_init_result(), if you want to reuse this
 *         pointer, MAKE SURE invoke snda_ecs_reset_result
 *         (SNDAECSResult*) to reset this pointer to initial status.
 * return SNDAECSErrorCode
 */
SNDAECSErrorCode snda_ecs_upload_part_copy(SNDAECSHandler* handler, const char* accesskey,
		const char* secretkey, const char* bucketname, const char* objectname,
		const char* uploadid, int partnumber,const char * region,const SNDAECSUserObjectMeta* userobjectmeta,
		const char* sbucket,const char* sobjectname,int ssl,SNDAECSResult* ret);

/* ================================================================================================== *
 * ============================================= SNDA =============================================== *
 * ================================= GrandCloud Elastic CloudStorage ================================ *
 * ============================================= test =============================================== *
 * ================================================================================================== */
/*
static void sdna_ecs_unit_test() {
}
*/

#endif /* SNDA_ECS_SDK_H */

/*
 * ProvideBy: SNDA Cloud Computing(http://www.grandcloud.cn).
 *
 * Implementation of c sdk for Elastic Cloud Storage of GrandCloud.
 */

#if defined(WIN32)
#define _CRT_SECURE_NO_DEPRECATE
#endif

#include <string.h>
#include <ctype.h>

#include "snda_ecs_sdk.h"
#include "snda_ecs_http_util.h"
#include "snda_ecs_constants.h"
#include "snda_ecs_common_util.h"




static SNDAECSKVList* snda_ecs_copy_k_v_list(SNDAECSKVList* clone, const SNDAECSKVList* orig) {
	if (orig) {
		snda_ecs_copy_string(&(clone->key), orig->key);
		snda_ecs_copy_string(&(clone->value), orig->value);
		if (orig->next) {
			if (clone->next) {
				snda_ecs_release_k_v_list(clone->next);
			}
			clone->next = snda_ecs_init_k_v_list();
			snda_ecs_copy_k_v_list(clone->next, orig->next);
		}
	}
	return clone;
}

static SNDAECSUserObjectMeta* snda_ecs_copy_user_object_meta(SNDAECSUserObjectMeta* clone, const SNDAECSUserObjectMeta* orig) {
	if (orig) {
		snda_ecs_copy_string(&(clone->contentmd5), orig->contentmd5);
		snda_ecs_copy_string(&(clone->contenttype), orig->contenttype);

		if (orig->usermetas) {
			if (clone->usermetas) {
				snda_ecs_release_k_v_list(clone->usermetas);
			}
			clone->usermetas = snda_ecs_init_k_v_list();
			snda_ecs_copy_k_v_list(clone->usermetas, orig->usermetas);
		}
	}
	return clone;
}

static SNDAECSErrorCode snda_ecs_put_object_inner(SNDAECSHandler* handler, const char* accesskey,
		const char* secretkey, const char* bucketname, const char* objectname,
		CallbackFunPtr readFun, void* inputstream, long contentlength,
		const SNDAECSUserObjectMeta* userobjectmeta, const char* region, int ssl,
		SNDAECSFollowLocation followlocation, long maxredirects,
		SNDAECSResult* ret)
{
    SNDAECSReadBuff buff;
    SNDAECSUserObjectMeta* innermeta = 0;
    SNDAECSErrorCode retcode = SNDA_ECS_ERROR;
	char* subresource = (char*)malloc(strlen("/") + strlen(objectname) + 1);

	sprintf(subresource, "/%s", objectname);

	buff.databuff = inputstream;
	buff.consumed = 0;
	buff.datasize = contentlength;

	innermeta = snda_ecs_init_user_object_meta();
	snda_ecs_copy_user_object_meta(innermeta, userobjectmeta);

	if (!(innermeta->contenttype)) {
		char contenttype[S_SNDA_ECS_CONTENT_TYPE_LEN];
		snda_ecs_set_object_type(innermeta, snda_ecs_get_content_type(objectname, contenttype));
	}

	retcode = snda_ecs_common_opt(handler, accesskey, secretkey,
			bucketname, region, subresource, subresource, ssl, 0, innermeta,
			SNDA_ECS_PUT, followlocation, maxredirects,
			readFun, &buff, buff.datasize,
			snda_ecs_get_server_response_body,
			ret->serverresponse->responsebody,
			ret);

	snda_ecs_release_user_object_meta(innermeta);
    snda_ecs_free_char_ptr(subresource);
	return retcode;
}

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
		SNDAECSResult* ret)
{
	return snda_ecs_put_object_inner(handler, accesskey, secretkey, bucketname, objectname,
			readFun, inputstream, contentlength, userobjectmeta, region, ssl,
			SNDA_ECS_NOT_FOLLOW_LOCATION , 0L,
			ret);
}

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
		SNDAECSResult* ret)
{
    SNDAECSErrorCode retcode = SNDA_ECS_ERROR;
	char* subresource= (char*)malloc(strlen("/") + strlen(objectname) + 1);
	sprintf(subresource, "/%s", objectname);

    retcode = snda_ecs_common_opt(handler, accesskey, secretkey,
			bucketname, region, subresource, subresource, ssl, 0, 0,
			SNDA_ECS_DELETE, followlocation, maxredirects,
			0, 0, 0,
			snda_ecs_get_server_response_body, ret->serverresponse->responsebody,
			ret);

    snda_ecs_free_char_ptr(subresource);

    return retcode;
}

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
		SNDAECSResult* ret)
{
    SNDAECSErrorCode retcode = SNDA_ECS_ERROR;
	char* subresource = (char*)malloc(strlen("/") + strlen(objectname) + 1);

	sprintf(subresource, "/%s", objectname);

	retcode = snda_ecs_common_opt(handler, accesskey, secretkey,
			bucketname, region, subresource, subresource, ssl, byterange, 0,
			SNDA_ECS_GET, followlocation, maxredirects,
			0, 0, 0,
			writeFun, outputstream,
			ret);

    snda_ecs_free_char_ptr(subresource);
    return retcode;
}


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
		SNDAECSResult* ret)
{
    SNDAECSErrorCode retcode = SNDA_ECS_ERROR;
    char* subresource = (char*)malloc(strlen("/") + strlen(objectname) + 1);
	sprintf(subresource, "/%s", objectname);

	retcode = snda_ecs_common_opt(handler, accesskey, secretkey,
			bucketname, region, subresource, subresource, ssl, byterange, 0,
			SNDA_ECS_HEAD, followlocation, maxredirects,
			0, 0, 0,
			snda_ecs_get_server_response_body, ret->serverresponse->responsebody,
			ret);

    snda_ecs_free_char_ptr(subresource);

    return retcode;
}


/**
 * Copy Object
 * @param SNDAECSHandler* handler, the handler you had
 *   	   initialized by invoking snda_ecs_init_handler()
 * @param const char* accesskey,your accessKey
 * @param const char* secretkey,your secretKey
 * @param const char* destbucketname,the name of the destination bucket
 * @param const char* destobjectname,the key of the destination object
 * @param const char* srcbucketname,the name of the source bucket
 * @param const char* srcobjectname,the key of the source object
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
SNDAECSErrorCode snda_ecs_copy_object(SNDAECSHandler* handler, const char* accesskey,
					 const char* secretkey, const char* destbucketname,
					 const char* destobjectname,const char *srcbucketname,
					 const char * srcobjectname,const SNDAECSUserObjectMeta* userobjectmeta,
					 const char* region, int ssl,
					 SNDAECSResult* ret) {
    SNDAECSErrorCode retcode = SNDA_ECS_ERROR;
    SNDAECSReadBuff buff;
    SNDAECSUserObjectMeta* innermeta = 0;
    char* src_object_info = 0;
	char* subresource = (char*)malloc(strlen("/") + strlen(destobjectname) + 1);
	sprintf(subresource, "/%s", destobjectname);

	buff.databuff = 0;
	buff.consumed = 0;
	buff.datasize = 0;

	innermeta = snda_ecs_init_user_object_meta();
	snda_ecs_copy_user_object_meta(innermeta, userobjectmeta);

	src_object_info = (char*)malloc(strlen("/") + strlen(srcbucketname) + strlen("/")+strlen(srcobjectname)+1);
	sprintf(src_object_info,"/%s/%s",srcbucketname,srcobjectname);
	snda_ecs_add_object_user_metas(innermeta, "x-SNDA-copy-source", src_object_info);
	retcode = snda_ecs_common_opt(handler, accesskey, secretkey,
			destbucketname, region, subresource, subresource, ssl, 0, innermeta,
			SNDA_ECS_PUT, SNDA_ECS_NOT_FOLLOW_LOCATION, 0L,
			snda_ecs_put_object_body, &buff, buff.datasize,
			snda_ecs_get_server_response_body,
			ret->serverresponse->responsebody,
			ret);

	snda_ecs_release_user_object_meta(innermeta);
    snda_ecs_free_char_ptr(src_object_info);
    snda_ecs_free_char_ptr(subresource);

	return retcode;

}

SNDAECSObjectMeta* snda_ecs_to_object_meta(SNDAECSResult* ret) {
	const char* snda_user_meta_prefix = "x-snda-meta-";
	SNDAECSObjectMeta* objectmeta = 0;
    SNDAECSKVList* usermetas = 0;
    const char* p = 0;
    int i = 0;
	int segmentbegin = 0;
    char* key = 0;
    char* value = 0;

    if (!snda_ecs_is_result_with_content(ret)) {
		return NULL;
	}

	objectmeta = snda_ecs_init_object_meta();
	usermetas = objectmeta->usermetas;
	p = (char*)(ret->serverresponse->responsebody->retbody);
    key = (char*)malloc(ret->serverresponse->responsebody->retbodysize + 1);
    value = (char*)malloc(ret->serverresponse->responsebody->retbodysize + 1);
	for (; i < ret->serverresponse->responsebody->retbodysize; ++i) {
		if (p[i] == '\n') {
			int segmentsize = i - segmentbegin;
			if (segmentsize > 0) {
                int n = 0;
                int j = 0;
				const char* segment = p + segmentbegin;
				// split this segment
				memset(key, 0, ret->serverresponse->responsebody->retbodysize + 1);
				for (; n < segmentsize; ++n) {
					if (segment[n] == ':') {
						break;
					}
					key[n] = segment[n];
				}
				key[n] = '\0';
				
				memset(value, 0, ret->serverresponse->responsebody->retbodysize + 1);
				for (++n; n < segmentsize; ++n) {
					if (segment[n] != ' ' && segment[n] != '\t') {
						value[j++] = segment[n];
					}
				}
				value[j] = '\0';

				if (strlen(key) && strcmp(key, "\r")) {
					if (!strcmp(key, "ETag")) {
						snda_ecs_copy_string(&(objectmeta->etag), value);
					} else if (!strcmp(key, "Content-Type")) {
						snda_ecs_copy_string(&(objectmeta->contenttype), value);
					} else if (!strcmp(key, "Content-Length")) {
						objectmeta->contentlength = atol(value);
					} else if (!strcmp(key, "Last-Modified")) {
						snda_ecs_copy_string(&(objectmeta->lastmodified), value);
					} else {
						char* lowercase = (char*)malloc(strlen(key) + 1);
						snda_ecs_to_lower_string(key, lowercase);
						if (snda_ecs_is_start_with(lowercase, snda_user_meta_prefix)) {
							if (!usermetas) {
								objectmeta->usermetas = snda_ecs_init_k_v_list();
								usermetas = objectmeta->usermetas;
							} else {
								usermetas->next = snda_ecs_init_k_v_list();
								usermetas = usermetas->next;
							}
							snda_ecs_copy_string(&(usermetas->key), key);
							snda_ecs_copy_string(&(usermetas->value), value);
						}
                        snda_ecs_free_char_ptr(lowercase);
					}
				}
			}
			segmentbegin = i + 1;
		}
	}

    snda_ecs_free_char_ptr(value);
    snda_ecs_free_char_ptr(key);
	return objectmeta;
}


void snda_ecs_add_object_user_metas(SNDAECSUserObjectMeta* const objectmeta, const char* key, const char* value)
{
	SNDAECSKVList* p = objectmeta->usermetas;
	if (!(objectmeta->usermetas)) {
		objectmeta->usermetas = snda_ecs_init_k_v_list();
		p = objectmeta->usermetas;
	} else {
		for (; p->next; p = p->next) ;
		p->next = snda_ecs_init_k_v_list();
		p = p->next;
	}

	snda_ecs_copy_string(&(p->key), key);
	snda_ecs_copy_string(&(p->value), value);
}

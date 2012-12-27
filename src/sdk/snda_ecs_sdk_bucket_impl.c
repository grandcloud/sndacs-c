/*
 * ProvideBy: SNDA Cloud Computing(http://www.grandcloud.cn).
 *
 * Implementation of c sdk for Elastic Cloud Storage of GrandCloud.
 */
#if defined(WIN32)
#define _CRT_SECURE_NO_DEPRECATE
#endif

#include <memory.h>

#include "snda_ecs_sdk.h"
#include "snda_ecs_common_util.h"
#include "snda_ecs_http_util.h"
#include "snda_ecs_constants.h"



static SNDAECSErrorCode snda_ecs_bucket_opt(SNDAECSHandler* handler, const char* accesskey, const char* secretkey,
		const char* bucketname, const char* region, const char* subresource, const char* subresourceneedtosign,
		int ssl, SNDAECSReadBuff* buff, SNDAECSHandleType type, SNDAECSFollowLocation followlocation, long maxredirects,
		SNDAECSResult* ret) {

	CallbackFunPtr readfun = 0;
	long buffsize = 0;
	if (buff) {
		readfun = snda_ecs_put_xml_body;
		buffsize = buff->datasize;
	}

	return snda_ecs_common_opt(handler, accesskey, secretkey,
			bucketname, region, subresource, subresourceneedtosign,
			ssl, 0, 0, type, followlocation, maxredirects,
			readfun, buff, buffsize,
			snda_ecs_get_server_response_body, ret->serverresponse->responsebody,
			ret);
}


SNDAECSErrorCode snda_ecs_put_bucket(SNDAECSHandler* handler, const char* accesskey,
									 const char* secretkey, const char* bucketname,
									 const char* region, int ssl,
									 SNDAECSResult* ret)
{
	const char* subresource = 0;
    char* p = 0;
	char* prefix = "<CreateBucketConfiguration><LocationConstraint>";
	char* suffix = "</LocationConstraint></CreateBucketConfiguration>";
	int xmlinputbuffsize = strlen(prefix) + strlen(region) + strlen(suffix) + 1;
    SNDAECSErrorCode retcode = SNDA_ECS_ERROR;

	SNDAECSReadBuff buff;
	buff.consumed = 0;
	buff.databuff = (char*)malloc(xmlinputbuffsize);
    p = (char*)(buff.databuff);
	sprintf(p , "%s", prefix);
	sprintf(p + strlen(p ), "%s", region);
	sprintf(p + strlen(p ), "%s", suffix);
	buff.datasize = strlen(p);

	retcode = snda_ecs_bucket_opt(handler, accesskey, secretkey,
			bucketname, region, subresource, subresource, ssl, &buff,
			SNDA_ECS_PUT, SNDA_ECS_NOT_FOLLOW_LOCATION, 0,
			ret);

	free(buff.databuff);

	return retcode;
}



SNDAECSErrorCode snda_ecs_delete_bucket_policy(SNDAECSHandler* handler, const char* accesskey,
		const char* secretkey, const char* bucketname, int ssl, SNDAECSResult* ret)
{
	const char* subresource = "?policy";
	const char* region = 0;

	return snda_ecs_bucket_opt(handler, accesskey, secretkey,
			bucketname, region, subresource, subresource, ssl, 0,
			SNDA_ECS_DELETE, SNDA_ECS_NOT_FOLLOW_LOCATION, 0,
			ret);
}

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
		const char* secretkey, const char* bucketname, int ssl, SNDAECSResult* ret)
{
	const char* subresource = "?location";
	const char* region = 0;

	return snda_ecs_bucket_opt(handler, accesskey, secretkey,
			bucketname, region, subresource, subresource, ssl, 0,
			SNDA_ECS_GET, SNDA_ECS_NOT_FOLLOW_LOCATION, 0,
			ret);
}

SNDAECSBucketLocation* snda_ecs_get_bucket_location_info(const char* accesskey,
		const char* secretkey, const char* bucketname, int ssl){
        SNDAECSHandler* handler;
        SNDAECSResult* ret;
        SNDAECSErrorCode retcode ;
        SNDAECSBucketLocation* location = 0;
        handler = snda_ecs_init_handler();
        ret = snda_ecs_init_result();
        retcode = snda_ecs_get_bucket_location(handler, accesskey,
                        secretkey, bucketname, ssl, ret);
        if (retcode != SNDA_ECS_SUCCESS) {
                printf("ClientErrorMessage:%s", ret->error->handlererrmsg);

        } if (ret->serverresponse->httpcode == 200) {
             location = snda_ecs_to_bucket_location(ret);
        } 

        snda_ecs_release_handler(handler);
        snda_ecs_release_result(ret);
        return location;
}
SNDAECSBucketLocation* snda_ecs_to_bucket_location(SNDAECSResult* ret)
{
    const char* snda_ecs_region_default = "huabei-1";
    xmlDocPtr doc = 0;
    xmlNodePtr cur  = 0;
    const char* location = 0;
    SNDAECSBucketLocation* bucketlocation  = 0;

	if (!snda_ecs_is_result_with_content(ret)) {
		return NULL;
	}

	doc = xmlParseMemory(ret->serverresponse->responsebody->retbody,
			ret->serverresponse->responsebody->retbodysize);
	if (!doc) {
		return NULL;
	}

	cur = xmlDocGetRootElement(doc);
	if (!cur) {
		xmlFreeDoc(doc);
		return NULL;
	}

	if (!snda_ecs_is_xml_element_node_with_name(cur, "LocationConstraint")) {
		xmlFreeDoc(doc);
		return NULL;
	}

	cur = cur->children;
	location = snda_ecs_region_default;
	if (cur) {
		location = (char*)(cur->content);
	}

	bucketlocation = snda_ecs_init_bucket_location();
	bucketlocation->location = (char*)malloc(strlen(location) + 1);
	sprintf(bucketlocation->location, "%s", location);

	xmlFreeDoc(doc);

	return bucketlocation;
}


SNDAECSErrorCode snda_ecs_put_bucket_policy(SNDAECSHandler* handler, const char* accesskey,
		const char* secretkey, const char* bucketname, const char* policy,
		int ssl, SNDAECSResult* ret)
{
	const char* subresource = "?policy";
	char* region = 0;
	SNDAECSBucketLocation *location;
    SNDAECSErrorCode retcode = SNDA_ECS_ERROR;
    SNDAECSReadBuff buff;
	buff.consumed = 0;

	location = snda_ecs_get_bucket_location_info(accesskey,secretkey,bucketname,ssl);
	if(location) {
		region = location->location;
	}

	buff.databuff = calloc(strlen(policy) + 1, sizeof(char));
	memcpy(buff.databuff, policy, strlen(policy));
	buff.datasize = strlen(buff.databuff);

	retcode = snda_ecs_bucket_opt(handler, accesskey, secretkey,
			bucketname, region, subresource, subresource, ssl, &buff,
			SNDA_ECS_PUT, SNDA_ECS_FOLLOW_LOCATION, 1,
			ret);

	free(buff.databuff);
     
	if(location) {
	  snda_ecs_release_bucket_location(location);
	}
	return retcode;
}



SNDAECSErrorCode snda_ecs_get_bucket_policy(SNDAECSHandler* handler, const char* accesskey,
		const char* secretkey, const char* bucketname, int ssl, SNDAECSResult* ret)
{
	const char* subresource = "?policy";
	const char* region = 0;
    
	return snda_ecs_bucket_opt(handler, accesskey, secretkey,
			bucketname, region, subresource, subresource, ssl, 0,
			SNDA_ECS_GET, SNDA_ECS_FOLLOW_LOCATION, 1,
			ret);
}


SNDAECSErrorCode snda_ecs_delete_bucket(SNDAECSHandler* handler, const char* accesskey,
		const char* secretkey, const char* bucketname, const char* region, int ssl,
		SNDAECSFollowLocation followlocation, long maxredirects,
		SNDAECSResult* ret)
{
	const char* subresource = 0;

	return snda_ecs_bucket_opt(handler, accesskey, secretkey,
			bucketname, region, subresource, subresource, ssl, 0,
			SNDA_ECS_DELETE, followlocation, maxredirects,
			ret);
}



SNDAECSErrorCode snda_ecs_get_bucket(SNDAECSHandler* handler, const char* accesskey, const char* secretkey,
		const char* bucketname, const char* prefix, const char* marker, const char* delimiter, int maxkeys,
		const char* region, int ssl, SNDAECSFollowLocation followlocation, long maxredirects,
		SNDAECSResult* ret)
{
    char maxkeysstr[24];
    int subresourcelen = 0;
    const char* prefixlabel = "&prefix=";
	const char* markerlabel = "&marker=";
	const char* delimiterlabel = "&delimiter=";
    char* subresource = 0;
    SNDAECSErrorCode retcode = SNDA_ECS_ERROR;

	maxkeys = maxkeys > 1000 ? 1000 : maxkeys;
	sprintf(maxkeysstr, "?max-keys=%d", maxkeys);
	subresourcelen = strlen(maxkeysstr) + 1;
	
	if (prefix && strlen(prefix)) {
		subresourcelen += strlen(prefixlabel) + strlen(prefix);
	}
	if (marker && strlen(marker)) {
		subresourcelen += strlen(markerlabel) + strlen(marker);
	}
	if (delimiter && strlen(delimiter)) {
		subresourcelen += strlen(delimiterlabel) + strlen(delimiter);
	}

	subresource = (char*)malloc(subresourcelen);
	sprintf(subresource, "%s", maxkeysstr);
	if (prefix && strlen(prefix)) {
		sprintf(subresource + strlen(subresource), "%s%s", prefixlabel, prefix);
	}
	if (marker && strlen(marker)) {
		sprintf(subresource + strlen(subresource), "%s%s", markerlabel, marker);
	}
	if (delimiter && strlen(delimiter)) {
		sprintf(subresource + strlen(subresource), "%s%s", delimiterlabel, delimiter);
	}

	retcode = snda_ecs_bucket_opt(handler, accesskey, secretkey,
			bucketname, region, subresource, 0, ssl, 0,
			SNDA_ECS_GET, followlocation, maxredirects,
			ret);

    snda_ecs_free_char_ptr(subresource);

    return retcode;
}

static SNDAECSObjectContent* snda_ecs_to_object_content(const xmlNodePtr contentelementnode)
{
    SNDAECSObjectContent *node = 0;
    xmlNodePtr child = 0;

	if (!snda_ecs_is_xml_element_node_with_name(contentelementnode, "Contents")) {
		return NULL;
	}

	if (!contentelementnode || !(contentelementnode->xmlChildrenNode)) {
		return NULL;
	}

	node = snda_ecs_init_object_content();
	child = contentelementnode->xmlChildrenNode;
	for (; child; child = child->next) {
		char **ptr = 0;
		if (snda_ecs_is_xml_element_node_with_name(child, "Key")) {
			ptr = &(node->objectname);
		} else if (snda_ecs_is_xml_element_node_with_name(child, "LastModified")) {
			ptr = &(node->lastmodified);
		} else if (snda_ecs_is_xml_element_node_with_name(child, "ETag")) {
			ptr = &(node->etag);
		} else if (snda_ecs_is_xml_element_node_with_name(child, "Size")) {
			if (child->children->content && strlen((char*)(child->children->content))) {
				node->size = atol((char*)(child->children->content));
			}
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


SNDAECSGetBucketResultContent* snda_ecs_to_get_bucket_result(SNDAECSResult* result)
{
    xmlDocPtr doc = 0;
    xmlNodePtr cur = 0;
    SNDAECSGetBucketResultContent* content = 0;
    SNDAECSObjectContent* currentcontentnode = 0;
    SNDAECSCommonPrefix * currentcommonprefix = 0;

	if (!snda_ecs_is_result_with_content(result)) {
		return NULL;
	}

	doc = xmlParseMemory(result->serverresponse->responsebody->retbody,
			result->serverresponse->responsebody->retbodysize);

	if (!doc) {
		return NULL;
	}
	cur = xmlDocGetRootElement(doc);
	if (!cur) {
		xmlFreeDoc(doc);
		return NULL;
	}
	if (!snda_ecs_is_xml_element_node_with_name(cur, "ListBucketResult")) {
		xmlFreeDoc(doc);
		return NULL;
	}

	content = snda_ecs_init_get_bucket_result_content();
	currentcontentnode = content->objects;
	currentcommonprefix = content->commonprefixes;
	for (cur = cur->xmlChildrenNode; cur; cur = cur->next) {
		char** ptr = 0;
		if (snda_ecs_is_xml_element_node_with_name(cur, "Name")) {
			ptr = &(content->bucketname);
		} else if (snda_ecs_is_xml_element_node_with_name(cur, "Delimiter")) {
			ptr = &(content->delimiter);
		} else if (snda_ecs_is_xml_element_node_with_name(cur, "Prefix")) {
			ptr = &(content->prefix);
		} else if (snda_ecs_is_xml_element_node_with_name(cur, "Marker")) {
			ptr = &(content->marker);
		} else if (snda_ecs_is_xml_element_node_with_name(cur, "NextMarker")) {
			ptr = &(content->nextmarker);
		} else {
			if (snda_ecs_is_xml_element_node_with_name(cur, "MaxKeys")) {
				if (cur->children->content && strlen((char*)(cur->children->content))) {
					content->maxkeys = atoi((char*)(cur->children->content));
				}
			} else if (snda_ecs_is_xml_element_node_with_name(cur, "IsTruncated")) {
				if (cur->children->content && strlen((char*)(cur->children->content))) {
					if (!strcmp((char*)(cur->children->content), "false")) {
						content->istruncated = 0;
					} else if (!strcmp((char*)(cur->children->content), "true")) {
						content->istruncated = 1;
					}
				}
			} else if (snda_ecs_is_xml_element_node_with_name(cur, "Contents")) {
				if (cur->xmlChildrenNode) {
					if (!currentcontentnode) {
						content->objects = snda_ecs_to_object_content(cur);
						currentcontentnode = content->objects;
					} else {
						currentcontentnode->next = snda_ecs_to_object_content(cur);
						currentcontentnode = currentcontentnode->next;
					}
				}
			} else if (snda_ecs_is_xml_element_node_with_name(cur, "CommonPrefixes")) {
				if (cur->xmlChildrenNode) {
					if (!currentcommonprefix) {
						content->commonprefixes = snda_ecs_to_common_prefix(cur);
						currentcommonprefix = content->commonprefixes;
					} else {
						currentcommonprefix->next = snda_ecs_to_common_prefix(cur);
						currentcommonprefix = currentcommonprefix->next;
					}
				}
			}
			continue;
		}

		if (cur->children->content) {
			char* value = (char*)(cur->children->content);
			*ptr = (char*)calloc(strlen(value) + 1, sizeof(char));
			strcpy(*ptr, value);
		}
	}


	xmlFreeDoc(doc);

	return content;
}


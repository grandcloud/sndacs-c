/*
 * ProvideBy: SNDA Cloud Computing(http://www.grandcloud.cn).
 *
 * Implementation of c sdk for Elastic Cloud Storage of GrandCloud.
 */

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

	char* prefix = "<CreateBucketConfiguration><LocationConstraint>";
	char* suffix = "</LocationConstraint></CreateBucketConfiguration>";
	int xmlinputbuffsize = strlen(prefix) + strlen(region) + strlen(suffix) + 1;

	SNDAECSReadBuff buff;
	buff.consumed = 0;
	buff.databuff = (char*)malloc(xmlinputbuffsize);
	sprintf(buff.databuff, "%s", prefix);
	sprintf(buff.databuff + strlen(buff.databuff), "%s", region);
	sprintf(buff.databuff + strlen(buff.databuff), "%s", suffix);
	buff.datasize = strlen(buff.databuff);

	SNDAECSErrorCode retcode = snda_ecs_bucket_opt(handler, accesskey, secretkey,
			bucketname, region, subresource, subresource, ssl, &buff,
			SNDA_ECS_PUT, SNDA_ECS_NOT_FOLLOW_LOCATION, 0,
			ret);

	free(buff.databuff);

	return retcode;
}


SNDAECSErrorCode snda_ecs_put_bucket_policy(SNDAECSHandler* handler, const char* accesskey,
		const char* secretkey, const char* bucketname, const char* policy,
		int ssl, SNDAECSResult* ret)
{
	const char* subresource = "?policy";
	const char* region = 0;

	SNDAECSReadBuff buff;
	buff.consumed = 0;
	buff.databuff = calloc(strlen(policy) + 1, sizeof(char));
	memcpy(buff.databuff, policy, strlen(policy));
	buff.datasize = strlen(buff.databuff);

	SNDAECSErrorCode retcode = snda_ecs_bucket_opt(handler, accesskey, secretkey,
			bucketname, region, subresource, subresource, ssl, &buff,
			SNDA_ECS_PUT, SNDA_ECS_NOT_FOLLOW_LOCATION, 0,
			ret);

	free(buff.databuff);

	return retcode;
}



SNDAECSErrorCode snda_ecs_get_bucket_policy(SNDAECSHandler* handler, const char* accesskey,
		const char* secretkey, const char* bucketname, int ssl, SNDAECSResult* ret)
{
	const char* subresource = "?policy";
	const char* region = 0;

	return snda_ecs_bucket_opt(handler, accesskey, secretkey,
			bucketname, region, subresource, subresource, ssl, 0,
			SNDA_ECS_GET, SNDA_ECS_NOT_FOLLOW_LOCATION, 0,
			ret);
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

SNDAECSBucketLocation* snda_ecs_to_bucket_location(SNDAECSResult* ret)
{
	if (!snda_ecs_is_result_with_content(ret)) {
		return NULL;
	}

	xmlDocPtr doc = xmlParseMemory(ret->serverresponse->responsebody->retbody,
			ret->serverresponse->responsebody->retbodysize);

	if (!doc) {
		return NULL;
	}
	xmlNodePtr cur = xmlDocGetRootElement(doc);
	if (!cur) {
		xmlFreeDoc(doc);
		return NULL;
	}

	if (!snda_ecs_is_xml_element_node_with_name(cur, "LocationConstraint")) {
		xmlFreeDoc(doc);
		return NULL;
	}

	cur = cur->children;
	char* location = (char*)snda_ecs_region_default;
	if (cur) {
		location = cur->content;
	}

	SNDAECSBucketLocation* bucketlocation = snda_ecs_init_bucket_location();
	bucketlocation->location = (char*)malloc(strlen(location) + 1);
	sprintf(bucketlocation->location, "%s", location);

	xmlFreeDoc(doc);

	return bucketlocation;
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
	maxkeys = maxkeys > 1000 ? 1000 : maxkeys;
	char maxkeysstr[24];
	sprintf(maxkeysstr, "?max-keys=%d", maxkeys);
	int subresourcelen = strlen(maxkeysstr) + 1;

	const char* prefixlabel = "&prefix=";
	const char* markerlabel = "&marker=";
	const char* delimiterlabel = "&delimiter=";
	if (prefix && strlen(prefix)) {
		subresourcelen += strlen(prefixlabel) + strlen(prefix);
	}
	if (marker && strlen(marker)) {
		subresourcelen += strlen(markerlabel) + strlen(marker);
	}
	if (delimiter && strlen(delimiter)) {
		subresourcelen += strlen(delimiterlabel) + strlen(delimiter);
	}

	char subresource[subresourcelen];
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

	return snda_ecs_bucket_opt(handler, accesskey, secretkey,
			bucketname, region, subresource, 0, ssl, 0,
			SNDA_ECS_GET, followlocation, maxredirects,
			ret);
}

static SNDAECSObjectContent* snda_ecs_to_object_content(const xmlNodePtr contentelementnode)
{
	if (!snda_ecs_is_xml_element_node_with_name(contentelementnode, "Contents")) {
		return NULL;
	}

	if (!contentelementnode || !(contentelementnode->xmlChildrenNode)) {
		return NULL;
	}

	SNDAECSObjectContent *node = snda_ecs_init_object_content();
	xmlNodePtr child = contentelementnode->xmlChildrenNode;
	for (; child; child = child->next) {
		char **ptr = 0;
		if (snda_ecs_is_xml_element_node_with_name(child, "Key")) {
			ptr = &(node->objectname);
		} else if (snda_ecs_is_xml_element_node_with_name(child, "LastModified")) {
			ptr = &(node->lastmodified);
		} else if (snda_ecs_is_xml_element_node_with_name(child, "ETag")) {
			ptr = &(node->etag);
		} else if (snda_ecs_is_xml_element_node_with_name(child, "Size")) {
			if (child->children->content && strlen(child->children->content)) {
				node->size = atol(child->children->content);
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
	if (!snda_ecs_is_result_with_content(result)) {
		return NULL;
	}

	xmlDocPtr doc = xmlParseMemory(result->serverresponse->responsebody->retbody,
			result->serverresponse->responsebody->retbodysize);

	if (!doc) {
		return NULL;
	}
	xmlNodePtr cur = xmlDocGetRootElement(doc);
	if (!cur) {
		xmlFreeDoc(doc);
		return NULL;
	}
	if (!snda_ecs_is_xml_element_node_with_name(cur, "ListBucketResult")) {
		xmlFreeDoc(doc);
		return NULL;
	}

	SNDAECSGetBucketResultContent* content = snda_ecs_init_get_bucket_result_content();
	SNDAECSObjectContent* currentcontentnode = content->objects;
	SNDAECSCommonPrefix * currentcommonprefix = content->commonprefixes;
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
				if (cur->children->content && strlen(cur->children->content)) {
					content->maxkeys = atoi(cur->children->content);
				}
			} else if (snda_ecs_is_xml_element_node_with_name(cur, "IsTruncated")) {
				if (cur->children->content && strlen(cur->children->content)) {
					if (!strcmp(cur->children->content, "false")) {
						content->istruncated = 0;
					} else if (!strcmp(cur->children->content, "true")) {
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


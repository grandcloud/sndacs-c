/*
 * ProvideBy: SNDA Cloud Computing(http://www.grandcloud.cn).
 *
 * Implementation of c sdk for Elastic Cloud Storage of GrandCloud.
 */

#include "snda_ecs_sdk.h"
#include "snda_ecs_http_util.h"
#include "snda_ecs_constants.h"
#include "snda_ecs_common_util.h"

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
		const char* secretkey, const char* bucketname, const char* objectname, const SNDAECSUserObjectMeta* userobjectmeta,
		const char* region, int ssl, SNDAECSFollowLocation followlocation, long maxredirects,
		SNDAECSResult* ret) {

	char subresource[strlen("/") + strlen(objectname) + strlen("?uploads") + 1];
	sprintf(subresource, "/%s?uploads", objectname);

	SNDAECSErrorCode retcode = snda_ecs_common_opt(handler, accesskey, secretkey,
			bucketname, region, subresource, subresource, ssl, 0, userobjectmeta,
			SNDA_ECS_POST, followlocation, maxredirects,
			0, 0, 0,
			snda_ecs_get_server_response_body,
			ret->serverresponse->responsebody,
			ret);

	return retcode;
}

SNDAECSInitiateMultipartUploadResult* snda_ecs_to_initiate_multipart_upload_result(SNDAECSResult* ret) {
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

	if (!snda_ecs_is_xml_element_node_with_name(cur, "InitiateMultipartUploadResult")) {
		xmlFreeDoc(doc);
		return NULL;
	}

	SNDAECSInitiateMultipartUploadResult* content = snda_ecs_init_initiate_multipart_upload_result();
	for (cur = cur->xmlChildrenNode; cur; cur = cur->next) {
		if (snda_ecs_is_xml_element_node_with_name(cur, "Bucket")) {
			snda_ecs_copy_string(&(content->bucket), cur->children->content);
		} else if (snda_ecs_is_xml_element_node_with_name(cur, "Key")) {
			snda_ecs_copy_string(&(content->key), cur->children->content);
		} else if (snda_ecs_is_xml_element_node_with_name(cur, "UploadId")) {
			snda_ecs_copy_string(&(content->uploadid), cur->children->content);
		}
	}

	xmlFreeDoc(doc);
	return content;
}

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
		SNDAECSResult* ret)
{
	const char* tosign = "?uploads";

	maxuploads = maxuploads > 1000 ? 1000 : maxuploads;
	char maxuploadsstr[32];
	sprintf(maxuploadsstr, "%s&max-uploads=%d", tosign, maxuploads);
	int subresourcelen = strlen(maxuploadsstr) + 1;

	const char* prefixlabel = "&prefix=";
	const char* keymarkerlabel = "&key-marker=";
	const char* uploadidmarkerlabel = "&upload-id-marker=";
	const char* delimiterlabel = "&delimiter=";
	if (prefix && strlen(prefix)) {
		subresourcelen += strlen(prefixlabel) + strlen(prefix);
	}
	if (keymarker && strlen(keymarker)) {
		subresourcelen += strlen(keymarkerlabel) + strlen(keymarker);
	}
	if (uploadidmarker && strlen(uploadidmarker)) {
		subresourcelen += strlen(uploadidmarkerlabel) + strlen(uploadidmarker);
	}
	if (delimiter && strlen(delimiter)) {
		subresourcelen += strlen(delimiterlabel) + strlen(delimiter);
	}

	char subresource[subresourcelen];
	sprintf(subresource, "%s", maxuploadsstr);
	if (prefix && strlen(prefix)) {
		sprintf(subresource + strlen(subresource), "%s%s", prefixlabel, prefix);
	}
	if (keymarker && strlen(keymarker)) {
		sprintf(subresource + strlen(subresource), "%s%s", keymarkerlabel, keymarker);
	}
	if (uploadidmarker && strlen(uploadidmarker)) {
		sprintf(subresource + strlen(subresource), "%s%s", uploadidmarkerlabel, uploadidmarker);
	}
	if (delimiter && strlen(delimiter)) {
		sprintf(subresource + strlen(subresource), "%s%s", delimiterlabel, delimiter);
	}

	SNDAECSErrorCode retcode = snda_ecs_common_opt(handler, accesskey, secretkey,
			bucketname, region, subresource, tosign, ssl, 0, 0,
			SNDA_ECS_GET, followlocation, maxredirects,
			0, 0, 0,
			snda_ecs_get_server_response_body,
			ret->serverresponse->responsebody,
			ret);

	return retcode;
}

static SNDAECSMultipartUpload* snda_ecs_to_one_upload(const xmlNodePtr uploadelementnode) {
	if (!snda_ecs_is_xml_element_node_with_name(uploadelementnode, "Upload")) {
		return NULL;
	}

	if (!uploadelementnode || !(uploadelementnode->xmlChildrenNode)) {
		return NULL;
	}

	SNDAECSMultipartUpload *node = snda_ecs_init_multipart_upload();
	xmlNodePtr child = uploadelementnode->xmlChildrenNode;
	for (; child; child = child->next) {
		if (snda_ecs_is_xml_element_node_with_name(child, "Key")) {
			snda_ecs_copy_string(&(node->key), child->children->content);
		} else if (snda_ecs_is_xml_element_node_with_name(child, "UploadId")) {
			snda_ecs_copy_string(&(node->uploadid), child->children->content);
		} else if (snda_ecs_is_xml_element_node_with_name(child, "Initiated")) {
			snda_ecs_copy_string(&(node->initiatedtime), child->children->content);
		}
	}

	return node;
}

SNDAECSMultipartUploadsContent* snda_ecs_to_multipart_uploads_content(SNDAECSResult* ret) {
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
	if (!snda_ecs_is_xml_element_node_with_name(cur, "ListMultipartUploadsResult")) {
		xmlFreeDoc(doc);
		return NULL;
	}

	SNDAECSMultipartUploadsContent* content = snda_ecs_init_multipart_uploads_content();
	SNDAECSMultipartUpload* currentupload = content->upload;
	SNDAECSCommonPrefix* commonprefix = content->commonprefixes;

	for (cur = cur->xmlChildrenNode; cur; cur = cur->next) {
		if (snda_ecs_is_xml_element_node_with_name(cur, "Bucket")) {
			snda_ecs_copy_string(&(content->bucket), cur->children->content);
		} else if (snda_ecs_is_xml_element_node_with_name(cur, "Delimiter")) {
			snda_ecs_copy_string(&(content->delimiter), cur->children->content);
		} else if (snda_ecs_is_xml_element_node_with_name(cur, "Prefix")) {
			snda_ecs_copy_string(&(content->prefix), cur->children->content);
		} else if (snda_ecs_is_xml_element_node_with_name(cur, "KeyMarker")) {
			snda_ecs_copy_string(&(content->keymarker), cur->children->content);
		} else if (snda_ecs_is_xml_element_node_with_name(cur, "UploadIdMarker")) {
			snda_ecs_copy_string(&(content->uploadidmarker), cur->children->content);
		} else if (snda_ecs_is_xml_element_node_with_name(cur, "NextKeyMarker")) {
			snda_ecs_copy_string(&(content->nextkeymarker), cur->children->content);
		} else if (snda_ecs_is_xml_element_node_with_name(cur, "NextUploadIdMarker")) {
			snda_ecs_copy_string(&(content->nextuploadidmarker), cur->children->content);
		} else if (snda_ecs_is_xml_element_node_with_name(cur, "MaxUploads")) {
			if (cur->children->content && strlen(cur->children->content)) {
				content->maxuploads = atoi(cur->children->content);
			}
		} else if (snda_ecs_is_xml_element_node_with_name(cur, "IsTruncated")) {
			if (cur->children->content && strlen(cur->children->content)) {
				if (!strcmp(cur->children->content, "false")) {
					content->istruncated = 0;
				} else if (!strcmp(cur->children->content, "true")) {
					content->istruncated = 1;
				}
			}
		} else if (snda_ecs_is_xml_element_node_with_name(cur, "Upload")) {
			if (cur->xmlChildrenNode) {
				if (!currentupload) {
					content->upload = snda_ecs_to_one_upload(cur);
					currentupload = content->upload;
				} else {
					currentupload->next = snda_ecs_to_one_upload(cur);
					currentupload = currentupload->next;
				}
			}
		} else if (snda_ecs_is_xml_element_node_with_name(cur, "CommonPrefixes")) {
			if (cur->xmlChildrenNode) {
				if (!commonprefix) {
					content->commonprefixes = snda_ecs_to_common_prefix(cur);
					commonprefix = content->commonprefixes;
				} else {
					commonprefix->next = snda_ecs_to_common_prefix(cur);
					commonprefix = commonprefix->next;
				}
			}
		}
	}


	xmlFreeDoc(doc);

	return content;
}

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
		SNDAECSResult* ret)
{
	const char* mask = "?uploadId=";
	char subresource[strlen("/") + strlen(objectname) + strlen(mask) + strlen(uploadid) + 1];
	sprintf(subresource, "/%s%s%s", objectname, mask, uploadid);

	SNDAECSErrorCode retcode = snda_ecs_common_opt(handler, accesskey, secretkey,
			bucketname, region, subresource, subresource, ssl, 0, 0,
			SNDA_ECS_DELETE, followlocation, maxredirects,
			0, 0, 0,
			snda_ecs_get_server_response_body,
			ret->serverresponse->responsebody,
			ret);

	return retcode;
}

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
		SNDAECSResult* ret)
{
	int len = strlen("/") + strlen(objectname) +
			strlen("&uploadId=")  + strlen(uploadid) +
			strlen("?partNumber=") + 16;
	char subresource[len];
	sprintf(subresource, "/%s?partNumber=%d&uploadId=%s", objectname, partnumber, uploadid);

	SNDAECSReadBuff buff;
	buff.databuff = inputstream;
	buff.consumed = 0;
	buff.datasize = contentlength;

	SNDAECSUserObjectMeta* userobjectmeta = snda_ecs_init_user_object_meta();
	snda_ecs_set_object_md5(userobjectmeta,  contentmd5);

	SNDAECSErrorCode retcode = snda_ecs_common_opt(handler, accesskey, secretkey,
			bucketname, region, subresource, subresource, ssl, 0, userobjectmeta,
			SNDA_ECS_PUT_RESPONSE_HEAD, SNDA_ECS_NOT_FOLLOW_LOCATION, 0L,
			readFun, &buff, buff.datasize,
			snda_ecs_get_server_response_body,
			ret->serverresponse->responsebody,
			ret);


	snda_ecs_release_user_object_meta(userobjectmeta);

	return retcode;
}

char* snda_ecs_to_upload_part_etag(char* etag, SNDAECSResult* ret) {
	if (!snda_ecs_is_result_with_content(ret)) {
		return NULL;
	}

	const char* p = (char*)(ret->serverresponse->responsebody->retbody);
	int i = 0;
	int segmentbegin = 0;
	for (; i < ret->serverresponse->responsebody->retbodysize; ++i) {
		if (p[i] == '\n') {
			int segmentsize = i - segmentbegin;
			if (segmentsize > 0) {
				char segment[segmentsize];
				memcpy(segment, p + segmentbegin, segmentsize);
				// split this segment
				char key[segmentsize + 1];
				int n = 0;
				for (; n < segmentsize; ++n) {
					if (segment[n] == ':') {
						break;
					}
					key[n] = segment[n];
				}
				key[n] = '\0';

				int j = 0;
				char value[segmentsize + 1];
				for (++n; n < segmentsize; ++n) {
					if (segment[n] != ' ' && segment[n] != '\t') {
						value[j++] = segment[n];
					}
				}
				value[j] = '\0';

				if (strlen(key) && strcmp(key, "\r")) {
					if (!strcmp(key, "ETag")) {
						snda_ecs_copy_string(&etag, value);
						break;
					}
				}
			}
			segmentbegin = i + 1;
		}
	}
	return etag;
}


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
		SNDAECSResult* ret)
{

	int len = strlen("/") + strlen(objectname) + strlen("?uploadId=")  + strlen(uploadid) + 1;
	char subresourcetosign[len];
	sprintf(subresourcetosign, "/%s?uploadId=%s", objectname, uploadid);

	maxparts = maxparts > 1000 ? 1000 : maxparts;
	len += strlen("&max-parts=") + 8;

	if (partnumbermarker >= 0) {
		partnumbermarker = partnumbermarker > 10000 ? 10000 : partnumbermarker;
		len += strlen("&part-number-marker=") + 8;
	}

	char subresource[len];
	sprintf(subresource, "%s&max-parts=%d", subresourcetosign, maxparts);
	if (partnumbermarker >= 0) {
		sprintf(subresource + strlen(subresource), "&part-number-marker=%d", partnumbermarker);
	}

	SNDAECSErrorCode retcode = snda_ecs_common_opt(handler, accesskey, secretkey,
			bucketname, region, subresource, subresourcetosign, ssl, 0, 0,
			SNDA_ECS_GET, followlocation, maxredirects,
			0, 0, 0,
			snda_ecs_get_server_response_body,
			ret->serverresponse->responsebody,
			ret);

	return retcode;
}

static SNDAECSMultipartsPart* snda_ecs_to_one_part(const xmlNodePtr partelementnode) {
	if (!snda_ecs_is_xml_element_node_with_name(partelementnode, "Part")) {
		return NULL;
	}

	if (!partelementnode || !(partelementnode->xmlChildrenNode)) {
		return NULL;
	}

	SNDAECSMultipartsPart *node = snda_ecs_init_multiparts_part();
	xmlNodePtr child = partelementnode->xmlChildrenNode;
	for (; child; child = child->next) {
		if (snda_ecs_is_xml_element_node_with_name(child, "PartNumber")) {
			if (child->children->content && strlen(child->children->content)) {
				node->partnumber = atoi(child->children->content);
			}
		} else if (snda_ecs_is_xml_element_node_with_name(child, "LastModified")) {
			snda_ecs_copy_string(&(node->lastmodified), child->children->content);
		} else if (snda_ecs_is_xml_element_node_with_name(child, "ETag")) {
			snda_ecs_copy_string(&(node->etag), child->children->content);
		} else if (snda_ecs_is_xml_element_node_with_name(child, "Size")) {
			if (child->children->content && strlen(child->children->content)) {
				node->size = atol(child->children->content);
			}
		}
	}

	return node;
}


SNDAECSMultipartsContent* snda_ecs_to_multipart_parts(SNDAECSResult* ret) {
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
	if (!snda_ecs_is_xml_element_node_with_name(cur, "ListPartsResult")) {
		xmlFreeDoc(doc);
		return NULL;
	}

	SNDAECSMultipartsContent* content = snda_ecs_init_multiparts_content();
	SNDAECSMultipartsPart* currentparts = content->parts;
	for (cur = cur->xmlChildrenNode; cur; cur = cur->next) {
		if (snda_ecs_is_xml_element_node_with_name(cur, "Bucket")) {
			snda_ecs_copy_string(&(content->bucket), cur->children->content);
		} else if (snda_ecs_is_xml_element_node_with_name(cur, "Key")) {
			snda_ecs_copy_string(&(content->key), cur->children->content);
		} else if (snda_ecs_is_xml_element_node_with_name(cur, "UploadId")) {
			snda_ecs_copy_string(&(content->uploadid), cur->children->content);
		} else if (snda_ecs_is_xml_element_node_with_name(cur, "PartNumberMarker")) {
			if (cur->children->content && strlen(cur->children->content)) {
				content->partnumbermarker = atoi(cur->children->content);
			}
		} else if (snda_ecs_is_xml_element_node_with_name(cur, "MaxParts")) {
			if (cur->children->content && strlen(cur->children->content)) {
				content->maxparts = atoi(cur->children->content);
			}
		} else if (snda_ecs_is_xml_element_node_with_name(cur, "IsTruncated")) {
			if (cur->children->content && strlen(cur->children->content)) {
				if (!strcmp(cur->children->content, "false")) {
					content->istruncated = 0;
				} else if (!strcmp(cur->children->content, "true")) {
					content->istruncated = 1;
				}
			}
		} else if (snda_ecs_is_xml_element_node_with_name(cur, "NextPartNumberMarker")) {
			if (cur->children->content && strlen(cur->children->content)) {
				content->nextpartnumbermarker = atoi(cur->children->content);
			}
		} else if (snda_ecs_is_xml_element_node_with_name(cur, "Part")) {
			if (cur->xmlChildrenNode) {
				if (!currentparts) {
					content->parts = snda_ecs_to_one_part(cur);
					currentparts = content->parts;
				} else {
					currentparts->next = snda_ecs_to_one_part(cur);
					currentparts = currentparts->next;
				}
			}
		}
	}


	xmlFreeDoc(doc);

	return content;
}

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
		SNDAECSResult* ret)
{
	int len = strlen("/") + strlen(objectname) + strlen("?uploadId=")  + strlen(uploadid) + 1;
	char subresource[len];
	sprintf(subresource, "/%s?uploadId=%s", objectname, uploadid);

	const char* bodybegin = "<CompleteMultipartUpload>";
	const char* bodyend = "</CompleteMultipartUpload>";

	const char* prefix = "<Part><PartNumber>";
	const char* middle = "</PartNumber><ETag>";
	const char* suffix = "</ETag></Part>";

	int bodylen = strlen(bodybegin) + strlen(bodyend) + 1;

	const SNDAECSMultipartsMeta* p = partsmeta;
	for (; p; p = p->next) {
		// we need to add parameter error handling everywhere to give a friendly error message
		if (p->partnumber < 0 || p->partnumber > 10000 || p->etag == 0) {
			char error[S_SNDA_ECS_CLIENT_ERROR_LEN];
			snprintf(error, S_SNDA_ECS_CLIENT_ERROR_LEN,
					"bad part number:%d or bad etag:%s", p->partnumber, p->etag);
			snda_ecs_set_handler_error(ret, error);
			return SNDA_ECS_BAD_MULTIPARTS_METAS;
		}

		bodylen += strlen(prefix) + 5 + strlen(middle) + strlen(p->etag) + strlen(suffix);
	}

	char body[bodylen];
	sprintf(body, "%s", bodybegin);
	p = partsmeta;
	for (; p; p = p->next) {
		sprintf(body + strlen(body), "%s%d%s%s%s", prefix, p->partnumber, middle, p->etag, suffix);
	}
	sprintf(body + strlen(body), "%s", bodyend);

	SNDAECSReadBuff buff;
	buff.consumed = 0;
	buff.databuff = body;
	buff.datasize = strlen(buff.databuff);

	CallbackFunPtr readfun = snda_ecs_put_xml_body;

	return snda_ecs_common_opt(handler, accesskey, secretkey,
			bucketname, region, subresource, subresource,
			ssl, 0, 0, SNDA_ECS_POST, followlocation, maxredirects,
			readfun, &buff, buff.datasize,
			snda_ecs_get_server_response_body, ret->serverresponse->responsebody,
			ret);
}



#include "global.h"
/***
 * A full example for upload an object with multipart upload
 */
const int PART_SIZE = 5 * 1024 * 1024;

SNDAECSInitiateMultipartUploadResult * initiate_multipart_upload(
		const char* accesskey, const char* secretkey, const char* bucket,
		const char *region, const char * objectname, int ssl,
		int followlocation, int maxredirects) {
	SNDAECSHandler* handler = 0;
	SNDAECSResult* ret = 0;
	SNDAECSInitiateMultipartUploadResult* multipartcontent = 0;
	SNDAECSUserObjectMeta* objectmeta = 0;
	char contenttype[S_SNDA_ECS_CONTENT_TYPE_LEN];
	SNDAECSErrorCode retcode;

	snda_ecs_global_init();
	handler = snda_ecs_init_handler();
	ret = snda_ecs_init_result();
	multipartcontent = 0;
	objectmeta = snda_ecs_init_user_object_meta();
	
	snda_ecs_set_object_type(objectmeta,
			snda_ecs_get_content_type(objectname, contenttype));
	retcode = snda_ecs_initiate_multipart_upload(handler,
			accesskey, secretkey, bucket, objectname, objectmeta, region, ssl,
			followlocation, maxredirects, ret);
	snda_ecs_release_user_object_meta(objectmeta);
	if (retcode != SNDA_ECS_SUCCESS) {
		printf("ClientErrorMessage:%s", ret->error->handlererrmsg);
	} else if (ret->serverresponse->httpcode >= 300) {
		SNDAECSErrorResponseContent* content = snda_ecs_to_error_response(ret);
		if (content) {
			if (content->code) {
				printf("ErrorCode:%s\n", content->code);
			}
			if (content->message) {
				printf("ErrorMessage:%s\n", content->message);
			}
			if (content->resource) {
				printf("Resource:%s\n", content->resource);
			}
			if (content->requestid) {
				printf("RequestId:%s\n", content->requestid);
			}
			if (content->fullbody) {
				printf("AllErrorMessage:%s\n", content->fullbody);
			}
		}
		snda_ecs_release_error_response_content(content);
	} else {
		printf("Initiate multipart upload success and the http code is:%d\n",
				ret->serverresponse->httpcode);
		multipartcontent = snda_ecs_to_initiate_multipart_upload_result(ret);
		if (multipartcontent) {
			if (multipartcontent->bucket) {
				printf("Bucket:%s\n", multipartcontent->bucket);

			}
			if (multipartcontent->key) {
				printf("Key:%s\n", multipartcontent->key);

			}
			if (multipartcontent->uploadid) {
				printf("UploadId:%s\n", multipartcontent->uploadid);
			}
		}
		//snda_ecs_release_initiate_multipart_upload_result(content);
	}

	snda_ecs_release_handler(handler);
	snda_ecs_release_result(ret);
	snda_ecs_global_uninit();
	return multipartcontent;
}

int multipart_upload(const char* accesskey, const char* secretkey,
		const char* bucket, const char* objectname, const char* uploadid,
		const char* region, int ssl, const char * localfile) {

	SNDAECSHandler* handler = 0;
	SNDAECSResult* ret = 0;
	char * contentmd5 = 0;
	FILE* fd = 0;
	long flength = 0;
	long cur,next,realsize;
	int partnumber ;
	int success = 0;

	snda_ecs_global_init();
	handler = snda_ecs_init_handler();
	ret = snda_ecs_init_result();
	contentmd5 = 0;
	fd = fopen(localfile, "rb");
	fseek(fd, 0L, SEEK_END);
	flength = ftell(fd);
	//int part_number = flength/PART_SIZE + 1;
	fseek(fd, 0L, 0);
	cur = 0;
	realsize = PART_SIZE;
	partnumber = 1;
	while (cur < flength) {
		SNDAECSErrorCode retcode;
		next = cur + PART_SIZE;
		if (next > flength) {
			next = flength;
			realsize = next - cur;
		}

		retcode = snda_ecs_upload_part(handler, accesskey,
				secretkey, bucket, objectname, uploadid, partnumber,
				snda_ecs_put_object_body, fd, realsize, contentmd5, region,
				ssl, ret);
		success = 0;
		if (retcode != SNDA_ECS_SUCCESS) {
			printf("ClientErrorMessage:%s", ret->error->handlererrmsg);
		} else if (ret->serverresponse->httpcode >= 300) {
			SNDAECSErrorResponseContent* content = snda_ecs_to_error_response(
					ret);
			if (content) {
				if (content->code) {
					printf("ErrorCode:%s\n", content->code);
				}
				if (content->message) {
					printf("ErrorMessage:%s\n", content->message);
				}
				if (content->resource) {
					printf("Resource:%s\n", content->resource);
				}
				if (content->requestid) {
					printf("RequestId:%s\n", content->requestid);
				}
				if (content->fullbody) {
					printf("AllErrorMessage:%s\n", content->fullbody);
				}
			}
			snda_ecs_release_error_response_content(content);
		} else {
			printf("Upload part success and the http code is %d\n",
					ret->serverresponse->httpcode);
			success = 1;
		}

		if (success == 1) {
			printf("Upload parts %d success!\n", partnumber);
		} else {
			printf("Upload parts %d Failed!\n", partnumber);
			break;
		}
		cur = next;
		partnumber++;
		fseek(fd, cur, 0);
	}

	fclose(fd);
	snda_ecs_release_handler(handler);
	snda_ecs_release_result(ret);
	snda_ecs_global_uninit();
	return success;
}

SNDAECSMultipartsContent* list_parts(const char* accesskey,
		const char* secretkey, const char* bucket, const char *region,
		const char * objectname, const char * uploadid, int ssl,
		int followlocation, int partnumbermarker, int maxparts, int maxdirects) {
	SNDAECSHandler* handler = 0;
	SNDAECSResult* ret = 0;
	SNDAECSErrorCode retcode = 0;
	SNDAECSMultipartsContent* multipartcontent = 0;
   
	snda_ecs_global_init();
	handler = snda_ecs_init_handler();
	ret = snda_ecs_init_result();
	retcode = snda_ecs_list_parts(handler, accesskey,
			secretkey, bucket, objectname, uploadid, partnumbermarker,
			maxparts, region, ssl, followlocation, maxdirects, ret);
	
	if (retcode != SNDA_ECS_SUCCESS) {
		printf("ClientErrorMessage:%s", ret->error->handlererrmsg);
	} else if (ret->serverresponse->httpcode < 300) {
		multipartcontent = snda_ecs_to_multipart_parts(ret);
		if (multipartcontent) {
			SNDAECSMultipartsPart* part = 0;
			printf("Bucket:%s\n", multipartcontent->bucket);
			printf("Key:%s\n", multipartcontent->key);
			printf("UploadId:%s\n", multipartcontent->uploadid);
			printf("MaxParts:%d\n", multipartcontent->maxparts);
			printf("IsTruncated:%d\n", multipartcontent->istruncated);
			printf("PartNumberMarker:%d\n", multipartcontent->partnumbermarker);
			printf("NextPartNumberMarker:%d\n",
					multipartcontent->nextpartnumbermarker);

			printf("PARTS/\n");
			part = multipartcontent->parts;
			while (part) {
				printf("\tPART/\n");
				printf("\t\tPartNumber:%d\n", part->partnumber);
				printf("\t\tSize:%ld\n", part->size);
				printf("\t\tLastModified:%s\n", part->lastmodified);
				printf("\t\tETag:%s\n", part->etag);
				part = part->next;
				printf("\t/PART\n");
			}
			printf("/PARTS\n");
		}
		//snda_ecs_release_multiparts_content(content);
	} else {
		SNDAECSErrorResponseContent* content = snda_ecs_to_error_response(ret);
		if (content) {
			if (content->code) {
				printf("ErrorCode:%s\n", content->code);
			}
			if (content->message) {
				printf("ErrorMessage:%s\n", content->message);
			}
			if (content->resource) {
				printf("Resource:%s\n", content->resource);
			}
			if (content->requestid) {
				printf("RequestId:%s\n", content->requestid);
			}
			if (content->fullbody) {
				printf("AllErrorMessage:%s\n", content->fullbody);
			}
		}
		if(ret->serverresponse->httpcode == 505) {
		  printf("Please check your bucketname,accessKey,SecretAccessKey!\n");
		}
		snda_ecs_release_error_response_content(content);
	}
	snda_ecs_release_handler(handler);
	snda_ecs_release_result(ret);
	snda_ecs_global_uninit();
	return multipartcontent;
}

void complete_multipart_upload(const char* accesskey,
		const char* secretkey, const char* bucket, const char *region,
		const char * objectname, const char * uploadid, int ssl,
		int followlocation, int maxdirects,
		const SNDAECSMultipartsContent * multipartcontent) {
	SNDAECSHandler* handler = 0;
	SNDAECSResult* ret = 0;
	SNDAECSMultipartsPart* part = 0;
	SNDAECSMultipartsMeta* metas = 0;
	SNDAECSMultipartsMeta* p = 0;
	SNDAECSErrorCode retcode;

	snda_ecs_global_init();
	handler = snda_ecs_init_handler();
	ret = snda_ecs_init_result();
	part = multipartcontent->parts;
	metas = snda_ecs_init_multiparts_meta();
	p = metas;
	while (part) {
		p->partnumber = part->partnumber;
		snda_ecs_copy_string(&(p->etag), part->etag);
		part = part->next;
		if (part) {
			p->next = snda_ecs_init_multiparts_meta();
			p = p->next;
		}
	}

	retcode = snda_ecs_complete_multipart_upload(handler,
			accesskey, secretkey, bucket, objectname, uploadid, metas, region,
			ssl, followlocation, maxdirects, ret);

	snda_ecs_release_multiparts_meta(metas);

	if (retcode != SNDA_ECS_SUCCESS) {
		printf("ClientErrorMessage:%s", ret->error->handlererrmsg);
	} else if (ret->serverresponse->httpcode >= 300) {
		SNDAECSErrorResponseContent* content = snda_ecs_to_error_response(ret);
		if (content) {
			if (content->code) {
				printf("ErrorCode:%s\n", content->code);
			}
			if (content->message) {
				printf("ErrorMessage:%s\n", content->message);
			}
			if (content->resource) {
				printf("Resource:%s\n", content->resource);
			}
			if (content->requestid) {
				printf("RequestId:%s\n", content->requestid);
			}
			if (content->fullbody) {
				printf("AllErrorMessage:%s\n", content->fullbody);
			}
		}
		if(ret->serverresponse->httpcode == 505) {
		  printf("Please check your bucketname,accessKey,SecretAccessKey!\n");
		}
		snda_ecs_release_error_response_content(content);
	} else {
		printf("Complete multipart upload success and the http code is %d\n",
				ret->serverresponse->httpcode);
	}
	snda_ecs_release_handler(handler);
	snda_ecs_release_result(ret);
	snda_ecs_global_uninit();
}

void multipart_upload_one_object(const char * accesskey,
		const char * secretkey, const char * bucket, const char * objectname,
		const char * region, const char * localfile, int followlocation,
		int maxdirects, int ssl) {
	SNDAECSInitiateMultipartUploadResult * multipartcontent =
			initiate_multipart_upload(accesskey, secretkey, bucket,
					region, objectname, ssl, followlocation, maxdirects);
	if (multipartcontent && multipartcontent->uploadid
			&& multipartcontent->bucket && multipartcontent->key) {
		int partnumbermarker = 0;
		int maxparts = 1000;
		SNDAECSMultipartsContent * listcontent = 0;
		if(0==multipart_upload(accesskey, secretkey, bucket, objectname,multipartcontent->uploadid, region, ssl, localfile)) {
		    printf("Multipart Upload failed!\n");
			return;
		}
		partnumbermarker = 0;
		maxparts = 1000;
		//Get etags for each part
		listcontent = list_parts(accesskey,
				secretkey, bucket, region, objectname,
				multipartcontent->uploadid, ssl, followlocation,
				partnumbermarker, maxparts, maxdirects);
		if (listcontent) {
			complete_multipart_upload(accesskey, secretkey, bucket,
					region, objectname, multipartcontent->uploadid, ssl,
					followlocation, maxdirects, listcontent);
		}
		snda_ecs_release_multiparts_content(listcontent);
	}
	snda_ecs_release_initiate_multipart_upload_result(multipartcontent);
}
void testComplete() {
	char * accesskey = "your accesskey";
	char * secretkey = "your secret";
	char * bucket = "your bucket";
	char * objectname = "your objectname";
	char * region = "your region";
	char * localfile = "you big file";
	int followlocation = 0;
	int maxdirects = 0;
	int ssl = 0;
	int partnumbermarker = 0;
	int maxparts = 1000;
	SNDAECSMultipartsContent * listcontent = list_parts(accesskey,
			secretkey, bucket, region, objectname, "4S6NM3MBJBK9MET41W30LJWA3",
			ssl, followlocation, partnumbermarker, maxparts, maxdirects);
	if (listcontent) {
		complete_multipart_upload(accesskey, secretkey, bucket, region,
				objectname, "4S6NM3MBJBK9MET41W30LJWA3", ssl, followlocation,
				maxdirects, listcontent);
	}
	snda_ecs_release_multiparts_content(listcontent);
}

int main() {
	char * accesskey = SNDA_ACCESSKEY;//;"your accessKey";
	char * secretkey = SNDA_ACCESS_SECRET;//"your secretKey";
	char * bucket = SNDA_BUCKET_HUADONG;//"your bucketname";
	char * region = SDNA_REGION_HUADONG;//your region
	char * objectname = MULTIPART_UPLOAD_OBJECT;
	char * localfile = LOCAL_BIG_FILE_W;
	int followlocation = 0;
	int maxdirects = 0;
	int ssl = 0;
	multipart_upload_one_object(accesskey, secretkey, bucket,
			objectname, region, localfile, followlocation, maxdirects, ssl);

	return 0;
}

#include "stdafx.h"

void head_object_example(const char* accesskey, const char* secretkey,
		const char* bucketname, const char *region, const char * objectname,
		long byterangefirst, long byterangelast, int ssl, int followlocation,
		int maxredirects) {
	SNDAECSHandler* handler = snda_ecs_init_handler();
	SNDAECSResult* ret = snda_ecs_init_result();
	SNDAECSByteRange* byterangeptr = 0;
	SNDAECSErrorCode retcode ;

	snda_ecs_global_init();
	handler = snda_ecs_init_handler();
	ret = snda_ecs_init_result();
	byterangeptr = snda_ecs_init_byte_range();
	byterangeptr->first = byterangefirst;
	byterangeptr->last = byterangelast;

	retcode = snda_ecs_head_object(handler, accesskey,
			secretkey, bucketname, objectname, byterangeptr, region, ssl,
			(SNDAECSFollowLocation)followlocation, maxredirects, ret);
	snda_ecs_release_byte_range(byterangeptr);
	if (retcode != SNDA_ECS_SUCCESS) {
		printf("ClientErrorMessage:%s", ret->error->handlererrmsg);
	} else if (ret->serverresponse->httpcode >= 300) {
		SNDAECSErrorResponseContent* content = snda_ecs_to_error_response(ret);
		printf("Error,HTTP Code:%d\n", ret->serverresponse->httpcode);
		if(content) {
			printf("ErrorCode:%s\n", content->code);
			printf("ErrorMessage:%s\n", content->message);
			printf("Resource:%s\n", content->resource);
			printf("RequestId:%s\n", content->requestid);
			printf("AllErrorMessage:%s\n", content->fullbody);
			snda_ecs_release_error_response_content(content);
		}
		if(ret->serverresponse->httpcode == 505) {
		  printf("Please check your bucketname,accessKey,SecretAccessKey!\n");
		}
	} else {
		SNDAECSObjectMeta* objectmeta = snda_ecs_to_object_meta(ret);
		SNDAECSKVList* p = 0;
		printf("Etag;%s\n", objectmeta->etag);
		printf("Content-Type:%s\n", objectmeta->contenttype);
		printf("Content-Length:%ld\n", objectmeta->contentlength);
		printf("Last-Modified:%s\n", objectmeta->lastmodified);
		p = objectmeta->usermetas;
		for (; p; p = p->next) {
			printf("p->key:%s\n", p->value);
		}

		snda_ecs_release_object_meta(objectmeta);
	}

	snda_ecs_release_handler(handler);
	snda_ecs_release_result(ret);
	snda_ecs_global_uninit();
}

int head_object() {
	printf("\n\n\n*************Head Object Example**************\n");
	char * accesskey = SNDA_ACCESSKEY;//;"your accessKey";
	char * secretkey = SNDA_ACCESS_SECRET;//"your secretKey";
	char * bucketname = SNDA_BUCKET_HUADONG;//"your bucketname";
	char * region = SDNA_REGION_HUADONG;//your region
	char * objectname = "test.sh";
	long byterangefirst = 0;
	long byterangelast = 50000;
	int followlocation = 0;
	int maxredirects = 0;
	int ssl = 0;

	head_object_example(accesskey, secretkey, bucketname, region, objectname,
			byterangefirst, byterangelast, ssl, followlocation, maxredirects);
	return 0;
}


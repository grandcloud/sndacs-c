#include "stdafx.h"

void get_object_example(const char* accesskey, const char* secretkey,
		const char* bucket, const char *region, const char * objectname,
		const char * locafile, long byterangefirst, long byterangelast,
		int ssl, int followlocation, int maxredirects) {
	
	SNDAECSHandler* handler = 0;
	SNDAECSResult* ret = 0;
	SNDAECSByteRange* byterangeptr = 0;
	FILE* writefd = 0;
	SNDAECSErrorCode retcode;

	snda_ecs_global_init();
	handler = snda_ecs_init_handler();
	ret = snda_ecs_init_result();
	byterangeptr = snda_ecs_init_byte_range();

	byterangeptr->first = byterangefirst;
	byterangeptr->last = byterangelast;

	writefd = fopen(locafile, "wb");
	if(!writefd) {
	   printf("Please check your localfile path!\n");
	   return;
	}
	retcode = snda_ecs_get_object(handler, accesskey,
			secretkey, bucket, objectname, byterangeptr, snda_ecs_write_fun,
			writefd, region, ssl, (SNDAECSFollowLocation)followlocation, maxredirects, ret);
	fclose(writefd);
	snda_ecs_release_byte_range(byterangeptr);
	if (retcode != SNDA_ECS_SUCCESS) {
		printf("ClientErrorMessage:%s", ret->error->handlererrmsg);
	} else if (ret->serverresponse->httpcode >= 300) {
		SNDAECSErrorResponseContent* content = snda_ecs_to_error_response(ret);
		printf("Get Object failed and the http code is:%d\n",
				ret->serverresponse->httpcode);
		
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
		printf("Get Object success and the http code is:%d\n",
				ret->serverresponse->httpcode);
	}

	snda_ecs_release_handler(handler);
	snda_ecs_release_result(ret);
	snda_ecs_global_uninit();
}

int get_object() {
	printf("\n\n\n*************GET Object Example**************\n");
	char * accesskey = SNDA_ACCESSKEY;//;"your accessKey";
	char * secretkey = SNDA_ACCESS_SECRET;//"your secretKey";
	char * bucketname = SNDA_BUCKET_HUADONG;//"your bucketname";
	char * region = SDNA_REGION_HUADONG;//your region
	char * objectname = "test.sh";
	//char * localfile = "/tmp/get_object_test";
	char * localfile = "F:/test.sh";
	long byterangefirst = 0;
	long byterangelast = 50000;
	int followlocation = 0;
	int maxredirects = 0;
	int ssl = 0;

	get_object_example(accesskey, secretkey, bucketname, region, objectname,
			localfile, byterangefirst, byterangelast, ssl, followlocation,
			maxredirects);
	return 0;
}


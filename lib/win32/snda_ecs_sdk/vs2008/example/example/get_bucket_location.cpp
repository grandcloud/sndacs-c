#include "stdafx.h"
void get_bucket_location_example(const char* accesskey, const char* secretkey,
		const char* bucketname, int ssl) {
	SNDAECSHandler* handler;
	SNDAECSResult* ret;
	SNDAECSErrorCode retcode ;

	snda_ecs_global_init();
	handler = snda_ecs_init_handler();
	ret = snda_ecs_init_result();
	retcode = snda_ecs_get_bucket_location(handler, accesskey,
			secretkey, bucketname, ssl, ret);
	if (retcode != SNDA_ECS_SUCCESS) {
		printf("ClientErrorMessage:%s", ret->error->handlererrmsg);
	} else if (ret->serverresponse->httpcode >= 300) {
		SNDAECSErrorResponseContent* content = snda_ecs_to_error_response(ret);
		if(content){
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
	} else if (ret->serverresponse->httpcode == 200) {
		SNDAECSBucketLocation* location = snda_ecs_to_bucket_location(ret);
		printf("bucket:%s\n", bucketname);
		printf("location:%s\n", location->location);
		snda_ecs_release_bucket_location(location);
	} else {
		printf("Get bucket location success and the http code is:%d\n",
				ret->serverresponse->httpcode);
	}

	snda_ecs_release_handler(handler);
	snda_ecs_release_result(ret);
	snda_ecs_global_uninit();
}

int get_bucket_location() {
	printf("\n\n\n*************GET bucket location Example**************\n");
	char * accesskey = SNDA_ACCESSKEY;//;"your accessKey";
	char * secretkey = SNDA_ACCESS_SECRET;//"your secretKey";
	char * bucketname = SNDA_BUCKET_HUADONG;//"your bucketname";
	int ssl = 0;
	get_bucket_location_example(accesskey, secretkey, bucketname, ssl);
	return 0;
}


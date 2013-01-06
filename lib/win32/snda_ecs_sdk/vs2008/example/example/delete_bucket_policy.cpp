#include "stdafx.h"
void delete_bucket_policy_example(const char* accesskey, const char* secretkey,
		const char* bucketname, int ssl) {
	SNDAECSHandler* handler = 0;
	SNDAECSResult* ret = 0;
	SNDAECSErrorCode retcode;
	snda_ecs_global_init();
	handler = snda_ecs_init_handler();
	ret = snda_ecs_init_result();
	retcode = snda_ecs_delete_bucket_policy(handler,
			accesskey, secretkey, bucketname, ssl, ret);
	if (retcode != SNDA_ECS_SUCCESS) {
		printf("ClientErrorMessage:%s", ret->error->handlererrmsg);
	} else if (ret->serverresponse->httpcode >= 300) {
		
		SNDAECSErrorResponseContent* content = snda_ecs_to_error_response(ret);
		printf("HTTP Code:%d\n",ret->serverresponse->httpcode);
		printf("hello");
		if(content){
			printf("ErrorCode:%s\n", content->code);
			printf("ErrorMessage:%s\n", content->message);
			printf("Resource:%s\n", content->resource);
			printf("RequestId:%s\n", content->requestid);
			printf("AllErrorMessage:%s\n", content->fullbody);
			snda_ecs_release_error_response_content(content);
		}
	} else {
		printf("Delete bucket policy success and the http code is:%d\n",
				ret->serverresponse->httpcode);
	}

	snda_ecs_release_handler(handler);
	snda_ecs_release_result(ret);
	snda_ecs_global_uninit();
}

int delete_bucket_policy() {
	printf("\n\n\n*************DELETE bucket Policy Example**************\n");
	char * accesskey = SNDA_ACCESSKEY;//;"your accessKey";
	char * secretkey = SNDA_ACCESS_SECRET;//"your secretKey";
	char * bucketname = SNDA_BUCKET_HUADONG;//"your bucketname";
	int ssl = 0;
	
	delete_bucket_policy_example(accesskey, secretkey, bucketname, ssl);
	return 0;
}

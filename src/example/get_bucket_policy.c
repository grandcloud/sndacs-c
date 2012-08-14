#include "global.h"
void get_bucket_policy_example(const char* accesskey, const char* secretkey,
		const char* bucketname, int ssl) {
	SNDAECSHandler* handler = 0;
	SNDAECSResult* ret = 0;
	SNDAECSErrorCode retcode;
	SNDAECSErrorResponseContent* content = 0;

	snda_ecs_global_init();
	handler = snda_ecs_init_handler();
	ret = snda_ecs_init_result();
	retcode = snda_ecs_get_bucket_policy(handler, accesskey,
			secretkey, bucketname, ssl, ret);
	if (retcode != SNDA_ECS_SUCCESS) {
		printf("ClientErrorMessage:%s", ret->error->handlererrmsg);
	} else if (ret->serverresponse->httpcode >= 300) {
		content = snda_ecs_to_error_response(ret);
		if(content) {
		printf("Error,Http Code:%d\n",ret->serverresponse->httpcode);
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
		
		char* policy = (char*)malloc(ret->serverresponse->responsebody->retbodysize + 1);
		policy[ret->serverresponse->responsebody->retbodysize] = '\0';

		memcpy(policy,(char*)(ret->serverresponse->responsebody->retbody),
		ret->serverresponse->responsebody->retbodysize
		);
		printf ("bucket:%s\n", bucketname);
		printf ("policy:%s\n", policy);
		snda_ecs_free_char_ptr(policy);

	} else {
		printf("Get bucket policy success and the http code is:%d\n",
				ret->serverresponse->httpcode);
	}

	snda_ecs_release_handler(handler);
	snda_ecs_relase_result(ret);
	snda_ecs_global_uninit();
}

int main() {
	char * accesskey = SNDA_ACCESSKEY;//;"your accessKey";
	char * secretkey = SNDA_ACCESS_SECRET;//"your secretKey";
	char * bucketname = SNDA_BUCKET_HUADONG;//"your bucketname";
	int ssl = 0;
	get_bucket_policy_example(accesskey, secretkey, bucketname, ssl);
	return 0;
}


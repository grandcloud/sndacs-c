#include <string.h>
#include "global.h"

void put_bucket_policy_example(const char* accesskey, const char* secretkey,
		const char* bucketname, const char * policy, int ssl) {
	SNDAECSHandler* handler = 0;
	SNDAECSResult* ret = 0;
	SNDAECSErrorCode retcode ;
	SNDAECSErrorResponseContent* content = 0;

	snda_ecs_global_init();
	handler = snda_ecs_init_handler();
	ret = snda_ecs_init_result();
	retcode = snda_ecs_put_bucket_policy(handler, accesskey,
			secretkey, bucketname, policy, ssl, ret);
	if (retcode != SNDA_ECS_SUCCESS) {
		printf("ClientErrorMessage:%s", ret->error->handlererrmsg);
	} else if (ret->serverresponse->httpcode >= 300) {
		content = snda_ecs_to_error_response(ret);
		printf("ErrorCode:%s\n", content->code);
		printf("ErrorMessage:%s\n", content->message);
		printf("Resource:%s\n", content->resource);
		printf("RequestId:%s\n", content->requestid);
		printf("AllErrorMessage:%s\n", content->fullbody);
		snda_ecs_release_error_response_content(content);
	} else {
		printf("Put bucket policy success and the http code is:%d\n",
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
	char * policy = "{\"Id\":\"a03d0798-6bea-cc52-c6cb-a5b9c0aaa8e2\",\"Statement\":[{\"Sid\":\"ConsoleRefererWhiteList\",\"Effect\":\"Allow\",\"Action\":\"storage:GetObject\",\"Resource\":\"srn:snda:storage:::huadong_test_c/*\"}]}";
	int ssl = 0;
	put_bucket_policy_example(accesskey, secretkey, bucketname, policy, ssl);
	getchar();
	return 0;
}

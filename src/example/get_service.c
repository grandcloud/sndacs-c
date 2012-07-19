#include <string.h>

#include "../snda_ecs_sdk.h"
#include "../snda_ecs_http_util.h"
#include "../snda_ecs_constants.h"
#include "../snda_ecs_common_util.h"
void get_service_example(const char* accesskey, const char* secretkey,int ssl) {
	snda_ecs_global_init();
	SNDAECSHandler* handler = snda_ecs_init_handler();
	SNDAECSResult* ret = snda_ecs_init_result();
	SNDAECSErrorCode retcode = snda_ecs_get_service(handler, accesskey,
			secretkey, ssl, ret);
	if (retcode != SNDA_ECS_SUCCESS) {
		printf ("ClientErrorMessage:%s\n", ret->error->handlererrmsg);
	} else if (ret->serverresponse->httpcode == 200) {
		SNDAECSGetServiceResultContent* content =
				snda_ecs_to_get_service_result(ret);
		// show get service content
		for (; content; content = content->next) {
			printf ("BucketName:%s\n", content->bucketname);
			printf ("CreationTime:%s\n", content->creationtime);
		}
		// ALWAYS REMEMBER to release pointer resource by releated release method
		snda_ecs_release_get_service_result_content(content);
	} else {
		SNDAECSErrorResponseContent* content = snda_ecs_to_error_response(ret);
		printf ("ErrorCode:%s\n", content->code);
		printf ("ErrorMessage:%s\n", content->message);
		printf ("Resource:%s\n", content->resource);
		printf ("RequestId:%s\n", content->requestid);
		printf ("AllErrorMessage:%s\n", content->fullbody);
		snda_ecs_release_error_response_content(content);
	}

	snda_ecs_release_handler(handler);
	snda_ecs_relase_result(ret);
}

int main() {
	char * accesskey = "your accessKey";
	char * secretkey = "your secretKey";
	int ssl = 0;
	get_service_example(accesskey, secretkey, ssl);
	return 0;
}


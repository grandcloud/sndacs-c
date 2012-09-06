#include "global.h"
void delete_bucket_example(const char* accesskey, const char* secretkey,
		const char* bucketname, const char* region, int ssl,
		SNDAECSFollowLocation followlocation, long maxredirects) {
  
    SNDAECSHandler* handler = 0;
	SNDAECSResult* ret = 0;
	SNDAECSErrorCode retcode;
	SNDAECSErrorResponseContent* content = 0;
	snda_ecs_global_init();
	handler = snda_ecs_init_handler();
	ret = snda_ecs_init_result();
	retcode = snda_ecs_delete_bucket(handler, accesskey,
			secretkey, bucketname, region, ssl, followlocation, maxredirects,
			ret);
	if (retcode != SNDA_ECS_SUCCESS) {
		printf("ClientErrorMessage:%s", ret->error->handlererrmsg);
	} else if (ret->serverresponse->httpcode >= 300) {
		content = snda_ecs_to_error_response(ret);
		printf("HttpCode:%d\n",ret->serverresponse->httpcode);
		if(content) {
		printf("ErrorCode:%s\n", content->code);
		printf("ErrorMessage:%s\n", content->message);
		printf("Resource:%s\n", content->resource);
		printf("RequestId:%s\n", content->requestid);
		printf("AllErrorMessage:%s\n", content->fullbody);
		}

		snda_ecs_release_error_response_content(content);

		} else {
		printf("Delete bucket success and the http code is:%d\n",
				ret->serverresponse->httpcode);
	}

	snda_ecs_release_handler(handler);
	snda_ecs_release_result(ret);
	snda_ecs_global_uninit();
}

int main() {
	char * accesskey = SNDA_ACCESSKEY;//;"your accessKey";
	char * secretkey = SNDA_ACCESS_SECRET;//"your secretKey";
	char * bucketname = SNDA_BUCKET_HUADONG;//"your bucketname";
	char * region = SDNA_REGION_HUADONG;//your region
	int ssl = 0;
	SNDAECSFollowLocation followlocation = 1;
	long maxredirects = 1;
	delete_bucket_example(accesskey, secretkey, bucketname, region, ssl,
			followlocation, maxredirects);
	return 0;
}


#include <string.h>

#include "../snda_ecs_sdk.h"
#include "../snda_ecs_http_util.h"
#include "../snda_ecs_constants.h"
#include "../snda_ecs_common_util.h"
void put_object_example(const char* accesskey, const char* secretkey,
		const char* bucketname, const char *region, const char * localfile,
		const char *objectname, int ssl) {
	snda_ecs_global_init();
	SNDAECSHandler* handler = snda_ecs_init_handler();
	SNDAECSResult* ret = snda_ecs_init_result();
	SNDAECSUserObjectMeta* objectmeta = snda_ecs_init_user_object_meta();
	snda_ecs_set_object_type(objectmeta, "binary/octet-stream");
	// furthermore, user can set user metas with snda_ecs_add_object_user_metas()
	// all key of user metas must begin with "x-snda-meta-", and case insensitive
	snda_ecs_add_object_user_metas(objectmeta, "x-snda-meta-1",
			"this is my user meta 1");
	snda_ecs_add_object_user_metas(objectmeta, "x-SNDA-metA-2",
			"WOO, the seconde user meta");

	FILE* fd = fopen(localfile, "rb");
	fseek(fd, 0L, SEEK_END);
	long flength = ftell(fd);
	fseek(fd, 0, 0);
	SNDAECSErrorCode retcode = snda_ecs_put_object(handler, accesskey,
			secretkey, bucketname, objectname, snda_ecs_put_object_body, fd,
			flength, objectmeta, region, ssl, ret);
	snda_ecs_release_user_object_meta(objectmeta);
	if (retcode != SNDA_ECS_SUCCESS) {
		printf("ClientErrorMessage:%s", ret->error->handlererrmsg);
	} else if (ret->serverresponse->httpcode >= 300) {
		SNDAECSErrorResponseContent* content = snda_ecs_to_error_response(ret);
		printf("ErrorCode:%s\n", content->code);
		printf("ErrorMessage:%s\n", content->message);
		printf("Resource:%s\n", content->resource);
		printf("RequestId:%s\n", content->requestid);
		printf("AllErrorMessage:%s\n", content->fullbody);
		snda_ecs_release_error_response_content(content);
	} else {
		printf("Put Object success and the http code is:%d\n",
				ret->serverresponse->httpcode);
	}

	snda_ecs_release_handler(handler);
	snda_ecs_relase_result(ret);
}

int main() {
	char * accesskey = "your accesskey";
	char * secretkey = "your secretkey";
	char * bucketname = "your bucketname";
	int ssl = 0;
	char * region = "huabei-1";
	char * localfile = "test.sh";
	char * objectname = "test.sh";
	put_object_example(accesskey, secretkey, bucketname, region, localfile,
			objectname, ssl);
	return 0;
}


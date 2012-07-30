#include "global.h"
void copy_object_example(const char* accesskey, const char* secretkey,
		const char* destbucketname, const char * destobjectname,
		const char * srcbucketname, const char * srcobjectname,
		const char *region, int ssl) {

	SNDAECSHandler* handler = 0;
	SNDAECSResult* ret = 0;
	SNDAECSUserObjectMeta* objectmeta = 0;
	SNDAECSErrorCode retcode;

	snda_ecs_global_init();
	handler = snda_ecs_init_handler();
	ret = snda_ecs_init_result();
	objectmeta = snda_ecs_init_user_object_meta();
	snda_ecs_set_object_type(objectmeta, "binary/octet-stream");
	// furthermore, user can set user metas with snda_ecs_add_object_user_metas()
	// all key of user metas must begin with "x-snda-meta-", and case insensitive
	snda_ecs_add_object_user_metas(objectmeta, "x-snda-meta-1",
			"this is my user meta 1");
	snda_ecs_add_object_user_metas(objectmeta, "x-SNDA-metA-2",
			"WOO, the seconde user meta");

	retcode = snda_ecs_copy_object(handler, accesskey,
			secretkey, destbucketname, destobjectname, srcbucketname,
			srcobjectname, objectmeta, region, ssl, ret);
	snda_ecs_release_user_object_meta(objectmeta);
	if (retcode != SNDA_ECS_SUCCESS) {
		printf("ClientErrorMessage:%s", ret->error->handlererrmsg);
	} else if (ret->serverresponse->httpcode >= 300) {
		SNDAECSErrorResponseContent* content = snda_ecs_to_error_response(ret);
		if(content) {
		printf("ErrorCode:%s\n", content->code);
		printf("ErrorMessage:%s\n", content->message);
		printf("Resource:%s\n", content->resource);
		printf("RequestId:%s\n", content->requestid);
		printf("AllErrorMessage:%s\n", content->fullbody);
		}
		if(ret->serverresponse->httpcode == 505) {
		  printf("Please check your bucketname,accessKey,SecretAccessKey!\n");
		}
		snda_ecs_release_error_response_content(content);
	} else {
		printf("Copy Object success and the http code is:%d\n",
				ret->serverresponse->httpcode);
	}

	snda_ecs_release_handler(handler);
	snda_ecs_relase_result(ret);
}

int main() {
	char * accesskey = SNDA_ACCESSKEY;//;"your accessKey";
	char * secretkey = SNDA_ACCESS_SECRET;//"your secretKey";
	char * region = SDNA_REGION_HUADONG;//your region
	char * destobjectname = "test_copy.sh";
	char * destbucketname = SNDA_BUCKET_HUADONG;
	char * srcbucketname = SNDA_BUCKET_HUADONG;
	char * srcobjectname = "test.sh";
	int ssl = 0;

	copy_object_example(accesskey, secretkey, destbucketname, destobjectname, srcbucketname,
			 srcobjectname,region, ssl);
	return 0;
}


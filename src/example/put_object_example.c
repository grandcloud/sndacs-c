#include "global.h"

void put_object_example(const char* accesskey, const char* secretkey,
		const char* bucketname, const char *region, const char * localfile,
		const char *objectname, int ssl) {
	SNDAECSHandler* handler = 0;
	SNDAECSResult* ret = 0;
	SNDAECSUserObjectMeta* objectmeta = 0;
	FILE* fd = 0;
	long flength ;
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

	fd = fopen(localfile, "rb");
	if(!fd) {
		printf("Please check your file!\n");
		return ;
	}
	fseek(fd, 0L, SEEK_END);
	flength = ftell(fd);
	fseek(fd, 0, 0);
	retcode = snda_ecs_put_object(handler, accesskey,
			secretkey, bucketname, objectname, snda_ecs_put_object_body, fd,
			flength, objectmeta, region, ssl, ret);
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
		snda_ecs_release_error_response_content(content);
		}
		if(ret->serverresponse->httpcode == 505) {
		  printf("Please check your bucketname,accessKey,SecretAccessKey!\n");
		}
	} else {
		printf("Put Object success and the http code is:%d\n",
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
	char * region = SDNA_REGION_HUADONG;//your region
	int ssl = 0;
	char * localfile = LOCAL_SMALL_FILE_W;
	char * objectname = "test.sh";
	put_object_example(accesskey, secretkey, bucketname, region, localfile,
			objectname, ssl);
	return 0;
}


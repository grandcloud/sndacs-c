#include <string.h>

#include "../snda_ecs_sdk.h"
#include "../snda_ecs_http_util.h"
#include "../snda_ecs_constants.h"
#include "../snda_ecs_common_util.h"
void get_object_example(const char* accesskey, const char* secretkey,
		const char* bucket, const char *region, const char * objectname,
		const char * locafile, long byterangefirst, long byterangelast,
		int ssl, int followlocation, int maxredirects) {
	snda_ecs_global_init();
	SNDAECSHandler* handler = snda_ecs_init_handler();
	SNDAECSResult* ret = snda_ecs_init_result();

	SNDAECSByteRange* byterangeptr = 0;
	byterangeptr = snda_ecs_init_byte_range();
	byterangeptr->first = byterangefirst;
	byterangeptr->last = byterangelast;

	FILE* writefd = fopen(locafile, "wb");
	SNDAECSErrorCode retcode = snda_ecs_get_object(handler, accesskey,
			secretkey, bucket, objectname, byterangeptr, snda_ecs_write_fun,
			writefd, region, ssl, followlocation, maxredirects, ret);
	fclose(writefd);
	snda_ecs_release_byte_range(byterangeptr);
	if (retcode != SNDA_ECS_SUCCESS) {
		printf("ClientErrorMessage:%s", ret->error->handlererrmsg);
	} else if (ret->serverresponse->httpcode >= 300) {
		printf("Get Object failed and the http code is:%d\n",
				ret->serverresponse->httpcode);
		SNDAECSErrorResponseContent* content = snda_ecs_to_error_response(ret);
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
		snda_ecs_release_error_response_content(content);
	} else {
		printf("Get Object success and the http code is:%d\n",
				ret->serverresponse->httpcode);
	}

	snda_ecs_release_handler(handler);
	snda_ecs_relase_result(ret);
}

int main() {
	char * accesskey = "your accesskey";
	char * secretkey = "your secretkey";
	char * bucketname = "your bucketname";
	char * objectname = "your objectname";
	char * region = "huabei-1";
	char * localfile = "/tmp/get_object_test";
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


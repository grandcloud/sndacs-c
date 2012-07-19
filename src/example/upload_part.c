#include <string.h>

#include "../snda_ecs_sdk.h"
#include "../snda_ecs_http_util.h"
#include "../snda_ecs_constants.h"
#include "../snda_ecs_common_util.h"

void upload_part_example(const char* accesskey, const char* secretkey,
		const char* bucket, const char *region, const char * objectname,
		const char * uploadid, const char * localfile, int ssl, int partnumber) {
	snda_ecs_global_init();
	SNDAECSHandler* handler = snda_ecs_init_handler();
	SNDAECSResult* ret = snda_ecs_init_result();
	FILE* fd = fopen(localfile, "rb");
	fseek(fd, 0L, SEEK_END);
	long filelength = ftell(fd);
	fseek(fd, 0, 0);
	char * contentmd5 = 0;
	SNDAECSErrorCode retcode = snda_ecs_upload_part(handler, accesskey,
			secretkey, bucket, objectname, uploadid, partnumber,
			snda_ecs_put_object_body, fd, filelength, contentmd5, region, ssl,
			ret);

	if (retcode != SNDA_ECS_SUCCESS) {
		printf("ClientErrorMessage:%s", ret->error->handlererrmsg);
	} else if (ret->serverresponse->httpcode >= 300) {
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
		printf("Upload part success and the http code is %d\n",
				ret->serverresponse->httpcode);
	}
	snda_ecs_release_handler(handler);
	snda_ecs_relase_result(ret);
}

int main() {
	char * accesskey = "your accesskey";
	char * secretkey = "your secretkey";
	char * bucketname = "csdktest1";
	char * objectname = "albums.json";
	char * region = "huabei-1";
	char * uploadid = "your uploadid";
	char * localfile = "/tmp/test.sh";
	int followlocation = 0;
	int maxredirects = 0;
	int ssl = 0;
	int partnumber = 3;

	upload_part_example(accesskey, secretkey, bucketname, region, objectname,
			uploadid, localfile, ssl, partnumber);
	return 0;
}


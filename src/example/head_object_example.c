#include <string.h>

#include "../snda_ecs_sdk.h"
#include "../snda_ecs_http_util.h"
#include "../snda_ecs_constants.h"
#include "../snda_ecs_common_util.h"
void head_object_example(const char* accesskey, const char* secretkey,
		const char* bucketname, const char *region, const char * objectname,
		long byterangefirst, long byterangelast, int ssl, int followlocation,
		int maxredirects) {
	snda_ecs_global_init();
	SNDAECSHandler* handler = snda_ecs_init_handler();
	SNDAECSResult* ret = snda_ecs_init_result();

	SNDAECSByteRange* byterangeptr = 0;
	byterangeptr = snda_ecs_init_byte_range();
	byterangeptr->first = byterangefirst;
	byterangeptr->last = byterangelast;

	SNDAECSErrorCode retcode = snda_ecs_head_object(handler, accesskey,
			secretkey, bucketname, objectname, byterangeptr, region, ssl,
			followlocation, maxredirects, ret);
	snda_ecs_release_byte_range(byterangeptr);
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
		SNDAECSObjectMeta* objectmeta = snda_ecs_to_object_meta(ret);
		printf("Etag;%s\n", objectmeta->etag);
		printf("Content-Type:%s\n", objectmeta->contenttype);
		printf("Content-Length:%s\n", objectmeta->lastmodified);
		printf("Last-Modified:%s\n", objectmeta->lastmodified);
		SNDAECSKVList* p = objectmeta->usermetas;
		for (; p; p = p->next) {
			printf("p->key:%s\n", p->value);
		}

		snda_ecs_release_object_meta(objectmeta);
	}

	snda_ecs_release_handler(handler);
	snda_ecs_relase_result(ret);
}

int main() {
	char * accesskey = "your accesskey";
	char * secretkey = "your secretkey";
	char * bucketname = "your bucketname";
	char * objectname = "name of the object";
	char * region = "huabei-1";
	long byterangefirst = 0;
	long byterangelast = 50000;
	int followlocation = 0;
	int maxredirects = 0;
	int ssl = 0;

	head_object_example(accesskey, secretkey, bucketname, region, objectname,
			byterangefirst, byterangelast, ssl, followlocation, maxredirects);
	return 0;
}


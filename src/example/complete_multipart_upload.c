#include <string.h>

#include "../snda_ecs_sdk.h"
#include "../snda_ecs_http_util.h"
#include "../snda_ecs_constants.h"
#include "../snda_ecs_common_util.h"

void complete_multipart_upload_example(const char* accesskey,
		const char* secretkey, const char* bucket, const char *region,
		const char * objectname, const char * uploadid, int ssl,
		int followlocation, int maxdirects) {
	snda_ecs_global_init();
	SNDAECSHandler* handler = snda_ecs_init_handler();
	SNDAECSResult* ret = snda_ecs_init_result();
	SNDAECSMultipartsMeta* metas = snda_ecs_init_multiparts_meta();
	SNDAECSMultipartsMeta* p = metas;
	p->partnumber = 1;
	snda_ecs_copy_string(&(p->etag), "\"58fda622140205b3d6a2457415d301f2\"");
	p->next = snda_ecs_init_multiparts_meta();
	p = p->next;
	p->partnumber = 2;
	snda_ecs_copy_string(&(p->etag), "\"58fda622140205b3d6a2457415d301f2\"");
	p->next = snda_ecs_init_multiparts_meta();
	p = p->next;
	p->partnumber = 3;
	snda_ecs_copy_string(&(p->etag), "\"296e2bd0ce7124b6fbda05873c261dfb\"");

	SNDAECSErrorCode retcode = snda_ecs_complete_multipart_upload(handler,
			accesskey, secretkey, bucket, objectname, uploadid, metas, region,
			ssl, followlocation, maxdirects, ret);

	snda_ecs_release_multiparts_meta(metas);

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
		printf("Complete multipart upload success and the http code is %d\n",
				ret->serverresponse->httpcode);
	}
	snda_ecs_release_handler(handler);
	snda_ecs_relase_result(ret);
}

int main() {
	char * accesskey = "your accesskey";
	char * secretkey = "your secretkey";
	char * bucketname = "your bucketname";
	char * objectname = "albums.json";
	char * region = "huabei-1";
	char * uploadid = "uploadid";
	int followlocation = 0;
	int maxredirects = 0;
	int ssl = 0;
	complete_multipart_upload_example(accesskey, secretkey, bucketname, region,
			objectname, uploadid, ssl, followlocation, maxredirects);
	return 0;
}


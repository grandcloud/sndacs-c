
#include "global.h"

void list_parts_example(const char* accesskey, const char* secretkey,
		const char* bucket, const char *region, const char * objectname,
		const char * uploadid, int ssl, int followlocation,
		int partnumbermarker, int maxparts, int maxdirects) {

	SNDAECSHandler* handler = 0;
	SNDAECSResult* ret = 0;
	SNDAECSErrorCode retcode;

	snda_ecs_global_init();
	handler = snda_ecs_init_handler();
	ret = snda_ecs_init_result();
	retcode = snda_ecs_list_parts(handler, accesskey,
			secretkey, bucket, objectname, uploadid, partnumbermarker,
			maxparts, region, ssl, followlocation, maxdirects, ret);
	if (retcode != SNDA_ECS_SUCCESS) {
		printf("ClientErrorMessage:%s", ret->error->handlererrmsg);
	} else if (ret->serverresponse->httpcode < 300) {
		SNDAECSMultipartsContent* content = snda_ecs_to_multipart_parts(ret);
		SNDAECSMultipartsPart* part = 0;
		if (content) {
			printf("Bucket:%s\n", content->bucket);
			printf("Key:%s\n", content->key);
			printf("UploadId:%s\n", content->uploadid);
			printf("MaxParts:%d\n", content->maxparts);
			printf("IsTruncated:%d\n", content->istruncated);
			printf("PartNumberMarker:%d\n", content->partnumbermarker);
			printf("NextPartNumberMarker:%d\n", content->nextpartnumbermarker);

			printf("PARTS/\n");
			part = content->parts;
			while (part) {
				printf("\tPART/\n");
				printf("\t\tPartNumber:%d\n", part->partnumber);
				printf("\t\tSize:%ld\n", part->size);
				printf("\t\tLastModified:%s\n", part->lastmodified);
				printf("\t\tETag:%s\n", part->etag);
				part = part->next;
				printf("\t/PART\n");
			}
			printf("/PARTS\n");
		}
		snda_ecs_release_multiparts_content(content);
	} else {
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
		if(ret->serverresponse->httpcode == 505) {
		  printf("Please check your bucketname,accessKey,SecretAccessKey,uploadid!\n");
		}
		snda_ecs_release_error_response_content(content);
	}
	snda_ecs_release_handler(handler);
	snda_ecs_release_result(ret);
	snda_ecs_global_uninit();
}

int main() {
	char * accesskey = SNDA_ACCESSKEY;//;"your accessKey";
	char * secretkey = SNDA_ACCESS_SECRET;//"your secretKey";
	char * bucket = SNDA_BUCKET_HUADONG;//"your bucketname";
	char * region = SDNA_REGION_HUADONG;//your region
	char * objectname =MULTIPART_UPLOAD_OBJECT;
	char * uploadid = "5KR75PNM9NNAF8MVD4H1MXXDN";
	int followlocation = 0;
	int maxdirects = 0;
	int ssl = 0;
	int partnumbermarker = 0;
	int maxparts = 1000;

	list_parts_example(accesskey, secretkey, bucket, region, objectname,
			uploadid, ssl, followlocation, partnumbermarker, maxparts,
			maxdirects);
	return 0;
}


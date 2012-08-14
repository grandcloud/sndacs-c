#include "global.h"

void get_bucket_example(const char* accesskey, const char* secretkey,
		const char* bucketname, const char* prefix, const char* marker,
		const char* delimiter, int maxkeys, const char* region, int ssl,
		SNDAECSFollowLocation followlocation, long maxredirects) {

    SNDAECSHandler* handler = 0;
	SNDAECSResult* ret = 0;
	SNDAECSErrorCode retcode; 
	SNDAECSGetBucketResultContent* content = 0;

	snda_ecs_global_init();
	handler = snda_ecs_init_handler();
	ret = snda_ecs_init_result();
	retcode = snda_ecs_get_bucket(handler, accesskey,
			secretkey, bucketname, prefix, marker, delimiter, maxkeys, region,
			ssl, followlocation, maxredirects, ret);
	if (retcode != SNDA_ECS_SUCCESS) {
		printf("ClientErrorMessage:%s", ret->error->handlererrmsg);
	} else if (ret->serverresponse->httpcode == 200) {
	    content = snda_ecs_to_get_bucket_result(ret);
		if (content) {
			if (content->bucketname) {
				printf("bucket:%s\n", content->bucketname);
			}
			if (content->prefix) {
				printf("prefix:%s\n", content->prefix);
			}
			if (content->marker) {
				printf("marker:%s\n", content->marker);
			}
			if (content->delimiter) {
				printf("delimiter:%s\n", content->delimiter);
			}
			if (content->nextmarker) {
				printf("nextmarker:%s\n", content->nextmarker);
			}
			printf("maxkeys:%d\n", content->maxkeys);
			printf("istruncated:%s\n", content->istruncated ? "true" : "false");

			printf("CONTENTS/\n");
			if (content->objects) {
				SNDAECSObjectContent* object = content->objects;
				while (object) {
					printf("\tCONTENT/\n");
					printf("\t\tobjectname:%s\n", object->objectname);
					printf("\t\tlastmodified:%s\n", object->lastmodified);
					printf("\t\tetag:%s\n", object->etag);
					printf("\t\tsize:%ld\n", object->size);
					object = object->next;
					printf("\t/CONTENT\n");
				}
			}
			printf("/CONTENTS\n");

			printf("COMMONPREFIXES/\n");
			if (content->commonprefixes) {
				SNDAECSCommonPrefix* object = content->commonprefixes;
				while (object) {
					printf("\tCOMMONPREFIX/\n");
					printf("\t\tcommonprefix:%s\n", object->commonprefix);
					object = object->next;
					printf("\t/COMMONPREFIX\n");
				}
			}
			printf("/COMMONPREFIXES\n");
		}
		snda_ecs_release_get_bucket_result_content(content);
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
		printf("The http code is:%d\n", ret->serverresponse->httpcode);
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
	char * prefix = "";
	char * marker = "";
	char * delimiter = "/";
	int maxkeys = 10;
	SNDAECSFollowLocation followlocation = 1;
	int ssl = 0;
	long maxredirects = 1;
	get_bucket_example(accesskey, secretkey, bucketname, prefix, marker,
			delimiter, maxkeys, region, ssl, followlocation, maxredirects);
	return 0;
}


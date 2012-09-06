#include "global.h"
void upload_part_example(const char* accesskey, const char* secretkey,
		const char* bucket, const char *region, const char * objectname,
		const char * uploadid, const char * localfile, int ssl, int partnumber) {
	SNDAECSHandler* handler = 0;
	SNDAECSResult* ret = 0;
	FILE* fd = 0;
	long filelength;
	char * contentmd5 = 0;
	SNDAECSErrorCode retcode;
	
	snda_ecs_global_init();
	handler = snda_ecs_init_handler();
	ret = snda_ecs_init_result();
	fd = fopen(localfile, "rb");
	if(!fd) {
	   printf("please check your localfile path!\n");
	   return ;
	}
	fseek(fd, 0L, SEEK_END);
	filelength = ftell(fd);
	fseek(fd, 0, 0);
	
	retcode = snda_ecs_upload_part(handler, accesskey,
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
		
		if(ret->serverresponse->httpcode == 505) {
		  printf("Please check your bucketname,accessKey,SecretAccessKey,uploadid!\n");
		}
		snda_ecs_release_error_response_content(content);
	} else {
		printf("Upload part success and the http code is %d\n",
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
	char * localfile = LOCAL_SMALL_FILE_W;
	char * objectname =MULTIPART_UPLOAD_OBJECT;
	char * uploadid = "5KR75PNM9NNAF8MVD4H1MXXDN";
	int ssl = 0;
	int partnumber = 3;
   
	upload_part_example(accesskey, secretkey, bucketname, region, objectname,
			uploadid, localfile, ssl, partnumber);
	return 0;
}


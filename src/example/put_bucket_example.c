#include "global.h"

void put_bucket_example( const char* accesskey,const char* secretkey,const char* bucketname,const char* region, int ssl) {
	SNDAECSHandler* handler = 0;
    SNDAECSResult* ret = 0;
    SNDAECSErrorCode retcode;
    snda_ecs_global_init();
    handler = snda_ecs_init_handler();
    ret = snda_ecs_init_result();
    // when put bucket successfully, no value returned
    retcode = snda_ecs_put_bucket(handler, accesskey, secretkey, bucketname, region, ssl, ret);
    if (retcode != SNDA_ECS_SUCCESS) {
        printf("ClientErrorMessage:%s", ret->error->handlererrmsg);
    } else if (ret->serverresponse->httpcode >= 300){
        SNDAECSErrorResponseContent* content = snda_ecs_to_error_response(ret);
		if(content) {
        printf ("ErrorCode:%s\n", content->code);
        printf ("ErrorMessage:%s\n", content->message);
        printf ("Resource:%s\n", content->resource);
        printf ("RequestId:%s\n", content->requestid);
        printf ("AllErrorMessage:%s\n", content->fullbody);
        snda_ecs_release_error_response_content(content);
		}
		if(ret->serverresponse->httpcode == 505) {
		  printf("Please check your bucketname,accessKey,SecretAccessKey!\n");
		}
    } else {
        printf ("Put bucket success and the http code is:%d\n", ret->serverresponse->httpcode);
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
  put_bucket_example(accesskey,secretkey,bucketname,region,ssl);
  return 0;
}


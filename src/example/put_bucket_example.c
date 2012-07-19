#include <string.h>

#include "../snda_ecs_sdk.h"
#include "../snda_ecs_http_util.h"
#include "../snda_ecs_constants.h"
#include "../snda_ecs_common_util.h"
void put_bucket_example( const char* accesskey,const char* secretkey,
                         const char* bucketname,const char* region, int ssl) {
    snda_ecs_global_init();
    SNDAECSHandler* handler = snda_ecs_init_handler();
    SNDAECSResult* ret = snda_ecs_init_result();
    // when put bucket successfully, no value returned
    SNDAECSErrorCode retcode = snda_ecs_put_bucket(handler, accesskey, secretkey, bucketname, region, ssl, ret);
    if (retcode != SNDA_ECS_SUCCESS) {
        printf("ClientErrorMessage:%s", ret->error->handlererrmsg);
    } else if (ret->serverresponse->httpcode >= 300){
        SNDAECSErrorResponseContent* content = snda_ecs_to_error_response(ret);
        printf ("ErrorCode:%s\n", content->code);
        printf ("ErrorMessage:%s\n", content->message);
        printf ("Resource:%s\n", content->resource);
        printf ("RequestId:%s\n", content->requestid);
        printf ("AllErrorMessage:%s\n", content->fullbody);
        snda_ecs_release_error_response_content(content);
    } else {
        printf ("Put bucket success and the http code is:%d\n", ret->serverresponse->httpcode);
    }

    snda_ecs_release_handler(handler);
    snda_ecs_relase_result(ret);
}

int main() {
  char * accesskey = "your accesskey";
  char * secretkey = "your sercretkey";
  char * bucketname = "your bucketname";
  char * region = "your region";
  int ssl = 0;
  put_bucket_example(accesskey,secretkey,bucketname,region,ssl);
  return 0;
}


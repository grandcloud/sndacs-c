#include "global.h"

void initiate_multipart_upload_example( const char* accesskey,const char* secretkey,const char* bucket,
                            const char *region,const char * objectname,int ssl,
                            int followlocation,int maxredirects){
   
	SNDAECSHandler* handler = 0;
    SNDAECSResult* ret = 0;
    SNDAECSUserObjectMeta* objectmeta = 0;
	char contenttype[S_SNDA_ECS_CONTENT_TYPE_LEN];
	SNDAECSErrorCode retcode ;

	snda_ecs_global_init();
    handler = snda_ecs_init_handler();
    ret = snda_ecs_init_result();
    objectmeta = snda_ecs_init_user_object_meta();
   
    snda_ecs_set_object_type(objectmeta, snda_ecs_get_content_type(objectname, contenttype));
    retcode = snda_ecs_initiate_multipart_upload(handler, accesskey, secretkey, 
                        bucket, objectname,objectmeta, region, ssl, followlocation, maxredirects,ret);
    snda_ecs_release_user_object_meta(objectmeta);
    if (retcode != SNDA_ECS_SUCCESS) {
        printf("ClientErrorMessage:%s", ret->error->handlererrmsg);
    } else if (ret->serverresponse->httpcode >= 300){
        SNDAECSErrorResponseContent* content = snda_ecs_to_error_response(ret);
        if(content) {
            if(content->code) {
            printf ("ErrorCode:%s\n", content->code);
            }
            if(content->message) {
            printf ("ErrorMessage:%s\n", content->message);
            }
            if(content->resource) {
            printf ("Resource:%s\n", content->resource);
            }
            if(content->requestid) {
            printf ("RequestId:%s\n", content->requestid);
            }
            if(content->fullbody) {
            printf ("AllErrorMessage:%s\n", content->fullbody);
            }
        }
		if(ret->serverresponse->httpcode == 505) {
		  printf("Please check your bucketname,accessKey,SecretAccessKey!\n");
		}
        snda_ecs_release_error_response_content(content);
    } else {
       SNDAECSInitiateMultipartUploadResult* content = snda_ecs_to_initiate_multipart_upload_result(ret);
       printf ("Initiate multipart upload success and the http code is:%d\n", ret->serverresponse->httpcode);
	   if(content) {
            if(content->bucket) {
            printf ("Bucket:%s\n", content->bucket);
            } 
            if(content->key) {
            printf ("Key:%s\n", content->key);
            }
            if(content->uploadid) {
            printf ("UploadId:%s\n", content->uploadid);
            }
        }
        snda_ecs_release_initiate_multipart_upload_result(content);
    } 

    snda_ecs_release_handler(handler);
    snda_ecs_relase_result(ret);
}

int main() {
	char * accesskey = SNDA_ACCESSKEY;//;"your accessKey";
	char * secretkey = SNDA_ACCESS_SECRET;//"your secretKey";
	char * bucketname = SNDA_BUCKET_HUADONG;//"your bucketname";
	char * region = SDNA_REGION_HUADONG;//your region
	char * objectname = MULTIPART_UPLOAD_OBJECT;
	int followlocation = 0;
	int maxredirects = 0;
	int ssl = 0;
	initiate_multipart_upload_example( accesskey,secretkey,bucketname,region,objectname,ssl,followlocation,maxredirects);
	return 0;
}




#include "global.h"
void multipart_upload_copy_example(const char* accesskey, const char* secretkey,
		const char* destbucketname, const char * destobjectname,const char * uploadid,
		const int partnumber,const char * srcbucketname, const char * srcobjectname,
		const char *region, int ssl) {

	SNDAECSHandler* handler = 0;
	SNDAECSResult* ret = 0;
	SNDAECSUserObjectMeta* objectmeta = 0;
	SNDAECSErrorCode retcode;

	snda_ecs_global_init();
	handler = snda_ecs_init_handler();
	ret = snda_ecs_init_result();
	objectmeta = snda_ecs_init_user_object_meta();
    
	retcode = snda_ecs_upload_part_copy(handler,accesskey,secretkey, destbucketname,destobjectname,
		uploadid,partnumber,region,objectmeta,srcbucketname,srcobjectname, ssl,ret);
	snda_ecs_release_user_object_meta(objectmeta);
	if (retcode != SNDA_ECS_SUCCESS) {
		printf("ClientErrorMessage:%s", ret->error->handlererrmsg);
	} else if (ret->serverresponse->httpcode >= 300) {
		SNDAECSErrorResponseContent* content = snda_ecs_to_error_response(ret);
		printf("Error,HTTP code %d\n",ret->serverresponse->httpcode );
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
	   char * result = snda_ecs_get_full_response(ret);
	   
	   printf("Upload Part - copy success and the http code is:%d\n",
				ret->serverresponse->httpcode);
	   if(result){
	    printf("Response:\n%s\n",result);
	   }
	}

	snda_ecs_release_handler(handler);
	snda_ecs_release_result(ret);
	snda_ecs_global_uninit();
}

int main() {
	char * accesskey = SNDA_ACCESSKEY;//;"your accessKey";
	char * secretkey = SNDA_ACCESS_SECRET;//"your secretKey";
	char * region = SDNA_REGION_HUADONG;//your region
	char * destobjectname = "eclipse-jee-juno-linux-gtk-x86_64.tar.gz";
	char * destbucketname = SNDA_BUCKET_HUADONG;
	char * srcbucketname = SNDA_BUCKET_HUADONG;
	char * srcobjectname = "grand_cloud/logo.jpg";//"test.sh";
	char * uploadid = "BIJLXC82HR3HZP5L7ALBHOM33";
	int partnumber=2;
	int ssl = 0;

	multipart_upload_copy_example(accesskey, secretkey, destbucketname, destobjectname,uploadid,partnumber, srcbucketname,
			 srcobjectname,region, ssl);
	return 0;
}


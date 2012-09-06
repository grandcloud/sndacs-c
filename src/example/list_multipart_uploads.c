#include "global.h"
void list_multipart_uploads_example(const char* accesskey,
		const char* secretkey, const char* bucketname, const char* prefix,
		const char* keymarker, const char * uploadidmarker,
		const char* delimiter, int maxuploads, const char* region, int ssl,
		SNDAECSFollowLocation followlocation, long maxredirects) {

	SNDAECSHandler* handler = 0;
	SNDAECSResult* ret = 0;
	SNDAECSErrorCode retcode;

	snda_ecs_global_init();
	handler = snda_ecs_init_handler();
	ret = snda_ecs_init_result();

	retcode = snda_ecs_list_multipart_uploads(handler,
			accesskey, secretkey, bucketname, prefix, keymarker,
			uploadidmarker, delimiter, maxuploads, region, ssl, followlocation,
			maxredirects, ret);
	if (retcode != SNDA_ECS_SUCCESS) {
		printf("ClientErrorMessage:%s", ret->error->handlererrmsg);
	} else if (ret->serverresponse->httpcode < 300) {
		SNDAECSMultipartUploadsContent* content =
				snda_ecs_to_multipart_uploads_content(ret);
		if (content) {
			SNDAECSMultipartUpload* upload = 0;
			SNDAECSCommonPrefix* object = 0;
			printf("Bucket:%s\n", content->bucket);
			printf("Prefix:%s\n", content->prefix);
			printf("Delimiter:%s\n", content->delimiter);
			printf("KeyMarker:%s\n", content->keymarker);
			printf("UploadIdMarker:%s\n", content->uploadidmarker);
			printf("NextKeyMarker:%s\n", content->nextkeymarker);
			printf("NextUploadIdMarker:%s\n", content->nextuploadidmarker);
			printf("IsTruncated:%d\n", content->istruncated);
			printf("MaxUploads:%d\n", content->maxuploads);

			printf("UPLOADS/\n");
			upload = content->upload;
			while (upload) {
				printf("\tUPLOAD/\n");
				printf("\t\tKey:%s\n", upload->key);
				printf("\t\tUploadId:%s\n", upload->uploadid);
				printf("\t\tInitiatedTime:%s\n", upload->initiatedtime);
				upload = upload->next;
				printf("\t/UPLOAD\n");
			}
			printf("/UPLOADS\n");

			printf("COMMONPREFIXES/\n");
			object = content->commonprefixes;
			while (object) {
				printf("\tCOMMONPREFIX/\n");
				printf("\t\tPrefix:%s\n", object->commonprefix);
				object = object->next;
				printf("\tCOMMONPREFIX/\n");
			}
			printf("/COMMONPREFIXES\n");
		}
		snda_ecs_release_multipart_uploads_content(content);
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
		if(ret->serverresponse->httpcode == 505) {
		  printf("Please check your bucketname,accessKey,SecretAccessKey!\n");
		}
	} else {
		printf("The http code is:%d\n", ret->serverresponse->httpcode);
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
	char * prefix = "";
	char * keymarker = "";
	char * delimiter = "/";
	char * uploadidmarker = 0;
	int maxuploads = 10;
	SNDAECSFollowLocation followlocation = 1;
	int ssl = 0;
	long maxredirects = 1;
	list_multipart_uploads_example(accesskey, secretkey, bucketname, prefix,
			keymarker, uploadidmarker, delimiter, maxuploads, region, ssl,
			followlocation, maxredirects);
	return 0;
}


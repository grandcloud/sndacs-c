#if defined(WIN32)
#define _CRT_SECURE_NO_DEPRECATE
#endif

#include <string.h>

#include "snda_ecs_sdk/snda_ecs_sdk.h"
#include "snda_ecs_sdk/snda_ecs_http_util.h"
#include "snda_ecs_sdk/snda_ecs_constants.h"
#include "snda_ecs_sdk/snda_ecs_common_util.h"

static void snda_ecs_oneline(const char* msg) {
	printf("%s \n", msg);
}

static void snda_ecs_onelinevaluestring(const char* key, const char* value) {
	printf("%s : %s\n", key, value);
}

static void snda_ecs_onelinevalueint(const char* key, int value) {
	printf("%s : %d\n", key, value);
}

static void snda_ecs_onelinevaluelong(const char* key, long value) {
	printf("%s : %ld\n", key, value);
}
const char* snda_ecs_argument_retriever(int argc, char** args, const char* expectedkey) {
	int i = 1;
	for (; i < argc; ++i) {
		if (snda_ecs_is_start_with(args[i], expectedkey)) {
			int valuelen = strlen(args[i]) - strlen(expectedkey);

			char* value = calloc(valuelen + 1, sizeof(char));
			memcpy(value, args[i] + strlen(expectedkey), valuelen);
			// okay, here we will never release this resource, just like singleton
			return value;
		}
	}
	return 0;
}

static void showhelp() {
	snda_ecs_oneline("./snda_ecs_cmd ");
	snda_ecs_oneline("       --cmd=get_service");

	snda_ecs_oneline("       --cmd=put_bucket");
	snda_ecs_oneline("       --cmd=get_bucket");
	snda_ecs_oneline("       --cmd=delete_bucket");
	snda_ecs_oneline("       --cmd=put_bucket_policy");
	snda_ecs_oneline("       --cmd=get_bucket_policy");
	snda_ecs_oneline("       --cmd=delete_bucket_policy");
	snda_ecs_oneline("       --cmd=get_bucket_location");

	snda_ecs_oneline("       --cmd=put_object");
	snda_ecs_oneline("       --cmd=get_object");
	snda_ecs_oneline("       --cmd=head_object");
	snda_ecs_oneline("       --cmd=delete_object");
	snda_ecs_oneline("       --cmd=copy_object");

	snda_ecs_oneline("       --cmd=initiate_multipart_upload");
	snda_ecs_oneline("       --cmd=list_multipart_uploads");
	snda_ecs_oneline("       --cmd=abort_multipart_upload");
	snda_ecs_oneline("       --cmd=upload_part");
	snda_ecs_oneline("       --cmd=list_parts");

}

int main(int argc, char** args) {
	
    SNDAECSHandler* handler = 0;
    SNDAECSResult* ret = 0;
    SNDAECSErrorCode retcode = SNDA_ECS_ERROR;
    const char* accesskey = 0;
    const char* secretkey = 0;

    const char* cmd = snda_ecs_argument_retriever(argc, args, "--cmd=");
	if (!cmd) {
		showhelp();
		exit(-1);
	}

	// global init snda elastic cloud storage environment
	snda_ecs_global_init();

	handler = snda_ecs_init_handler();
	if (!handler) {
		snda_ecs_oneline("init snda ecs handler failed.");
		exit(-1);
	}

	// wrapper for result
	ret = snda_ecs_init_result();

	accesskey = snda_ecs_argument_retriever(argc, args, "--accesskey=");
	secretkey = snda_ecs_argument_retriever(argc, args, "--secretkey=");

	if (!strcmp(cmd, "get_service")) {
		const char* sslstr = snda_ecs_argument_retriever(argc, args, "--ssl=");
        int ssl = 0;

		if (!cmd || !accesskey || !secretkey || !sslstr) {
			snda_ecs_oneline("arguments: --cmd=get_service --accesskey= --secretkey= --ssl=(0/1)");
			exit(-1);
		}

		ssl = atoi(sslstr);
		retcode = snda_ecs_get_service(handler, accesskey, secretkey, ssl, ret);

		if (retcode == SNDA_ECS_SUCCESS && ret->serverresponse->httpcode == 200) {

			SNDAECSGetServiceResultContent* content = snda_ecs_to_get_service_result(ret);
			// show get service content
			for (; content; content = content->next) {
				snda_ecs_onelinevaluestring("BucketName", content->bucketname);
				snda_ecs_onelinevaluestring("CreationTime", content->creationtime);
			}

			// ALWAYS REMEMBER to release pointer resource by releated release method
			snda_ecs_release_get_service_result_content(content);
		}

	} else if (!strcmp(cmd, "put_bucket")) {
		const char* bucketname = snda_ecs_argument_retriever(argc, args, "--bucket=");
		const char* region = snda_ecs_argument_retriever(argc, args, "--region=");
		const char* sslstr = snda_ecs_argument_retriever(argc, args, "--ssl=");
        int ssl  = 0;

		if (!cmd || !accesskey || !secretkey || !bucketname || !region || !sslstr) {
			snda_ecs_oneline("arguments: --cmd=put_bucket --accesskey= --secretkey= --bucket= --region=(huadong-1/huabei-1) --ssl=(0/1)");
			exit(-1);
		}

		ssl = atoi(sslstr);
		// when put bucket successfully, no value returned
		retcode = snda_ecs_put_bucket(handler, accesskey, secretkey, bucketname, region, ssl, ret);

	} else if (!strcmp(cmd, "put_bucket_policy")) {
		const char* bucket = snda_ecs_argument_retriever(argc, args, "--bucket=");
		const char* policy = snda_ecs_argument_retriever(argc, args, "--policy=");
		const char* sslstr = snda_ecs_argument_retriever(argc, args, "--ssl=");
        int ssl = 0;
		if (!cmd || !accesskey || !secretkey || !bucket || !policy || !sslstr) {
			snda_ecs_oneline("arguments: --cmd=put_bucket_policy --accesskey= --secretkey= --bucket= --policy= --ssl=(0/1)");
			exit(-1);
		}
		ssl = atoi(sslstr);

		retcode = snda_ecs_put_bucket_policy(handler, accesskey, secretkey, bucket, policy, ssl, ret);
	} else if (!strcmp(cmd, "get_bucket_policy")) {
		const char* bucketname = snda_ecs_argument_retriever(argc, args, "--bucket=");
		const char* sslstr = snda_ecs_argument_retriever(argc, args, "--ssl=");
        int ssl = 0;
		if (!cmd || !accesskey || !secretkey || !bucketname || !sslstr) {
			snda_ecs_oneline("arguments: --cmd=get_bucket_policy --accesskey= --secretkey= --bucket= --ssl=(0/1)");
			exit(-1);
		}
		ssl = atoi(sslstr);

		retcode = snda_ecs_get_bucket_policy(handler, accesskey, secretkey, bucketname, ssl, ret);

		if (retcode == SNDA_ECS_SUCCESS && ret->serverresponse->httpcode == 200) {
			char* policy = (char*)malloc(ret->serverresponse->responsebody->retbodysize + 1);
			policy[ret->serverresponse->responsebody->retbodysize] = '\0';

			memcpy(policy,
				(char*)(ret->serverresponse->responsebody->retbody),
				ret->serverresponse->responsebody->retbodysize
			);
			snda_ecs_onelinevaluestring("bucket", bucketname);
			snda_ecs_onelinevaluestring("policy", policy);
            snda_ecs_free_char_ptr(policy);
		}
	} else if (!strcmp(cmd, "delete_bucket_policy")) {
		const char* bucketname = snda_ecs_argument_retriever(argc, args, "--bucket=");
		const char* sslstr = snda_ecs_argument_retriever(argc, args, "--ssl=");
        int ssl = 0;
		if (!cmd || !accesskey || !secretkey || !bucketname || !sslstr) {
			snda_ecs_oneline("arguments: --cmd=delete_bucket_policy --accesskey= --secretkey= --bucket= --ssl=(0/1)");
			exit(-1);
		}
		ssl = atoi(sslstr);

		retcode = snda_ecs_delete_bucket_policy(handler, accesskey, secretkey, bucketname, ssl, ret);

	} else if (!strcmp(cmd, "get_bucket_location")) {
		const char* bucketname = snda_ecs_argument_retriever(argc, args, "--bucket=");
		const char* sslstr = snda_ecs_argument_retriever(argc, args, "--ssl=");
        int ssl = 0;
		if (!cmd || !accesskey || !secretkey || !bucketname || !sslstr) {
			snda_ecs_oneline("arguments: --cmd=get_bucket_location --accesskey= --secretkey= --bucket= --ssl=(0/1)");
			exit(-1);
		}
		ssl = atoi(sslstr);

		retcode = snda_ecs_get_bucket_location(handler, accesskey, secretkey, bucketname, ssl, ret);

		if (retcode == SNDA_ECS_SUCCESS && ret->serverresponse->httpcode == 200) {
			SNDAECSBucketLocation* location = snda_ecs_to_bucket_location(ret);

			snda_ecs_onelinevaluestring("bucket", bucketname);
			snda_ecs_onelinevaluestring("location", location->location);

			snda_ecs_release_bucket_location(location);
		}
	} else if (!strcmp(cmd, "delete_bucket")) {
		const char* bucketname = snda_ecs_argument_retriever(argc, args, "--bucket=");
		const char* region = snda_ecs_argument_retriever(argc, args, "--region=");
		const char* sslstr = snda_ecs_argument_retriever(argc, args, "--ssl=");
		const char* followlocationstr = snda_ecs_argument_retriever(argc, args, "--followlocation=");
		const char* maxredirectsstr = snda_ecs_argument_retriever(argc, args, "--maxredirects=");
        int ssl = 0;
        int followlocation = 0;
		long maxredirects = 0;

		if (!cmd || !accesskey || !secretkey || !bucketname || !region || !sslstr || !followlocationstr || !maxredirectsstr) {
			snda_ecs_oneline("arguments: --cmd=delete_bucket --accesskey= --secretkey= --bucket= --region= --ssl=(0/1) --followlocation=(0/1) --maxredirects=");
			exit(-1);
		}
		ssl = atoi(sslstr);
		followlocation = atoi(followlocationstr);
		maxredirects = atol(maxredirectsstr);
		retcode = snda_ecs_delete_bucket(handler, accesskey, secretkey, bucketname, region, ssl, followlocation, maxredirects, ret);

	} else if (!strcmp(cmd, "get_bucket")) {
		const char* bucketname = snda_ecs_argument_retriever(argc, args, "--bucket=");
		const char* prefix = snda_ecs_argument_retriever(argc, args, "--prefix=");
		const char* marker = snda_ecs_argument_retriever(argc, args, "--marker=");
		const char* delimiter = snda_ecs_argument_retriever(argc, args, "--delimiter=");
		const char* maxkeysstr = snda_ecs_argument_retriever(argc, args, "--maxkeys=");
		const char* region = snda_ecs_argument_retriever(argc, args, "--region=");
		const char* sslstr = snda_ecs_argument_retriever(argc, args, "--ssl=");
		const char* followlocationstr = snda_ecs_argument_retriever(argc, args, "--followlocation=");
		const char* maxredirectsstr = snda_ecs_argument_retriever(argc, args, "--maxredirects=");
        int ssl = 0;
        int followlocation = 0;
		long maxredirects = 0;
        int maxkeys = 0;

		if (!cmd || !accesskey || !secretkey || !bucketname || !region || !sslstr || !followlocationstr || !maxredirectsstr) {
			snda_ecs_oneline("arguments: --cmd=get_bucket --accesskey= --secretkey= --bucket= --region= --ssl=(0/1) --followlocation=(0/1) --maxredirects= [--prefix= --marker= -- delimiter= --maxkeys=]");
			exit(-1);
		}
		maxkeys = maxkeysstr == 0 ? 1000 : atoi(maxkeysstr);
		ssl = atoi(sslstr);
		followlocation = atoi(followlocationstr);
		maxredirects = atol(maxredirectsstr);

		snda_ecs_onelinevaluestring("prefix", prefix);
		snda_ecs_onelinevaluestring("marker", marker);

		retcode = snda_ecs_get_bucket(handler, accesskey, secretkey, bucketname, prefix, marker, delimiter, maxkeys, region, ssl, followlocation, maxredirects, ret);

		if (retcode == SNDA_ECS_SUCCESS && ret->serverresponse->httpcode == 200) {
			SNDAECSGetBucketResultContent* content = snda_ecs_to_get_bucket_result(ret);
			if (content) {
				if (content->bucketname) {
					snda_ecs_onelinevaluestring("bucket", content->bucketname);
				}
				if (content->prefix) {
					snda_ecs_onelinevaluestring("prefix", content->prefix);
				}
				if (content->marker) {
					snda_ecs_onelinevaluestring("marker", content->marker);
				}
				if (content->delimiter) {
					snda_ecs_onelinevaluestring("delimiter", content->delimiter);
				}
				if (content->nextmarker) {
					snda_ecs_onelinevaluestring("nextmarker", content->nextmarker);
				}
				snda_ecs_onelinevalueint("maxkeys", content->maxkeys);
				snda_ecs_onelinevaluestring("istruncated", content->istruncated ? "true" : "false");

				snda_ecs_oneline("CONTENTS/");
				if (content->objects) {
					SNDAECSObjectContent* object = content->objects;
					while (object) {
						snda_ecs_oneline("\tCONTENT/");
						snda_ecs_onelinevaluestring("\t\tobjectname", object->objectname);
						snda_ecs_onelinevaluestring("\t\tlastmodified", object->lastmodified);
						snda_ecs_onelinevaluestring("\t\tetag", object->etag);
						snda_ecs_onelinevaluelong("\t\tsize", object->size);
						object = object->next;
						snda_ecs_oneline("\t/CONTENT");
					}
				}
				snda_ecs_oneline("/CONTENTS");

				snda_ecs_oneline("COMMONPREFIXES/");
				if (content->commonprefixes) {
					SNDAECSCommonPrefix* object = content->commonprefixes;
					while (object) {
						snda_ecs_oneline("\tCOMMONPREFIX/");
						snda_ecs_onelinevaluestring("\t\tcommonprefix", object->commonprefix);
						object = object->next;
						snda_ecs_oneline("\t/COMMONPREFIX");
					}
				}
				snda_ecs_oneline("/COMMONPREFIXES");
			}
			snda_ecs_release_get_bucket_result_content(content);
		}
	} else if (!strcmp(cmd, "put_object")) {
		/**
		 * this sample simulate putting an object to snda elastic cloud storage from local file
		 */
		const char* bucket = snda_ecs_argument_retriever(argc, args, "--bucket=");
		const char* objectname = snda_ecs_argument_retriever(argc, args, "--key=");
		const char* localfile = snda_ecs_argument_retriever(argc, args, "--localfile=");
		const char* filelengthstr = snda_ecs_argument_retriever(argc, args, "--filelength=");
		const char* contenttype = snda_ecs_argument_retriever(argc, args, "--contenttype=");
		const char* region = snda_ecs_argument_retriever(argc, args, "--region=");
		const char* sslstr = snda_ecs_argument_retriever(argc, args, "--ssl=");
        int ssl = 0;
        SNDAECSUserObjectMeta* objectmeta = 0;
        FILE* fd = 0;

		if (!cmd || !accesskey || !secretkey || !bucket || !objectname || !filelengthstr || !localfile || !region || !sslstr) {
			snda_ecs_oneline("arguments: --cmd=put_object --accesskey= --secretkey= --bucket=  --filelength= --key= --localfile= --region= --ssl=(0/1) [--contenttype= ]");
			exit(-1);
		}
		ssl = atoi(sslstr);

		objectmeta = snda_ecs_init_user_object_meta();
		if (contenttype) {
			snda_ecs_set_object_type(objectmeta, contenttype);
		}
		// furthermore, user can set user metas with snda_ecs_add_object_user_metas()
		// all key of user metas must begin with "x-snda-meta-", and case insensitive
		snda_ecs_add_object_user_metas(objectmeta, "x-snda-meta-1", "this is my user meta 1");
		snda_ecs_add_object_user_metas(objectmeta, "x-SNDA-metA-2", "WOO, the seconde user meta");

		fd = fopen(localfile, "rb");
		retcode = snda_ecs_put_object(handler, accesskey, secretkey, bucket, objectname,
				snda_ecs_put_object_body, fd, atol(filelengthstr),
				objectmeta, region, ssl,
				ret);
        fclose(fd);
		// release object meta
		snda_ecs_release_user_object_meta(objectmeta);

	} else if (!strcmp(cmd, "delete_object")) {
		const char* bucket = snda_ecs_argument_retriever(argc, args, "--bucket=");
		const char* objectname = snda_ecs_argument_retriever(argc, args, "--key=");
		const char* region = snda_ecs_argument_retriever(argc, args, "--region=");
		const char* sslstr = snda_ecs_argument_retriever(argc, args, "--ssl=");
		const char* followlocationstr = snda_ecs_argument_retriever(argc, args, "--followlocation=");
		const char* maxredirectsstr = snda_ecs_argument_retriever(argc, args, "--maxredirects=");
        int ssl = 0;
        int followlocation = 0;
		long maxredirects = 0;

		if (!cmd || !accesskey || !secretkey || !bucket || !objectname || !region || !sslstr || !followlocationstr || !maxredirectsstr ) {
			snda_ecs_oneline("arguments: --cmd=delete_object --accesskey= --secretkey= --bucket= --key= --region= --ssl=(0/1) --followlocation=(0/1) --maxredirects= ");
			exit(-1);
		}
		ssl = atoi(sslstr);
		followlocation = atoi(followlocationstr);
		maxredirects = atol(maxredirectsstr);

		retcode = snda_ecs_delete_object(handler, accesskey, secretkey, bucket, objectname, region , ssl, followlocation, maxredirects, ret);
	} else if (!strcmp(cmd, "get_object")) {
		/**
		 * this sample simulate to get an object from snda elastic cloud storage to localfile
		 */
		const char* bucket = snda_ecs_argument_retriever(argc, args, "--bucket=");
		const char* objectname = snda_ecs_argument_retriever(argc, args, "--key=");
		const char* locafile = snda_ecs_argument_retriever(argc, args, "--localfile=");
		const char* byterangefirst = snda_ecs_argument_retriever(argc, args, "--byterangefirst=");
		const char* byterangelast = snda_ecs_argument_retriever(argc, args, "--byterangelast=");
		const char* region = snda_ecs_argument_retriever(argc, args, "--region=");
		const char* sslstr = snda_ecs_argument_retriever(argc, args, "--ssl=");
		const char* followlocationstr = snda_ecs_argument_retriever(argc, args, "--followlocation=");
		const char* maxredirectsstr = snda_ecs_argument_retriever(argc, args, "--maxredirects=");
        int ssl = 0;
        int followlocation = 0;
		long maxredirects = 0;
        SNDAECSByteRange* byterangeptr = 0;
        FILE* writefd = 0;

		if (!cmd || !accesskey || !secretkey || !bucket || !objectname || !locafile || !region || !sslstr || !followlocationstr || !maxredirectsstr ) {
			snda_ecs_oneline("arguments: --cmd=get_object --accesskey= --secretkey= --bucket= --key= --localfile= --region= --ssl=(0/1) --followlocation=(0/1) --maxredirects= [--byterangefirst= --byterangelast=]");
			exit(-1);
		}
		ssl = atoi(sslstr);
		followlocation = atoi(followlocationstr);
		maxredirects = atol(maxredirectsstr);

		if (byterangefirst && byterangelast) {
			byterangeptr = snda_ecs_init_byte_range();
			byterangeptr->first = atol(byterangefirst);
			byterangeptr->last = atol(byterangelast);
		}

		writefd = fopen(locafile, "wb");

		retcode = snda_ecs_get_object(handler, accesskey, secretkey, bucket, objectname, byterangeptr,
				snda_ecs_write_fun, writefd, region, ssl,  followlocation, maxredirects,
				ret);

		fclose(writefd);

		snda_ecs_release_byte_range(byterangeptr);
	} else if (!strcmp(cmd, "head_object")) {
		const char* bucket = snda_ecs_argument_retriever(argc, args, "--bucket=");
		const char* objectname = snda_ecs_argument_retriever(argc, args, "--key=");
		const char* byterangefirst = snda_ecs_argument_retriever(argc, args, "--byterangefirst=");
		const char* byterangelast = snda_ecs_argument_retriever(argc, args, "--byterangelast=");
		const char* region = snda_ecs_argument_retriever(argc, args, "--region=");
		const char* sslstr = snda_ecs_argument_retriever(argc, args, "--ssl=");
		const char* followlocationstr = snda_ecs_argument_retriever(argc, args, "--followlocation=");
		const char* maxredirectsstr = snda_ecs_argument_retriever(argc, args, "--maxredirects=");
        int ssl = 0;
        int followlocation = 0;
		long maxredirects = 0;
        SNDAECSByteRange* byterangeptr = 0;

		if (!cmd || !accesskey || !secretkey || !bucket || !objectname || !region || !sslstr || !followlocationstr || !maxredirectsstr ) {
			snda_ecs_oneline("arguments: --cmd=head_object --accesskey= --secretkey= --bucket= --key= --region= --ssl=(0/1) --followlocation=(0/1) --maxredirects= [--byterangefirst= --byterangelast=]");
			exit(-1);
		}
		ssl = atoi(sslstr);
		followlocation = atoi(followlocationstr);
		maxredirects = atol(maxredirectsstr);

		if (byterangefirst && byterangelast) {
			byterangeptr = snda_ecs_init_byte_range();
			byterangeptr->first = atol(byterangefirst);
			byterangeptr->last = atol(byterangelast);
		}

		retcode = snda_ecs_head_object(handler, accesskey, secretkey, bucket, objectname, byterangeptr,
				region, ssl, followlocation, maxredirects,
				ret);

		snda_ecs_release_byte_range(byterangeptr);

		if (retcode == SNDA_ECS_SUCCESS && ret->serverresponse->httpcode < 300) {
			SNDAECSObjectMeta* objectmeta = snda_ecs_to_object_meta(ret);
            SNDAECSKVList* p = objectmeta->usermetas;

			snda_ecs_onelinevaluestring("Etag", objectmeta->etag);
			snda_ecs_onelinevaluestring("Content-Type", objectmeta->contenttype);
			snda_ecs_onelinevaluelong("Content-Length", objectmeta->contentlength);
			snda_ecs_onelinevaluestring("Last-Modified", objectmeta->lastmodified);

            for (; p; p = p->next) {
				snda_ecs_onelinevaluestring("p->key", p->value);
			}

			snda_ecs_release_object_meta(objectmeta);
		}
	} else if(!strcmp(cmd,"copy_object")){
		const char* destbucketname = snda_ecs_argument_retriever(argc, args, "--destbucket=");
		const char* destobjectname = snda_ecs_argument_retriever(argc, args, "--destkey=");
		const char* srcbucketname = snda_ecs_argument_retriever(argc, args, "--srcbucket=");
		const char* srcobjectname = snda_ecs_argument_retriever(argc, args, "--srckey=");
		const char* region = snda_ecs_argument_retriever(argc, args, "--region=");
		const char* sslstr = snda_ecs_argument_retriever(argc, args, "--ssl=");
        int ssl = 0;
        SNDAECSUserObjectMeta* objectmeta = 0;

		if (!cmd || !accesskey || !secretkey || !destbucketname || !destobjectname || !region || !sslstr || !srcbucketname || !srcobjectname ) {
					snda_ecs_oneline("arguments: --cmd=copy_object --accesskey= --secretkey= --destbucket= --destkey= --region= --ssl=(0/1) --srcbucket= --srckey= ");
					exit(-1);
		}
		objectmeta = snda_ecs_init_user_object_meta();
		snda_ecs_set_object_type(objectmeta, "binary/octet-stream");
		// furthermore, user can set user metas with snda_ecs_add_object_user_metas()
		// all key of user metas must begin with "x-snda-meta-", and case insensitive
		snda_ecs_add_object_user_metas(objectmeta, "x-snda-meta-1",
				"this is my user meta 1");
		snda_ecs_add_object_user_metas(objectmeta, "x-SNDA-metA-2",
				"WOO, the seconde user meta");
		ssl = atoi(sslstr);
		retcode = snda_ecs_copy_object(handler, accesskey,
				secretkey, destbucketname, destobjectname, region, srcbucketname,
				srcobjectname, objectmeta, ssl, ret);
		snda_ecs_release_user_object_meta(objectmeta);
	} else if (!strcmp(cmd, "initiate_multipart_upload")) {
		const char* bucket = snda_ecs_argument_retriever(argc, args, "--bucket=");
		const char* objectname = snda_ecs_argument_retriever(argc, args, "--key=");
		const char* contenttype = snda_ecs_argument_retriever(argc, args, "--contenttype=");
		const char* region = snda_ecs_argument_retriever(argc, args, "--region=");
		const char* sslstr = snda_ecs_argument_retriever(argc, args, "--ssl=");
		const char* followlocationstr = snda_ecs_argument_retriever(argc, args, "--followlocation=");
		const char* maxredirectsstr = snda_ecs_argument_retriever(argc, args, "--maxredirects=");
        int ssl = 0;
        int followlocation = 0;
		long maxredirects = 0;
        SNDAECSUserObjectMeta* objectmeta = 0;

		if (!cmd || !accesskey || !secretkey || !bucket || !objectname || !region || !sslstr || !followlocationstr || !maxredirectsstr ) {
			snda_ecs_oneline("arguments: --cmd=initiate_multipart_upload --accesskey= --secretkey= --bucket= --key= --region= --ssl=(0/1) --followlocation=(0/1) --maxredirects= [--contenttype=]");
			exit(-1);
		}
		ssl = atoi(sslstr);
		followlocation = atoi(followlocationstr);
		maxredirects = atol(maxredirectsstr);

		objectmeta = snda_ecs_init_user_object_meta();
		if (contenttype) {
			snda_ecs_set_object_type(objectmeta, contenttype);
		} else {
			char contenttype[S_SNDA_ECS_CONTENT_TYPE_LEN];
			snda_ecs_set_object_type(objectmeta, snda_ecs_get_content_type(objectname, contenttype));
		}

		retcode = snda_ecs_initiate_multipart_upload(handler, accesskey, secretkey, bucket, objectname,
				objectmeta, region, ssl, followlocation, maxredirects,
				ret);
		snda_ecs_release_user_object_meta(objectmeta);

		if (retcode == SNDA_ECS_SUCCESS) {
			SNDAECSInitiateMultipartUploadResult* content = snda_ecs_to_initiate_multipart_upload_result(ret);
			snda_ecs_onelinevaluestring("Bucket", content->bucket);
			snda_ecs_onelinevaluestring("Key", content->key);
			snda_ecs_onelinevaluestring("UploadId", content->uploadid);
			snda_ecs_release_initiate_multipart_upload_result(content);
		}
	} else if (!strcmp(cmd, "list_multipart_uploads")) {
		const char* bucketname = snda_ecs_argument_retriever(argc, args, "--bucket=");
		const char* prefix = snda_ecs_argument_retriever(argc, args, "--prefix=");
		const char* keymarker = snda_ecs_argument_retriever(argc, args, "--keymarker=");
		const char* uploadidmarker = snda_ecs_argument_retriever(argc, args, "--uploadidmarker=");
		const char* delimiter = snda_ecs_argument_retriever(argc, args, "--delimiter=");
		const char* maxuploadsstr = snda_ecs_argument_retriever(argc, args, "--maxuploads=");
		const char* region = snda_ecs_argument_retriever(argc, args, "--region=");
		const char* sslstr = snda_ecs_argument_retriever(argc, args, "--ssl=");
		const char* followlocationstr = snda_ecs_argument_retriever(argc, args, "--followlocation=");
		const char* maxredirectsstr = snda_ecs_argument_retriever(argc, args, "--maxredirects=");
        int maxuploads = 0;
		int ssl = 0;
		int followlocation = 0;
		long maxdirects = 0;

		if (!cmd || !accesskey || !secretkey || !bucketname || !region || !sslstr || !followlocationstr || !maxredirectsstr ) {
			snda_ecs_oneline("arguments: --cmd=list_multipart_uploads --accesskey= --secretkey= --bucket= --region= --ssl=(0/1) --followlocation=(0/1) --maxredirects= [--prefix= --keymarker= --uploadidmarker=  --delimiter= --maxuploads=]");
			exit(-1);
		}

		maxuploads = maxuploadsstr == 0 ? 1000 : atoi(maxuploadsstr);
		ssl = atoi(sslstr);
		followlocation = atoi(followlocationstr);
		maxdirects = atol(maxredirectsstr);

		retcode = snda_ecs_list_multipart_uploads(handler, accesskey, secretkey, bucketname, prefix, keymarker, uploadidmarker, delimiter, maxuploads, region, ssl, followlocation, maxdirects, ret);
		if (retcode == SNDA_ECS_SUCCESS) {
			SNDAECSMultipartUploadsContent* content = snda_ecs_to_multipart_uploads_content(ret);
			if (content) {
				SNDAECSMultipartUpload* upload = content->upload;
                SNDAECSCommonPrefix* object = content->commonprefixes;

                snda_ecs_onelinevaluestring("Bucket", content->bucket);
				snda_ecs_onelinevaluestring("Prefix", content->prefix);
				snda_ecs_onelinevaluestring("Delimiter", content->delimiter);
				snda_ecs_onelinevaluestring("KeyMarker", content->keymarker);
				snda_ecs_onelinevaluestring("UploadIdMarker", content->uploadidmarker);
				snda_ecs_onelinevaluestring("NextKeyMarker", content->nextkeymarker);
				snda_ecs_onelinevaluestring("NextUploadIdMarker", content->nextuploadidmarker);
				snda_ecs_onelinevalueint("IsTruncated", content->istruncated);
				snda_ecs_onelinevalueint("MaxUploads", content->maxuploads);

				snda_ecs_oneline("UPLOADS/");
				while (upload) {
					snda_ecs_oneline("\tUPLOAD/");
					snda_ecs_onelinevaluestring("\t\tKey", upload->key);
					snda_ecs_onelinevaluestring("\t\tUploadId", upload->uploadid);
					snda_ecs_onelinevaluestring("\t\tInitiatedTime", upload->initiatedtime);
					upload = upload->next;
					snda_ecs_oneline("\t/UPLOAD");
				}
				snda_ecs_oneline("/UPLOADS");

				snda_ecs_oneline("COMMONPREFIXES/");
				while (object) {
					snda_ecs_oneline("\tCOMMONPREFIX/");
					snda_ecs_onelinevaluestring("\t\tPrefix", object->commonprefix);
					object = object->next;
					snda_ecs_oneline("\tCOMMONPREFIX/");
				}
				snda_ecs_oneline("/COMMONPREFIXES");
			}
			snda_ecs_release_multipart_uploads_content(content);
		}
	} else if (!strcmp(cmd, "abort_multipart_upload")) {
		const char* bucket = snda_ecs_argument_retriever(argc, args, "--bucket=");
		const char* objectname = snda_ecs_argument_retriever(argc, args, "--key=");
		const char* uploadid = snda_ecs_argument_retriever(argc, args, "--uploadid=");
		const char* region = snda_ecs_argument_retriever(argc, args, "--region=");
		const char* sslstr = snda_ecs_argument_retriever(argc, args, "--ssl=");
		const char* followlocationstr = snda_ecs_argument_retriever(argc, args, "--followlocation=");
		const char* maxredirectsstr = snda_ecs_argument_retriever(argc, args, "--maxredirects=");
        int ssl = 0;
		int followlocation = 0;
		long maxdirects = 0;

		if (!cmd || !accesskey || !secretkey || !bucket || !objectname || !uploadid || !region || !sslstr || !followlocationstr || !maxredirectsstr ) {
			snda_ecs_oneline("arguments: --cmd=abort_multipart_upload --accesskey= --secretkey= --bucket= --key= --uploadid= --region= --ssl=(0/1) --followlocation=(0/1) --maxredirects=");
			exit(-1);
		}

		ssl = atoi(sslstr);
		followlocation = atoi(followlocationstr);
		maxdirects = atol(maxredirectsstr);

		retcode = snda_ecs_abort_multipart_upload(handler, accesskey, secretkey, bucket, objectname,
				uploadid, region, ssl, followlocation, maxdirects,
				ret);
	} else if (!strcmp(cmd, "upload_part")) {
		const char* bucket = snda_ecs_argument_retriever(argc, args, "--bucket=");
		const char* objectname = snda_ecs_argument_retriever(argc, args, "--key=");
		const char* uploadid = snda_ecs_argument_retriever(argc, args, "--uploadid=");
		const char* partnumberstr = snda_ecs_argument_retriever(argc, args, "--partnumber=");
		const char* localfile = snda_ecs_argument_retriever(argc, args, "--localfile=");
		const char* filelengthstr = snda_ecs_argument_retriever(argc, args, "--filelength=");
		const char* contentmd5 = snda_ecs_argument_retriever(argc, args, "--contenmd5=");
		const char* region = snda_ecs_argument_retriever(argc, args, "--region=");
		const char* sslstr = snda_ecs_argument_retriever(argc, args, "--ssl=");
        int ssl = 0;
		int partnumber = 0;
		long filelength = 0;
        FILE* fd = 0;

		if (!cmd || !accesskey || !secretkey || !bucket || !objectname || !uploadid || !localfile || !filelengthstr || !partnumberstr || !region || !sslstr) {
			snda_ecs_oneline("arguments: --cmd=upload_part --accesskey= --secretkey= --bucket= --key= --uploadid= --localfile= --filelength= --partnumber= --region= --ssl=(0/1) [--contenmd5=]");
			exit(-1);
		}

		ssl = atoi(sslstr);
		partnumber = atoi(partnumberstr);
		filelength = atol(filelengthstr);

		fd = fopen(localfile, "rb");

		retcode = snda_ecs_upload_part(handler, accesskey, secretkey, bucket, objectname, uploadid, partnumber,
				snda_ecs_put_object_body, fd, filelength, contentmd5, region, ssl,
				ret);

        fclose(fd);

	} else if (!strcmp(cmd, "list_parts")) {
		const char* bucket = snda_ecs_argument_retriever(argc, args, "--bucket=");
		const char* objectname = snda_ecs_argument_retriever(argc, args, "--key=");

		const char* uploadid = snda_ecs_argument_retriever(argc, args, "--uploadid=");

		const char* partnumbermarkerstr = snda_ecs_argument_retriever(argc, args, "--partnumbermarker=");
		const char* maxpartsstr = snda_ecs_argument_retriever(argc, args, "--maxparts=");

		const char* region = snda_ecs_argument_retriever(argc, args, "--region=");
		const char* sslstr = snda_ecs_argument_retriever(argc, args, "--ssl=");
		const char* followlocationstr = snda_ecs_argument_retriever(argc, args, "--followlocation=");
		const char* maxredirectsstr = snda_ecs_argument_retriever(argc, args, "--maxredirects=");
        int partnumbermarker = 0;
		int maxparts = 0;
		int ssl = 0;
		int followlocation = 0;
		long maxdirects = 0;

		if (!cmd || !accesskey || !secretkey || !bucket || !objectname || !uploadid || !region || !sslstr || !followlocationstr || !maxredirectsstr ) {
			snda_ecs_oneline("arguments: --cmd=list_parts --accesskey= --secretkey= --bucket= --key= --uploadid= --region= --ssl=(0/1) --followlocation=(0/1) --maxredirects= [--partnumbermarker= --maxparts=]");
			exit(-1);
		}

		partnumbermarker = partnumbermarkerstr == 0 ? -1 : atoi(partnumbermarkerstr);
		maxparts = maxpartsstr == 0 ? 1000 : atoi(maxpartsstr);
		ssl = atoi(sslstr);
		followlocation = atoi(followlocationstr);
		maxdirects = atol(maxredirectsstr);

		retcode = snda_ecs_list_parts(handler, accesskey, secretkey, bucket, objectname, uploadid, partnumbermarker, maxparts, region, ssl, followlocation, maxdirects, ret);
		if (retcode == SNDA_ECS_SUCCESS) {
            SNDAECSMultipartsContent* content = snda_ecs_to_multipart_parts(ret);
            SNDAECSMultipartsPart* part = content->parts;
			if (content) {
				snda_ecs_onelinevaluestring("Bucket", content->bucket);
				snda_ecs_onelinevaluestring("Key", content->key);
				snda_ecs_onelinevaluestring("UploadId", content->uploadid);
				snda_ecs_onelinevalueint("MaxParts", content->maxparts);
				snda_ecs_onelinevalueint("IsTruncated", content->istruncated);
				snda_ecs_onelinevalueint("PartNumberMarker", content->partnumbermarker);
				snda_ecs_onelinevalueint("NextPartNumberMarker", content->nextpartnumbermarker);

				snda_ecs_oneline("PARTS/");
				while (part) {
					snda_ecs_oneline("\tPART/");
					snda_ecs_onelinevalueint("\t\tPartNumber", part->partnumber);
					snda_ecs_onelinevaluelong("\t\tSize", part->size);
					snda_ecs_onelinevaluestring("\t\tLastModified", part->lastmodified);
					snda_ecs_onelinevaluestring("\t\tETag", part->etag);
					part = part->next;
					snda_ecs_oneline("\t/PART");
				}
				snda_ecs_oneline("/PARTS");
			}
			snda_ecs_release_multiparts_content(content);
		}
	} else if (!strcmp(cmd, "complete_multiparts")) {
		// okay, it's complicated to generate a common command line tool, so just give a sample
		char* accesskey = args[2];
		char* secretkey = args[3];
		char* bucket = args[4];
		char* objectname = args[5];
		char* uploadid = args[6];
		char* region = args[7];
		int ssl = atoi(args[8]);
		int followlocation = atoi(args[9]);
		long maxdirects = atol(args[10]);

		SNDAECSMultipartsMeta* metas = snda_ecs_init_multiparts_meta();
		SNDAECSMultipartsMeta* p = metas;
		p->partnumber = 100;
		snda_ecs_copy_string(&(p->etag), "\"58fda622140205b3d6a2457415d301f2\"");
		p->next = snda_ecs_init_multiparts_meta();
		p = p->next;
		p->partnumber = 1000;
		snda_ecs_copy_string(&(p->etag), "\"58fda622140205b3d6a2457415d301f2\"");
		p->next = snda_ecs_init_multiparts_meta();
		p = p->next;
		p->partnumber = 10000;
		snda_ecs_copy_string(&(p->etag), "\"296e2bd0ce7124b6fbda05873c261dfb\"");

		retcode = snda_ecs_complete_multipart_upload(handler, accesskey, secretkey, bucket, objectname, uploadid, metas, region, ssl, followlocation, maxdirects, ret);

		snda_ecs_release_multiparts_meta(metas);
	} else if (!strcmp(cmd, "test")) {
		//sdna_ecs_unit_test();
		return 0;
	} else {
		showhelp();
		exit(-1);
	}


	if (SNDA_ECS_SUCCESS != retcode) {
		snda_ecs_onelinevaluestring("ErrorMessage", ret->error->handlererrmsg);
	} else {
		snda_ecs_oneline("");
		// get http code
		snda_ecs_onelinevalueint("http response code", ret->serverresponse->httpcode);
		if (ret->serverresponse->httpcode >= 300) {
			SNDAECSErrorResponseContent* content = snda_ecs_to_error_response(ret);
            snda_ecs_onelinevaluestring(cmd, "FAILED WITH ERROR:");
	    	if (content) {
	    		if (content->code) {
	    			snda_ecs_onelinevaluestring("\tErrorCode", content->code);
				}
				if (content->message) {
					snda_ecs_onelinevaluestring("\tErrorMessage", content->message);
				}
				if (content->resource) {
					snda_ecs_onelinevaluestring("\tResource", content->resource);
				}
				if (content->requestid) {
					snda_ecs_onelinevaluestring("\tRequestId", content->requestid);
				}
				if (content->fullbody) {
					snda_ecs_onelinevaluestring("\tAllErrorMessage", content->fullbody);
				}
	    	}
	    	snda_ecs_release_error_response_content(content);
	    } else {
	    	snda_ecs_onelinevaluestring(cmd, "SUCCESSFULLY.");
	    }
	}


	snda_ecs_release_result(ret);

	snda_ecs_release_handler(handler);

	snda_ecs_global_uninit();

	return 0;
}



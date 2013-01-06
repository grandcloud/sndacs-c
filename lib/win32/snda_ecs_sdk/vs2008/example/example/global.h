/*
 * ProvideBy: SNDA Cloud Computing(http://www.grandcloud.cn).
 * 
 * Header file of c sdk for Elastic Cloud Storage(ECS) of GrandCloud.
 * 
 * This header file defines test constants for ECS sdk api.
 */

#ifndef GLOBAL_H
#define GLOBAL_H
#include <string.h>

#include "snda_ecs_sdk/snda_ecs_sdk.h"
#include "snda_ecs_sdk/snda_ecs_http_util.h"
#include "snda_ecs_sdk/snda_ecs_constants.h"
#include "snda_ecs_sdk/snda_ecs_common_util.h"


#define SNDA_ACCESSKEY "youraccesskey"

#define SNDA_ACCESS_SECRET "yoursecretkey"

#define SNDA_BUCKET_HUADONG "huadong_test_c"
#define SDNA_REGION_HUADONG "huadong-1"

#define MULTIPART_UPLOAD_OBJECT "multipart_upload_test"
#define LOCAL_SMALL_FILE_W "E:/c/C-SDK-V3/C-SDK-V3/src/example/test_data/compile.sh"
#define LOCAL_SMALL_FILE_L "compile.sh"
#define LOCAL_BIG_FILE_W "F:/FlowerShop.rar"
#define LOCAL_BIG_FILE_L "ds"

int get_service_example();
int put_bucket();
int get_bucket() ;
int delete_bucket();
int put_bucket_policy();
int get_bucket_policy();
int delete_bucket_policy();
int get_bucket_location();
int put_object();
int get_object();
int head_object();
int delete_object() ;
#endif /* SNDA_ECS_CONSTANTS_H */

/*
 * ProvideBy: SNDA Cloud Computing(http://www.grandcloud.cn).
 * 
 * Header file of c sdk for Elastic Cloud Storage(ECS) of GrandCloud.
 * 
 * This header file defines constants for ECS sdk api.
 */

#ifndef SNDA_ECS_CONSTANTS_H
#define SNDA_ECS_CONSTANTS_H

#undef  _FILE_OFFSET_BITS
#define _FILE_OFFSET_BITS 64

#define S_SNDA_ECS_MAX_URL_LEN (5 * 1024)

#define S_SNDA_ECS_AUTH_LEN (100)

#define S_SNDA_ECS_SHA1_LEN (20)

#define S_SNDA_ECS_BASIC_MSG_TO_SIGNATURE_LEN (4 * 1024)

#define S_SNDA_ECS_DATE_LEN (32)
#define S_SNDA_ECS_DATE_HEADER_LEN (40)
#define S_SNDA_ECS_CONTENT_LENGTH_LEN (32)
#define S_SNDA_ECS_BYTE_RANGE_HEADER_LEN (64)

#define S_SNDA_ECS_CONTENT_TYPE_LEN (64)
#define S_SNDA_ECS_MAX_EXTENSION_NAME_LEN (32)
#define S_SNDA_ECS_RET_BODY_ONE_TIME_ALLOCATE_LEN (2 * 1024 * 1024)

#define S_SNDA_ECS_CLIENT_ERROR_LEN (512)
/*
 * default Elastic Cloud Storage host of GrandCloud
 */
static const char* snda_ecs_default_hostname = "storage.grandcloud.cn";
static const char* snda_ecs_huadong1_hostname = "storage-huadong-1.grandcloud.cn";
static const char* snda_ecs_huabei1_hostname = "storage-huabei-1.grandcloud.cn";

static const char* snda_ecs_region_huadong1 = "huadong-1";
static const char* snda_ecs_region_huabei1 = "huabei-1";
static const char* snda_ecs_region_default = "huabei-1";

static const char* snda_canonicalized_prefix = "x-snda-";
static const char* snda_user_meta_prefix = "x-snda-meta-";

static const int snda_ecs_without_ssl = 0;

static const int snda_ecs_get_bucket_maxkeys_default = 1000;

static const char* snda_ecs_default_content_type = "binary/octet-stream";

#endif /* SNDA_ECS_CONSTANTS_H */

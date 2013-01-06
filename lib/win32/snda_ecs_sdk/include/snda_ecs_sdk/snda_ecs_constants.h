/*
 * ProvideBy: SNDA Cloud Computing(http://www.grandcloud.cn).
 * 
 * Header file of c sdk for Elastic Cloud Storage(ECS) of GrandCloud.
 * 
 * This header file defines constants for ECS sdk api.
 */

#ifndef SNDA_ECS_CONSTANTS_H
#define SNDA_ECS_CONSTANTS_H

#ifdef __cplusplus
extern "C" {
#endif

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
#define S_SNDA_ECS_MAX_HOST_LEN (32)

#define S_SNDA_ECS_RET_BODY_ONE_TIME_ALLOCATE_LEN (2 * 1024 * 1024)

#define S_SNDA_ECS_CLIENT_ERROR_LEN (512)

#ifdef __cplusplus
}
#endif

#endif /* SNDA_ECS_CONSTANTS_H */

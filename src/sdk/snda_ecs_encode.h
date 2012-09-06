/*
 * ProvideBy: SNDA Cloud Computing(http://www.grandcloud.cn).
 *
 * Header file of c sdk for Elastic Cloud Storage(ECS) of GrandCloud.
 *
 * This header file declare base64 encoding for ECS sdk api.
 */

#ifndef SNDA_ECS_ENCODE_H
#define SNDA_ECS_ENCODE_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @return a allocated buffer contains a base64 string terminated with '\0',
 * 		   user must release this resource after it will be never used.
 */
char * base64_encode(const char* inputbuff, int inputlen);

/**
 * make sure that hmacsha1 has 20 bytes at least because sha1 has 160 bits output.
 * S_SNDA_ECS_SHA1_LEN defined in <snda_ecs_constants.h> is appropriate.
 */
int hmac_sha1(const char* secretkey, const char * message, int messagelen, char *hmacsha1);

#ifdef __cplusplus
}
#endif

#endif /*SNDA_ECS_ENCODE_H*/


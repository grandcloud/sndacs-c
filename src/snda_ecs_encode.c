/*
 * ProvideBy: SNDA Cloud Computing(http://www.grandcloud.cn).
 *
 * Header file of c sdk for Elastic Cloud Storage(ECS) of GrandCloud.
 *
 * impl of base64 encoding utilities for ECS sdk api impl.
 */

#include <stddef.h>
#include <malloc.h>
#include <memory.h>
#include <openssl/hmac.h>

#include "snda_ecs_encode.h"

/*
 *  Macro for Base64encode
 */


#define B0(a) (a & 0xFF)
#define B1(a) (a >> 8 & 0xFF)
#define B2(a) (a >> 16 & 0xFF)
#define B3(a) (a >> 24 & 0xFF)


inline static char get_b64_char(int index)
{
    const char szBase64Table[] =
    	"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    if(index >= 0 && index < 64)
    {
        return szBase64Table[index];
    }

    return '=';
}

char * base64_encode(const char* inputbuff, int inputlen) {

	char * base64buff = NULL;
	char* p64 = NULL;

	unsigned long ulTmp = 0;

	int i = 0;
	int rest = 0;
	int j = 0;

	int base64buffsize = 0;

	unsigned char * psrc = (unsigned char *)inputbuff;

	if (!inputbuff || !inputlen) {
		return NULL;
	}

	base64buffsize = (inputlen / 3 + (inputlen % 3 > 0)) * 4 + 1;
	base64buff = (char*)malloc(base64buffsize);
	memset(base64buff, 0, base64buffsize);

	p64 = base64buff;
	for(i = 0; i < inputlen / 3 * 3; i += 3)
	{
		unsigned long ulTmp = *(unsigned long *)psrc;
		register int b0 = get_b64_char((B0(ulTmp) >> 2) & 0x3F);
		register int b1 = get_b64_char(
			(B0(ulTmp) << 6 >> 2 | B1(ulTmp) >> 4) & 0x3F);
		register int b2 = get_b64_char(
				(B1(ulTmp) << 4 >> 2 | B2( ulTmp ) >> 6) & 0x3F);
		register int b3 = get_b64_char((B2( ulTmp ) << 2 >> 2) & 0x3F);
		*((unsigned long *)p64) = b0 | b1 << 8 | b2 << 16 | b3 << 24;
		p64 += 4;
		psrc += 3;
	}

	if(i < inputlen)
	{
		rest = inputlen - i;
		for(j = 0; j < rest; ++j)
		{
			*(((unsigned char *)&ulTmp) + j) = *psrc++;
		}

		p64[0] = get_b64_char((B0( ulTmp ) >> 2) & 0x3F);
		p64[1] = get_b64_char((B0( ulTmp ) << 6 >> 2 | B1( ulTmp ) >> 4) & 0x3F);
		p64[2] =
				rest > 1 ?
						get_b64_char(
								(B1( ulTmp ) << 4 >> 2 | B2( ulTmp ) >> 6)
										& 0x3F) :
						'=';
		p64[3] = rest > 2 ? get_b64_char((B2( ulTmp ) << 2 >> 2) & 0x3F) : '=';
		p64 += 4;
	}

	return base64buff;
}

int hmac_sha1(const char* secretkey, const char * message, int messagelen, char *hmacsha1)
{
    HMAC_CTX ctx;
    unsigned int returnlength;

    HMAC_Init(&ctx, (const unsigned char*)secretkey, strlen(secretkey), EVP_sha1());
    HMAC_Update(&ctx, (const unsigned char*)message, messagelen);
    HMAC_Final(&ctx, (unsigned char*)hmacsha1, &returnlength);
    HMAC_cleanup(&ctx);

    return returnlength;
}


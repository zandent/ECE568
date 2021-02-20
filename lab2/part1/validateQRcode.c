#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "lib/sha1.h"

#include <time.h>
int hexToInt(const char data){
	if(data>=48 && data<=57){
		return data-48;
	}else if(data>=65 && data<=70){
		return data-55;
	}else{
		return 0;
	}
	return 0;
}
void xorFunc(uint8_t* out, uint8_t* d0, uint8_t* d1, int len){
	int i;
	for(i=0; i<len; i++){
		out[i] = d0[i] ^ d1[i];
	}
}
static int
validateTOTP(char * secret_hex, char * TOTP_string)
{
	uint8_t secret_cast[64];
	memset(secret_cast,0,64);
	int i;
	for (i=0; i<10; i++){
		secret_cast[i] = hexToInt(secret_hex[2*i])*16+hexToInt(secret_hex[2*i+1]);
	}
	uint8_t ipad[64];
	uint8_t opad[64];
	memset(ipad,0x36,64);
	memset(opad,0x5c,64);
	xorFunc(ipad,secret_cast,ipad, 64);
	xorFunc(opad,secret_cast,opad, 64);
	
	uint64_t rawtext = time(NULL)/30;
	uint8_t text[8];
	for(i=0; i<8; i++){
		text[7-i] = (rawtext>>(i*8)) & 0x00ff;
	}
	SHA1_INFO ctx;
	uint8_t inner[SHA1_DIGEST_LENGTH];
	sha1_init(&ctx);
	sha1_update(&ctx, ipad, 64);
	sha1_update(&ctx, text, 8);
	sha1_final(&ctx, inner);
	uint8_t outer[SHA1_DIGEST_LENGTH];
	sha1_init(&ctx);
	sha1_update(&ctx, opad, 64);
	sha1_update(&ctx, inner, 20);
	sha1_final(&ctx, outer);
	int offset = outer[19] & 0xf ;
	int opt = (outer[offset] & 0x7f) << 24
		| (outer[offset+1] & 0xff) << 16
		| (outer[offset+2] & 0xff) << 8
		| (outer[offset+3] & 0xff) ;
	char digit[7];
	sprintf(digit, "%06d", opt%1000000);
	printf("%06d\n", opt%1000000);
	return !(memcmp(digit, TOTP_string, 6));
}


int
main(int argc, char * argv[])
{
	if ( argc != 3 ) {
		printf("Usage: %s [secretHex] [TOTP]\n", argv[0]);
		return(-1);
	}

	char *	secret_hex = argv[1];
	char *	TOTP_value = argv[2];

	assert (strlen(secret_hex) <= 20);
	assert (strlen(TOTP_value) == 6);

	printf("\nSecret (Hex): %s\nTOTP Value: %s (%s)\n\n",
		secret_hex,
		TOTP_value,
		validateTOTP(secret_hex, TOTP_value) ? "valid" : "invalid");

	return(0);
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "lib/encoding.h"

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
int
main(int argc, char * argv[])
{
	if ( argc != 4 ) {
		printf("Usage: %s [issuer] [accountName] [secretHex]\n", argv[0]);
		return(-1);
	}

	char *	issuer = argv[1];
	char *	accountName = argv[2];
	char *	secret_hex = argv[3];

	assert (strlen(secret_hex) <= 20);

	printf("\nIssuer: %s\nAccount Name: %s\nSecret (Hex): %s\n\n",
		issuer, accountName, secret_hex);

	// Create an otpauth:// URI and display a QR code that's compatible
	// with Google Authenticator
	
	const char * issuer_encoded = urlEncode(issuer);
	const char * accountName_encoded = urlEncode(accountName);
	uint8_t secret_cast[10];
	char secret_encoded[17];
	int i;
	for (i=0; i<10; i++){
		secret_cast[i] = hexToInt(secret_hex[2*i])*16+hexToInt(secret_hex[2*i+1]);
	}	
	assert(base32_encode(secret_cast,10,secret_encoded,16)!=-1);
	secret_encoded[16]='\0';
	char url[15+strlen(accountName_encoded)+8+strlen(issuer_encoded)+8+strlen(secret_encoded)+10];
	sprintf(url,"otpauth://totp/%s?issuer=%s&secret=%s&period=30",accountName_encoded,issuer_encoded,secret_encoded);
	displayQRcode(url);


	return (0);
}

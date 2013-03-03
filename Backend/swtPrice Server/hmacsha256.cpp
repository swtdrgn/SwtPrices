#include "StdAfx.h"
#include "hmacsha256.h"

using namespace hmac;

/******************************************************************************
 * SHA-256.
 */
	
void hmac::sha256_initialize(sha256 *sha) {
    int i;
    for (i = 0; i < 17; ++i) sha->buffer[i] = 0;
    sha->state[0] = 0x6a09e667;
    sha->state[1] = 0xbb67ae85;
    sha->state[2] = 0x3c6ef372;
    sha->state[3] = 0xa54ff53a;
    sha->state[4] = 0x510e527f;
    sha->state[5] = 0x9b05688c;
    sha->state[6] = 0x1f83d9ab;
    sha->state[7] = 0x5be0cd19;
    for (i = 0; i < 8; ++i) sha->length[i] = 0;
}

//  Changed by RKW, formal args are now const uint8_t, uint_32
//    from const unsigned char, unsigned long respectively
void hmac::sha256_update(sha256 *sha,
                   const uint8_t *message,
                   uint32_t length) {
    int i, j;
    /* Add the length of the received message, counted in
     * bytes, to the total length of the messages hashed to
     * date, counted in bits and stored in 8 separate bytes. */
    for (i = 7; i >= 0; --i) {
        int bits;
		if (i == 7)
			bits = length << 3;
		else if (i == 0 || i == 1 || i == 2)
			bits = 0;
		else
			bits = length >> (53 - 8 * i);
		bits &= 0xff;
        if (sha->length[i] + bits > 0xff) {
            for (j = i - 1; j >= 0 && sha->length[j]++ == 0xff; --j);
        }
        sha->length[i] += bits;
    }
    /* Add the received message to the SHA buffer, updating the
     * hash at each block (each time the buffer is filled). */
    while (length > 0) {
        /* Find the index in the SHA buffer at which to
         * append what's left of the received message. */
        int index = sha->length[6] % 2 * 32 + sha->length[7] / 8;
        index = (index + 64 - length % 64) % 64;
        /* Append the received message bytes to the SHA buffer until
         * we run out of message bytes or until the buffer is filled. */
        for (;length > 0 && index < 64; ++message, ++index, --length) {
            sha->buffer[index / 4] |= *message << (24 - index % 4 * 8);
        }
        /* Update the hash with the buffer contents if the buffer is full. */
        if (index == 64) {
            /* Update the hash with a block of message content. See FIPS 180-2
             * (<csrc.nist.gov/publications/fips/fips180-2/fips180-2.pdf>)
             * for a description of and details on the algorithm used here. */
			// Changed by RKW, const unsigned long becomes const uint32_t
            const uint32_t k[64] = {
                0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
                0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
                0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
                0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
                0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
                0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
                0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
                0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
                0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
                0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
                0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
                0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
                0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
                0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
                0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
                0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
            };
			// Changed by RKW, unsigned long becomes uint32_t
            uint32_t w[64], a, b, c, d, e, f, g, h;
            int t;
            for (t = 0; t < 16; ++t) {
                w[t] = sha->buffer[t];
                sha->buffer[t] = 0;
            }
            for (t = 16; t < 64; ++t) {
				// Changed by RKW, unsigned long becomes uint32_t
                uint32_t s0, s1;
                s0 = (w[t - 15] >> 7 | w[t - 15] << 25);
                s0 ^= (w[t - 15] >> 18 | w[t - 15] << 14);
                s0 ^= (w[t - 15] >> 3);
                s1 = (w[t - 2] >> 17 | w[t - 2] << 15);
                s1 ^= (w[t - 2] >> 19 | w[t - 2] << 13);
                s1 ^= (w[t - 2] >> 10);
                w[t] = (s1 + w[t - 7] + s0 + w[t - 16]) & 0xffffffffU;
            }
            a = sha->state[0];
            b = sha->state[1];
            c = sha->state[2];
            d = sha->state[3];
            e = sha->state[4];
            f = sha->state[5];
            g = sha->state[6];
            h = sha->state[7];
            for (t = 0; t < 64; ++t) {
				// Changed by RKW, unsigned long becomes uint32_t
                uint32_t e0, e1, t1, t2;
                e0 = (a >> 2 | a << 30);
                e0 ^= (a >> 13 | a << 19);
                e0 ^= (a >> 22 | a << 10);
                e1 = (e >> 6 | e << 26);
                e1 ^= (e >> 11 | e << 21);
                e1 ^= (e >> 25 | e << 7);
                t1 = h + e1 + ((e & f) ^ (~e & g)) + k[t] + w[t];
                t2 = e0 + ((a & b) ^ (a & c) ^ (b & c));
                h = g;
                g = f;
                f = e;
                e = d + t1;
                d = c;
                c = b;
                b = a;
                a = t1 + t2;
            }
            sha->state[0] = (sha->state[0] + a) & 0xffffffffU;
            sha->state[1] = (sha->state[1] + b) & 0xffffffffU;
            sha->state[2] = (sha->state[2] + c) & 0xffffffffU;
            sha->state[3] = (sha->state[3] + d) & 0xffffffffU;
            sha->state[4] = (sha->state[4] + e) & 0xffffffffU;
            sha->state[5] = (sha->state[5] + f) & 0xffffffffU;
            sha->state[6] = (sha->state[6] + g) & 0xffffffffU;
            sha->state[7] = (sha->state[7] + h) & 0xffffffffU;
        }
    }
}

//  Changed by RKW, formal args are now const uint8_t, uint_32
//    from const unsigned char, unsigned long respectively
void hmac::sha256_finalize(sha256 *sha,
                     const uint8_t *message,
                     uint32_t length) {
    int i;
	// Changed by RKW, unsigned char becomes uint8_t
    uint8_t terminator[64 + 8] = { 0x80 };
    /* Hash the final message bytes if necessary. */
    if (length > 0) sha256_update(sha, message, length);
    /* Create a terminator that includes a stop bit, padding, and
     * the the total message length. See FIPS 180-2 for details. */
    length = 64 - sha->length[6] % 2 * 32 - sha->length[7] / 8;
    if (length < 9) length += 64;
    for (i = 0; i < 8; ++i) terminator[length - 8 + i] = sha->length[i];
    /* Hash the terminator to finalize the message digest. */
    sha256_update(sha, terminator, length);
    /* Extract the message digest. */
    for (i = 0; i < 32; ++i) {
        sha->hash[i] = (sha->state[i / 4] >> (24 - 8 * (i % 4))) & 0xff;
    }
}

//  Changed by RKW, formal args are now uint8_t, const uint_8
//    from unsigned char, const unsigned char respectively
void sha256_get(uint8_t hash[32],
                const uint8_t *message,
                int length) {	
    int i;
    sha256 sha;
    sha256_initialize(&sha);
    sha256_finalize(&sha, message, length);
    for (i = 0; i < 32; ++i) hash[i] = sha.hash[i];
}

/******************************************************************************
 * HMAC-SHA256.
 */

//  Changed by RKW, formal arg is now const uint8_t
//    from const unsigned char
void hmac::hmac_sha256_initialize(hmac_sha256 *hmac,
                            const uint8_t *key, int length) {
    int i;
    /* Prepare the inner hash key block, hashing the key if it's too long. */
    if (length <= 64) {
        for (i = 0; i < length; ++i) hmac->key[i] = key[i] ^ 0x36;
        for (; i < 64; ++i) hmac->key[i] = 0x36;
    } else {
        sha256_initialize(&(hmac->sha));
        sha256_finalize(&(hmac->sha), key, length);
        for (i = 0; i < 32; ++i) hmac->key[i] = hmac->sha.hash[i] ^ 0x36;
        for (; i < 64; ++i) hmac->key[i] = 0x36;
    }
    /* Initialize the inner hash with the key block. */
    sha256_initialize(&(hmac->sha));
    sha256_update(&(hmac->sha), hmac->key, 64);
}

//  Changed by RKW, formal arg is now const uint8_t
//    from const unsigned char
void hmac::hmac_sha256_update(hmac_sha256 *hmac,
                        const uint8_t *message, int length) {
    /* Update the inner hash. */
    sha256_update(&(hmac->sha), message, length);
}

//  Changed by RKW, formal arg is now const uint8_t
//    from const unsigned char
void hmac::hmac_sha256_finalize(hmac_sha256 *hmac,
                          const uint8_t *message, int length) {
    int i;
    /* Finalize the inner hash and store its value in the digest array. */
    sha256_finalize(&(hmac->sha), message, length);
    for (i = 0; i < 32; ++i) hmac->digest[i] = hmac->sha.hash[i];
    /* Convert the inner hash key block to the outer hash key block. */
    for (i = 0; i < 64; ++i) hmac->key[i] ^= (0x36 ^ 0x5c);
    /* Calculate the outer hash. */
    sha256_initialize(&(hmac->sha));
    sha256_update(&(hmac->sha), hmac->key, 64);
    sha256_finalize(&(hmac->sha), hmac->digest, 32);
    /* Use the outer hash value as the HMAC digest. */
    for (i = 0; i < 32; ++i) hmac->digest[i] = hmac->sha.hash[i];
}

//  Changed by RKW, formal args are now uint8_t, const uint8_t
//    from unsinged char, const unsigned char respectively
void hmac::hmac_sha256_get(uint8_t digest[32],
                     const uint8_t *message, int message_length,
                     const uint8_t *key, int key_length) {
    int i;
    hmac_sha256 hmac;
    hmac_sha256_initialize(&hmac, key, key_length);
    hmac_sha256_finalize(&hmac, message, message_length);
    for (i = 0; i < 32; ++i) digest[i] = hmac.digest[i];
}

std::string hmac::signature_sha256( std::string k, std::string m ){
	const char *kc = k.c_str();

	hmac_sha256 hmac;
    sha256 sha;
    unsigned char key[64];
    unsigned char block[1024];
    int i, c, d;
    /* Read the key, hashing it if necessary. */
	sha256_initialize(&sha);
	for (i = 0; (c = kc[i]) > 31 && c < 127; ++i) {
		if (i > 0 && i % 64 == 0) sha256_update(&sha, key, 64);
			key[i % 64] = c;
	}
    /* Initialize the HMAC-SHA256 digest with the key or its hash. */
	if (i <= 64) {
		hmac_sha256_initialize(&hmac, key, i);
	} else {
		sha256_finalize(&sha, key, i % 64);
		hmac_sha256_initialize(&hmac, sha.hash, 64);
	}
	/* Read the message, updating the HMAC-SHA256 digest accordingly. */
	hmac_sha256_update(&hmac,(const uint8_t *)m.c_str(), m.size());
	/* Finalize the HMAC-SHA256 digest and output its value. */
	hmac_sha256_finalize(&hmac, NULL, 0);
	std::string s = base64_char(&hmac.digest[0],32);

	print_hex(&hmac.digest[0],32);
	printf("\n\nHash = %s\n",s.c_str());

	return s;
}

char hmac::base64_set[] =
	{	'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
		'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',
		'0','1','2','3','4','5','6','7','8','9','+','/' };


int hmac::base64_length( std::string s ){
	return ceil(s.length()*sizeof(char)*8./6.);
}

uint8_t * hmac::to_base64( std::string s ){
	int size = base64_length(s);
	const char * cstr = s.c_str();
	uint8_t * b = new uint8_t [size];
	for( int i=0; i<size; i++ ){
		if(i%4==0){
			b[i] = (cstr[i*6/8]&0xFC) >> 2;
		}
		else if(i%4==1){
			b[i] = ((cstr[(int)floor(i*6./8.)]&0x03)<<4) + ((cstr[(int)floor(i*6./8.)+1]&0xF0)>>4);
		}
		else if(i%4==2){
			b[i] = ((cstr[(int)floor(i*6./8.)]&0x0F)<<2) + ((cstr[(int)floor(i*6./8.)+1]&0xC0)>>6);
		}
		else if(i%4==3){
			b[i] = cstr[(int)floor(i*6./8.)]&0x3F;
		}
	}
	return b;
}

std::string hmac::base64_char( uint8_t * s, int length ){
	int size = ceil(length*sizeof(uint8_t)*8./6.);
	unsigned char b;
	std::string str="";
	int i;
	for( i=0;i<size;i++,b=0 ){
		if(i%4==0){
			b = (s[i*6/8]&0xFC) >> 2;
		}
		else if(i%4==1){
			b = (((int)s[(int)floor(i*6./8.)]&0x03)<<4) + (((int)floor(i*6./8.)+1<length)?(((int)s[(int)floor(i*6./8.)+1]&0xF0)>>4):0);
		}
		else if(i%4==2){
			b = ((s[(int)floor(i*6./8.)]&0x0F)<<2) + (((int)floor(i*6./8.)+1<length)?((s[(int)floor(i*6./8.)+1]&0xC0)>>6):0);
		}
		else if(i%4==3){
			b = s[(int)floor(i*6./8.)]&0x3F;
		}
		if(b<64){ str += base64_set[b]; }
		else{ str += "?"; }
	}

	for( std::string::iterator it=--str.end(); *it == 'A'; it-- ){
		*it = '=';
	}
	return str;
}

/*char* hmac::base64_char( uint8_t * s, int size ){
	char * str = new char[size+1];
	for( int i=0; i<size; i++ ){
		if( ((char*)s)[i] > 63 ){
			printf("base64_char failed.");
			exit(0);
		}
		str[i] = base64_set[((char*)s)[i]];
	}
	str[size] = 0;
	return str;
}*/

void hmac::print_hex( uint8_t * b, int size ){
	for( int i=0 ; i<size ; i++ ){
	   printf("%2.2x",b[i]);
	}
}

int hmac::char_base64( char * s, uint8_t *& b ){
	int size = 0;
	while(s[size]!=0 && s[size]!='='){ size++; }
	int length = ceil(size*(6./8.)/sizeof(uint8_t));
	b = new uint8_t[length];
	uint8_t t;
	for( int i=0,j=0; i<size; i++ ){
		if( 'A' <= s[i] && s[i] <= 'Z' ){
			t = s[i] - 'A';
		}
		else if( 'a' <= s[i] && s[i] <= 'z' ){
			t = s[i] - 'a' + 26;
		}
		else if( '0' <= s[i] && s[i] <= '9' ){
			t = s[i] - '0' + 52;
		}
		else if( s[i] == '+' ){
			t = 62;
		}
		else if( s[i] == '/' ){
			t = 63;
		}
		else{
			printf("Invalid character.");
			return 0;
		}

		if(i%4==0){
			b[j] = t<<2;
		}
		else if(i%4==1){
			b[j] += (t>>4)&0x03;
			b[++j] = (t&0x0F)<<4;
		}
		else if(i%4==2){
			b[j++] += (t>>2)&0x0F;
			b[j] = (t&0x03)<<6;
		}
		else if(i%4==3){
			b[j++] += t&0x3F;
		}
	}
	return length;
}


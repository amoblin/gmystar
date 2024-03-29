/* MD5.H - header file for MD5.C */
/* 
Copyright (C) 1991-2, RSA Data Security, Inc. Created 1991. All
rights reserved.

License to copy and use this software is granted provided that it
is identified as the "RSA Data Security, Inc. MD5 Message-Digest
Algorithm" in all material mentioning or referencing this software
or this function.

License is also granted to make and use derivative works provided
that such works are identified as "derived from the RSA Data
Security, Inc. MD5 Message-Digest Algorithm" in all material
mentioning or referencing the derived work.

RSA Data Security, Inc. makes no representations concerning either
the merchantability of this software or the suitability of this
software for any particular purpose. It is provided "as is"
without express or implied warranty of any kind.

These notices must be retained in any copies of any part of this
documentation and/or software.*/

#ifndef MD5_H
#define MD5_h

/* POINTER defines a generic pointer type */
typedef unsigned char *POINTER;

/* UINT2 defines a two byte word */
typedef unsigned short int UINT2;

/* UINT4 defines a four byte word */
typedef unsigned long int UINT4;

/* MD5 context. */
typedef struct 
{
  UINT4 state[4];                                   /* state (ABCD) */
  UINT4 count[2];        /* number of bits, modulo 2^64 (lsb first) */
  unsigned char buffer[64];                         /* input buffer */
} MD5_CTX;

class MD5 {
public:
	static unsigned char PADDING[64];

	static void MD5Init(MD5_CTX * context);
	static void MD5Update(MD5_CTX *context, unsigned char *input, unsigned int inputLen);
	static void MD5Final(unsigned char digest[16], MD5_CTX *context);
	
	//return the md5sum (in the form of static-local-variable),whose length is 16 bytes
	static unsigned char* ComputeHash(unsigned char *src,int len);

private:

	static void MD5Transform(UINT4 [4], unsigned char [64]);
	static void Encode(unsigned char *, UINT4 *, unsigned int);
	static void Decode(UINT4 *, unsigned char *, unsigned int);
	static void MD5_memcpy(POINTER, POINTER, unsigned int);
	static void MD5_memset(POINTER, int, unsigned int);

};

#endif /* MD5_H */


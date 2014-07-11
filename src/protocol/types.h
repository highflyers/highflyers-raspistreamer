/*
 * types.h
 *
 *  Created on: May 7, 2014
 *      Author: loganek
 */

#ifndef TYPES_H_
#define TYPES_H_

#define FRAMEPARSER_HELPER_MAXLENGTH 2048
#define FRAMEPARSER_HELPER_SENTINEL 13
#define FRAMEPARSER_HELPER_ENDFRAME 12

typedef unsigned char byte;
#ifndef __cplusplus
typedef unsigned char bool;
#endif
typedef int int32;
typedef unsigned int uint32;
typedef unsigned short uint16;

#define true 1
#define false 0

enum EndianType
{
	LITTLE,
	BIG
};

#endif /* TYPES_H_ */

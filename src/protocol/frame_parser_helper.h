#ifndef HIGHFLYERS_PROTOCOL_FRAME_PARSER_HELPER_H
#define HIGHFLYERS_PROTOCOL_FRAME_PARSER_HELPER_H

#include "types.h"
#include <string.h>

void init_highflyers_protocol ();
bool frame_parser_helper_check_bytes (byte* bytes, int size);

uint32 frame_parser_helper_to_uint32 (const byte* bytes, int index);
uint16 frame_parser_helper_to_uint16 (const byte* bytes, int index);
double frame_parser_helper_to_double (const byte* bytes, int index);
void frame_parser_helper_set_uint32 (byte* bytes, uint32 value);
void frame_parser_helper_set_int32 (byte* bytes, int32 value);
void frame_parser_helper_set_double (byte* bytes, double value);
void frame_parser_helper_set_uint16 (byte* bytes, uint16 value);

#define CONVERT_TO_DATATYPE( type ) \
type frame_parser_helper_to_##type (const byte* bytes, int index) \
{ \
	type val = 0; \
	memcpy(&val, bytes+index, sizeof( type )); \
	return val; \
}

#define SETTER_TO_ARRAY_DATATYPE( type ) \
void frame_parser_helper_set_ ##type (byte* bytes, type value) \
{ \
	memcpy(bytes, (byte*)&value, sizeof(type)); \
}


#endif

// GENERATED CODE! DON'T MODIFY IT!
#ifndef HIGHFLYERS_PROTOCOL_FRAMES_H
#define HIGHFLYERS_PROTOCOL_FRAMES_H
#include "types.h"
void frame_preparse_data (const byte* data, bool* output, int field_count);
void frame_finalise(const byte* data, int size, bool* output);
typedef enum
{
	T_IPAddress,
	T_StreamerInfo
} FrameTypes;
typedef struct
{
	byte ip1;
	byte ip2;
	byte ip3;
	byte ip4;
} IPAddress;
IPAddress* IPAddress_parse (const byte* data);
int IPAddress_current_size (const IPAddress* value);
int IPAddress_preserialize (const IPAddress* value, byte* output);
void IPAddress_serialize (const IPAddress* value, byte* output);

typedef struct
{
	IPAddress ip;
	uint16 port;
	uint16 width;
	uint16 height;
	uint16 framerate;
} StreamerInfo;
StreamerInfo* StreamerInfo_parse (const byte* data);
int StreamerInfo_current_size (const StreamerInfo* value);
int StreamerInfo_preserialize (const StreamerInfo* value, byte* output);
void StreamerInfo_serialize (const StreamerInfo* value, byte* output);

#endif


// GENERATED CODE! DON'T MODIFY IT!
#include "frames.h"
#include "frame_parser_helper.h"
#include <stdlib.h>
void frame_preparse_data (const byte* data, bool* output, int field_count)
{
	uint16 field_flags = frame_parser_helper_to_uint16 (data, 0);
	int i;
	for (i = 0; i < field_count; i++)
		output[i] = (field_flags & (1 << i)) != 0;
}
void frame_finalise(const byte* data, int size, bool* output)
{
	int bytesAdded = 0;
	int i;
	for (i = 0; i < size; i++)
	{
		if ((data[i] == FRAMEPARSER_HELPER_SENTINEL)
				|| (data[i] == FRAMEPARSER_HELPER_ENDFRAME))
		{
			frame_parser_helper_set_byte (output + i + bytesAdded,
			FRAMEPARSER_HELPER_SENTINEL);
			bytesAdded++;
		}
		frame_parser_helper_set_byte (output + i + bytesAdded, data[i]);
	}
	frame_parser_helper_set_byte (output + size + bytesAdded,
	FRAMEPARSER_HELPER_ENDFRAME);
}
IPAddress* IPAddress_parse (const byte* data)
{
	bool fields[4];
	int iterator = 2;
	IPAddress* value;
	value = (IPAddress*)malloc(sizeof(IPAddress));
	frame_preparse_data(data, fields, 4);
	if (fields[0])
	{
		value->ip1 = data[iterator];
		iterator += sizeof(byte);
	}
	else
	{
		free (value);
		return NULL;
	}
	if (fields[1])
	{
		value->ip2 = data[iterator];
		iterator += sizeof(byte);
	}
	else
	{
		free (value);
		return NULL;
	}
	if (fields[2])
	{
		value->ip3 = data[iterator];
		iterator += sizeof(byte);
	}
	else
	{
		free (value);
		return NULL;
	}
	if (fields[3])
	{
		value->ip4 = data[iterator];
		iterator += sizeof(byte);
	}
	else
	{
		free (value);
		return NULL;
	}
	return value;
}

int IPAddress_current_size (const IPAddress* value)
{
	int size = 2; //enable flags
	size += sizeof(byte);
	size += sizeof(byte);
	size += sizeof(byte);
	size += sizeof(byte);
	return size;
}
int IPAddress_preserialize (const IPAddress* value, byte* output)
{
	uint16 must_be = 0;
	int iterator = 2;
	must_be = (uint16)(must_be | (1 << 0));
frame_parser_helper_set_byte(output + iterator, value->ip1);
	iterator += sizeof(byte);
	must_be = (uint16)(must_be | (1 << 1));
frame_parser_helper_set_byte(output + iterator, value->ip2);
	iterator += sizeof(byte);
	must_be = (uint16)(must_be | (1 << 2));
frame_parser_helper_set_byte(output + iterator, value->ip3);
	iterator += sizeof(byte);
	must_be = (uint16)(must_be | (1 << 3));
frame_parser_helper_set_byte(output + iterator, value->ip4);
	iterator += sizeof(byte);
	frame_parser_helper_set_uint16 (output, must_be);
	return iterator;
}
void IPAddress_serialize (const IPAddress* value, byte* output)
{
	int frame_size = 1 + IPAddress_current_size (value) + sizeof(uint32); // struct + crc32
	byte* data = (byte*)malloc (frame_size);
	data[0] = 0;
	int iterator = IPAddress_preserialize(value, data + 1);
	uint32 crc = frame_parser_helper_calculate_crc (data, iterator + 1);
	frame_parser_helper_set_uint32 (data + 1 + iterator, crc);
	frame_finalise(data, frame_size, output);
	free(data);
}
StreamerInfo* StreamerInfo_parse (const byte* data)
{
	bool fields[5];
	int iterator = 2;
	StreamerInfo* value;
	value = (StreamerInfo*)malloc(sizeof(StreamerInfo));
	frame_preparse_data(data, fields, 5);
	if (fields[0])
	{
		IPAddress* str = IPAddress_parse(data + iterator);
		value->ip = *str;
		free(str);
		iterator += IPAddress_current_size(&value->ip);
	}
	else
	{
		free (value);
		return NULL;
	}
	if (fields[1])
	{
		value->port = frame_parser_helper_to_uint16(data, iterator);
		iterator += sizeof(uint16);
	}
	else
	{
		free (value);
		return NULL;
	}
	if (fields[2])
	{
		value->width = frame_parser_helper_to_uint16(data, iterator);
		iterator += sizeof(uint16);
	}
	else
	{
		free (value);
		return NULL;
	}
	if (fields[3])
	{
		value->height = frame_parser_helper_to_uint16(data, iterator);
		iterator += sizeof(uint16);
	}
	else
	{
		free (value);
		return NULL;
	}
	if (fields[4])
	{
		value->framerate = frame_parser_helper_to_uint16(data, iterator);
		iterator += sizeof(uint16);
	}
	else
	{
		free (value);
		return NULL;
	}
	return value;
}

int StreamerInfo_current_size (const StreamerInfo* value)
{
	int size = 2; //enable flags
	size += IPAddress_current_size(&value->ip);
	size += sizeof(uint16);
	size += sizeof(uint16);
	size += sizeof(uint16);
	size += sizeof(uint16);
	return size;
}
int StreamerInfo_preserialize (const StreamerInfo* value, byte* output)
{
	uint16 must_be = 0;
	int iterator = 2;
	must_be = (uint16)(must_be | (1 << 0));
{
	byte* output_f = (byte*)malloc (IPAddress_current_size (&value->ip));
	int size_f = IPAddress_preserialize(&value->ip, output_f);
	memcpy(output + iterator, output_f, size_f);
	free(output_f);
	iterator += size_f;
}
	must_be = (uint16)(must_be | (1 << 1));
frame_parser_helper_set_uint16(output + iterator, value->port);
	iterator += sizeof(uint16);
	must_be = (uint16)(must_be | (1 << 2));
frame_parser_helper_set_uint16(output + iterator, value->width);
	iterator += sizeof(uint16);
	must_be = (uint16)(must_be | (1 << 3));
frame_parser_helper_set_uint16(output + iterator, value->height);
	iterator += sizeof(uint16);
	must_be = (uint16)(must_be | (1 << 4));
frame_parser_helper_set_uint16(output + iterator, value->framerate);
	iterator += sizeof(uint16);
	frame_parser_helper_set_uint16 (output, must_be);
	return iterator;
}
void StreamerInfo_serialize (const StreamerInfo* value, byte* output)
{
	int frame_size = 1 + StreamerInfo_current_size (value) + sizeof(uint32); // struct + crc32
	byte* data = (byte*)malloc (frame_size);
	data[0] = 1;
	int iterator = StreamerInfo_preserialize(value, data + 1);
	uint32 crc = frame_parser_helper_calculate_crc (data, iterator + 1);
	frame_parser_helper_set_uint32 (data + 1 + iterator, crc);
	frame_finalise(data, frame_size, output);
	free(data);
}


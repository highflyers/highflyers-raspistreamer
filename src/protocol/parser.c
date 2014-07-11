#include "parser.h"
#include "frame_builder.h"
#include <stdlib.h>

void frame_proxy_initialize(FrameProxy* proxy)
{
	proxy->pointer = NULL;
}

void frame_proxy_free(FrameProxy* proxy)
{
	free((void*)proxy->pointer);
	proxy->pointer = NULL;
}

void parse_frame (HighFlyersParser* obj);

void parser_initialize (HighFlyersParser* obj)
{
	obj->prev_sentinel = false;
	obj->iterator = 0;
	obj->last_frame_actual = false;
}

void parser_append_byte (HighFlyersParser* obj, byte b)
{
	bool tmp_sentinel = obj->prev_sentinel;
	
	if (obj->prev_sentinel)
	{
		if (b == FRAMEPARSER_HELPER_SENTINEL || b == FRAMEPARSER_HELPER_ENDFRAME)
			obj->bytes[obj->iterator++ % FRAMEPARSER_HELPER_MAXLENGTH] = b;
		else
		{
			obj->iterator = 0;
		}
		obj->prev_sentinel = false;
	}
	else if (b == FRAMEPARSER_HELPER_SENTINEL)
		obj->prev_sentinel = true;
	else
		obj->bytes[obj->iterator++ % FRAMEPARSER_HELPER_MAXLENGTH] = b;

	if (b == FRAMEPARSER_HELPER_ENDFRAME && !tmp_sentinel)
		parse_frame (obj);
	else if (FRAMEPARSER_HELPER_MAXLENGTH == obj->iterator) 
	{
		obj->iterator = 0;
	}
}

void parse_frame (HighFlyersParser* obj)
{
	FrameProxy p;
	p = frame_builder_build_frame (obj->bytes, obj->iterator);
	if (p.pointer != NULL)
	{
		obj->last_frame_actual = true;
		obj->last_frame = p;
	}
	else
	{
		obj->last_frame_actual = false;
	}
	obj->iterator = 0;
}

bool parser_has_frame (const HighFlyersParser* obj)
{
	return obj->last_frame_actual;
}

FrameProxy parser_get_last_frame_ownership (HighFlyersParser* obj)
{
	obj->last_frame_actual = false;
	return obj->last_frame;
}

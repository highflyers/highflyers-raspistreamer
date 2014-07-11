#ifndef HIGHFLYERS_PROTOCOL_PARSER_H
#define HIGHFLYERS_PROTOCOL_PARSER_H

#include "frames.h"

typedef struct {
	FrameTypes type;
	void* pointer;
} FrameProxy;

void frame_proxy_initialize(FrameProxy* proxy);
void frame_proxy_free(FrameProxy* proxy);

typedef struct {
	bool prev_sentinel;
	byte bytes[FRAMEPARSER_HELPER_MAXLENGTH];
	int iterator;
	FrameProxy last_frame;
	bool last_frame_actual;
} HighFlyersParser;


void parser_initialize (HighFlyersParser* obj);
void parser_append_byte (HighFlyersParser* obj, byte b);
bool parser_has_frame (const HighFlyersParser* obj);
FrameProxy parser_get_last_frame_ownership (HighFlyersParser* obj);

#endif

#pragma once
#include <string.h>

#define MAX_PACKET_SIZE 1000000

enum PacketTypes {

	INIT_CONNECTION = 0,

	ACTION_EVENT = 1,

	POSITION_EVENT = 2,

	LEAP_PLAYER = 0,

	OCULUS_PLAYER = 1

};

struct Packet {

    unsigned int packet_type;
	unsigned int whichPlayer;
	float leapX;
	float leapY;
	float leapZ;
	float oculX;
	float oculY;
	float oculZ;
	float leapSbX;
	float leapSbY;
	float leapSbZ;
	float oculSbX;
	float oculSbY;
	float oculSbZ;
	bool leapHoldingSnowball;
	bool oculHoldingSnowball;

	bool leapHit;
	bool oculHit;
	

    void serialize(char * data) {
        memcpy(data, this, sizeof(Packet));
    }

    void deserialize(char * data) {
        memcpy(this, data, sizeof(Packet));
    }
};
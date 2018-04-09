#pragma once
#include "ServerNetwork.h"
#include "NetworkData.h"

class ServerGame
{

public:

    ServerGame(void);
    ~ServerGame(void);

	float leapPlayerX = 0;
	float leapPlayerY = 0;
	float leapPlayerZ = 40;

	float oculusPlayerX = 0;
	float oculusPlayerY = 0;
	float oculusPlayerZ = 40;

	float leapSnowballX;
	float leapSnowballY;
	float leapSnowballZ;

	float oculSnowballX;
	float oculSnowballY;
	float oculSnowballZ;

	bool leapHoldingSnowball;
	bool oculHoldingSnowball;

	bool leapHit;
	bool oculHit;

    void update();

	void receiveFromClients();

	void sendActionPackets();
	void sendOculusPlayerPackets();
	void sendLeapPlayerPackets();
	void sendPlayerInfoPackets();

private:

   // IDs for the clients connecting for table in ServerNetwork 
    static unsigned int client_id;

   // The ServerNetwork object 
    ServerNetwork* network;

	// data buffer
   char network_data[MAX_PACKET_SIZE];
};
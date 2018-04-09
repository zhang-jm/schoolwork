#pragma once
#include <winsock2.h>
#include <Windows.h>
#include "ClientNetwork.h"
#include "NetworkData.h"

class ClientGame
{
public:
	ClientGame(void);
	~ClientGame(void);

	ClientNetwork* network;

	float playerX;
	float playerY;
	float playerZ;

	float enemyPlayerX;
	float enemyPlayerY;
	float enemyPlayerZ;

	float snowballX;
	float snowballY;
	float snowballZ;

	float enemySnowballX;
	float enemySnowballY;
	float enemySnowballZ;

	bool playerSnowballInHand;
	bool enemySnowballInHand;

	bool enemyHit;
	bool hitByEnemy;

	void sendActionPackets();

	void sendPositionPackets();

    char network_data[MAX_PACKET_SIZE];

    void update();
};


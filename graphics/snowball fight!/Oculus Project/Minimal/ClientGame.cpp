#include "StdAfx.h"
#include "ClientGame.h"


ClientGame::ClientGame(void)
{
	playerX = 0;
	playerY = 0;
	playerZ = 0;
    network = new ClientNetwork();

    // send init packet
    const unsigned int packet_size = sizeof(Packet);
    char packet_data[packet_size];

    Packet packet;
    packet.packet_type = INIT_CONNECTION;

    packet.serialize(packet_data);

    NetworkServices::sendMessage(network->ConnectSocket, packet_data, packet_size);
}


ClientGame::~ClientGame(void)
{
}

void ClientGame::sendActionPackets()
{
    // send action packet
    const unsigned int packet_size = sizeof(Packet);
    char packet_data[packet_size];

    Packet packet;
	packet.whichPlayer = OCULUS_PLAYER;
    packet.packet_type = ACTION_EVENT;

    packet.serialize(packet_data);

    NetworkServices::sendMessage(network->ConnectSocket, packet_data, packet_size);
}

void ClientGame::sendPositionPackets() {
	// send action packet
	const unsigned int packet_size = sizeof(Packet);
	char packet_data[packet_size];

	Packet packet;
	packet.whichPlayer = OCULUS_PLAYER;
	packet.packet_type = POSITION_EVENT;
	packet.oculX = playerX*100.0f;
	packet.oculY = playerY*20.0f;
	packet.oculZ = playerZ*100.0f;
	packet.oculHoldingSnowball = playerSnowballInHand;
	packet.oculSbX = snowballX*20.0f;
	packet.oculSbY = snowballY*20.0f;
	packet.oculSbZ = snowballZ*20.0f;
	packet.oculHit = hitByEnemy;

	packet.serialize(packet_data);

	NetworkServices::sendMessage(network->ConnectSocket, packet_data, packet_size);
}

void ClientGame::update()
{
    Packet packet;
    int data_length = network->receivePackets(network_data);
	//playerX++;
    if (data_length <= 0) 
    {
        //no data recieved
        return;
    }

    int i = 0;
    while (i < (unsigned int)data_length) 
    {
        packet.deserialize(&(network_data[i]));
        i += sizeof(Packet);
		
		enemyPlayerX = packet.leapX;
		enemyPlayerY = packet.leapY;
		enemyPlayerZ = packet.leapZ;

		enemySnowballX = packet.leapSbX;
		enemySnowballY = packet.leapSbY;
		enemySnowballZ = packet.leapSbZ;
		enemySnowballInHand = packet.leapHoldingSnowball;

		enemyHit = packet.leapHit;
		//printf("Leap Player Position: %f, %f, %f \n", enemyPlayerX, enemyPlayerY, enemyPlayerZ);
		//printf("Leap Snowball Position: %f, %f, %f \n", enemySnowballX, enemySnowballY, enemySnowballZ);
		//Sleep(10);
		//sendPositionPackets();
        /*switch (packet.packet_type) {

            case ACTION_EVENT:

                printf("client received action event packet from server\n");
				//Sleep(50);
                //sendActionPackets();
				sendPositionPackets();

                break;

			case POSITION_EVENT:

				//printf("Received position event from server");
				//Sleep(50);
				sendPositionPackets();

            default:

                printf("error in packet types\n");

                break;
        }*/
    }
	sendPositionPackets();
}

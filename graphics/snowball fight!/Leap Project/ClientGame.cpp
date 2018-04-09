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
	packet.whichPlayer = LEAP_PLAYER;
    packet.packet_type = ACTION_EVENT;

    packet.serialize(packet_data);

    NetworkServices::sendMessage(network->ConnectSocket, packet_data, packet_size);
}

void ClientGame::sendPositionPackets() {
	// send action packet
	const unsigned int packet_size = sizeof(Packet);
	char packet_data[packet_size];

	Packet packet;
	packet.whichPlayer = LEAP_PLAYER;
	packet.packet_type = POSITION_EVENT;
	packet.leapX = playerX;
	packet.leapY = playerY;
	packet.leapZ = playerZ;
	packet.leapHoldingSnowball = playerSnowballInHand;
	packet.leapSbX = snowballX;
	packet.leapSbY = snowballY;
	packet.leapSbZ = snowballZ;

	packet.leapHit = hitByEnemy;

	packet.serialize(packet_data);

	int sent = NetworkServices::sendMessage(network->ConnectSocket, packet_data, packet_size);
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
		
		
		enemyPlayerX = packet.oculX;
		enemyPlayerY = packet.oculY;
		enemyPlayerZ = packet.oculZ;
	
		enemySnowballX = packet.oculSbX;
		enemySnowballY = packet.oculSbY;
		enemySnowballZ = packet.oculSbZ;
		enemySnowballInHand = packet.oculHoldingSnowball;

		enemyHit = packet.oculHit;
		
		//printf("Oculus Player Position: %f, %f, %f \n", enemyPlayerX, enemyPlayerY, enemyPlayerZ);
		//printf("Oculus Snowball Position: %f, %f, %f \n", enemySnowballX, enemySnowballY, enemySnowballZ);

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

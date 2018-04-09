#include "StdAfx.h"
#include "ServerGame.h"

unsigned int ServerGame::client_id; 

ServerGame::ServerGame(void)
{
    // id's to assign clients for our table
    client_id = 0;

    // set up the server network to listen 
    network = new ServerNetwork(); 
}

ServerGame::~ServerGame(void)
{
}

void ServerGame::update()
{
    // get new clients
   if(network->acceptNewClient(client_id))
   {
        printf("client %d has been connected to the server\n",client_id);

        client_id++;
   }

   receiveFromClients();
}

void ServerGame::receiveFromClients()
{
	memset(network_data, 0, MAX_PACKET_SIZE);
    Packet packet;

    // go through all clients
    std::map<unsigned int, SOCKET>::iterator iter;

    for(iter = network->sessions.begin(); iter != network->sessions.end(); iter++)
    {
        int data_length = network->receiveData(iter->first, network_data);

        if (data_length <= 0) 
        {
            //no data recieved
            continue;
        }

		printf("received: %d \n", data_length);

        int i = 0;
        while (i < (unsigned int)data_length) 
        {
            packet.deserialize(&(network_data[i]));
            i += sizeof(Packet);
			

			switch (packet.whichPlayer) {

				case LEAP_PLAYER:

					leapPlayerX = packet.leapX;
					leapPlayerY = packet.leapY;
					leapPlayerZ = packet.leapZ;
					leapSnowballX = packet.leapSbX;
					leapSnowballY = packet.leapSbY;
					leapSnowballZ = packet.leapSbZ;
					leapHoldingSnowball = packet.leapHoldingSnowball;
					leapHit = packet.leapHit;

					//printf("Leap Player Position: %f, %f, %f \n", leapPlayerX, leapPlayerY, leapPlayerZ);
					//printf("Leap Snowball Position: %f, %f, %f \n", leapSnowballX, leapSnowballY, leapSnowballZ);
					break;

				case OCULUS_PLAYER:

					oculusPlayerX = packet.oculX;
					oculusPlayerY = packet.oculY;
					oculusPlayerZ = packet.oculZ;
					oculSnowballX = packet.oculSbX;
					oculSnowballY = packet.oculSbY;
					oculSnowballZ = packet.oculSbZ;
					oculHoldingSnowball = packet.oculHoldingSnowball;
					oculHit = packet.oculHit;

					//printf("Oculus Player Position: %f, %f, %f \n", oculusPlayerX, oculusPlayerY, oculusPlayerZ);
					//printf("Oculus Snowball Position: %f, %f, %f \n", oculSnowballX, oculSnowballY, oculSnowballZ);
					break;


			}


            switch (packet.packet_type) {

                case INIT_CONNECTION:

                    printf("server received init packet from client\n");
					//Sleep(50);
                    sendActionPackets();

                    break;

                case ACTION_EVENT:

                    printf("server received action event packet from client\n");
					Sleep(50);
                    sendActionPackets();

                    break;

				case POSITION_EVENT:

					//printf("received position event packet from client\n");
					//printf(packet.)
					//sendActionPackets();
					Sleep(50);
					//if (packet.whichPlayer == LEAP_PLAYER) sendOculusPlayerPackets();
					//else if (packet.whichPlayer == OCULUS_PLAYER) sendLeapPlayerPackets();
					//else sendActionPackets();

					break;

                default:

                    printf("error in packet types\n");

                    break;
            }
        }

		sendPlayerInfoPackets();
    }
}


void ServerGame::sendActionPackets()
{
    // send action packet
    const unsigned int packet_size = sizeof(Packet);
    char packet_data[packet_size];

    Packet packet;
	packet.whichPlayer = 2;
    packet.packet_type = ACTION_EVENT;

    packet.serialize(packet_data);

    network->sendToAll(packet_data,packet_size);
}

/*void ServerGame::sendLeapPlayerPackets()
{
	// send action packet
	const unsigned int packet_size = sizeof(Packet);
	char packet_data[packet_size];

	Packet packet;
	packet.packet_type = ACTION_EVENT;
	packet.whichPlayer = LEAP_PLAYER;
	packet.x = leapPlayerX;
	packet.y = leapPlayerY;
	packet.z = leapPlayerZ;

	packet.serialize(packet_data);


	network->sendToAll(packet_data, packet_size);
}

void ServerGame::sendOculusPlayerPackets() 
{
	// send action packet
	const unsigned int packet_size = sizeof(Packet);
	char packet_data[packet_size];

	Packet packet;
	packet.packet_type = ACTION_EVENT;
	packet.whichPlayer = OCULUS_PLAYER;

	//REMOVE THIS after implementing Oculus client
	//oculusPlayerX = leapPlayerX;
	//oculusPlayerY = leapPlayerY;
	//oculusPlayerZ = leapPlayerZ;

	packet.x = oculusPlayerX;
	packet.y = oculusPlayerY;
	packet.z = oculusPlayerZ;

	packet.serialize(packet_data);


	network->sendToAll(packet_data, packet_size);
}*/

void ServerGame::sendPlayerInfoPackets() {
	// send action packet
	const unsigned int packet_size = sizeof(Packet);
	char packet_data[packet_size];

	Packet packet;
	packet.packet_type = POSITION_EVENT;
	//packet.whichPlayer = OCULUS_PLAYER;

	//REMOVE THIS after implementing Oculus client
	//oculusPlayerX = leapPlayerX;
	//oculusPlayerY = leapPlayerY;
	//oculusPlayerZ = leapPlayerZ;

	packet.leapX = leapPlayerX;
	packet.leapY = leapPlayerY;
	packet.leapZ = leapPlayerZ;
	packet.leapSbX = leapSnowballX;
	packet.leapSbY = leapSnowballY;
	packet.leapSbZ = leapSnowballZ;
	packet.leapHoldingSnowball = leapHoldingSnowball;
	packet.leapHit = leapHit;

	packet.oculX = oculusPlayerX;
	packet.oculY = oculusPlayerY;
	packet.oculZ = oculusPlayerZ;
	packet.oculSbX = oculSnowballX;
	packet.oculSbY = oculSnowballY;
	packet.oculSbZ = oculSnowballZ;
	packet.oculHoldingSnowball = oculHoldingSnowball;
	packet.oculHit = oculHit;

	packet.serialize(packet_data);
	network->sendToAll(packet_data, packet_size);
}
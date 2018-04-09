// Server.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <string>
//#include "rpc/server.h"

using std::string;


#include "ServerGame.h"

// used for multi-threading
#include <process.h>

void serverLoop();


ServerGame * server;


int main()
{


	// initialize the server
	server = new ServerGame();
	std::cout << "Starting server..." << std::endl;


	// create thread with arbitrary argument for the run function
	//_beginthread(serverLoop, 0, (void*)12);	
	serverLoop();

}

void serverLoop()
{
	while (true)
	{
		server->update();
		//std::cout << "Updating" << std::endl;
	}
}


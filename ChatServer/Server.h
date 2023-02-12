#pragma once
#include <iostream>
#include <thread>
#include <vector>
#include <SFML/Network.hpp>
#include "Client.h"
#include "Message.h";
#include "Queue.h"

class Client;

class Server
{
private:
	sf::TcpListener m_listener;
	std::vector<Client*> m_clients;
	Queue<Message>* m_queue;
	int m_portNumber;

public:
	Server(int p_portNumber);
	void DiscoverClients();
	void ConnectClients();
	void DisconnectClient(sf::TcpSocket* p_clientSocket, size_t p_index);
	void SendData(Message msg);
	void ManageMessages();
	void Run();
};


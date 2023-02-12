#pragma once
#include <iostream>
#include <thread>
#include <vector>
#include <SFML/Network.hpp>
#include "Server.h"
#include "Queue.h"
#include "Message.h"

class Server;

class Client
{
private:
	std::string m_id;
	int m_index;
	sf::TcpSocket* m_clientSocket;
	Server* m_server;
	Queue<Message>* m_queue;

public:
	Client(Server* p_server, int p_index, sf::TcpSocket* p_socket, Queue<Message>* p_queue);
	void ReceiveMessages();
	void SendData(Message m);

	int GetIndex();
	sf::TcpSocket* GetClientSocket();
};


#include "Client.h"

sf::Packet& operator <<(sf::Packet& packet, Message& msg) // send
{
	return packet << msg.msg << msg.type << msg.myIndex << msg.rotate << msg.seed;
}

sf::Packet& operator >>(sf::Packet& packet, Message& msg) // receive
{
	return packet >> msg.msg >> msg.type >> msg.myIndex >> msg.rotate >> msg.seed;
}

Client::Client(Server* p_server, int p_index, sf::TcpSocket* p_socket, Queue<Message>* p_queue)
{
	m_server = p_server;
	m_index = p_index;
	m_clientSocket = p_socket;
	m_queue = p_queue;

	std::thread(&Client::ReceiveMessages, this).detach();
}

void Client::ReceiveMessages()
{
	while (true)
	{
		sf::Packet packet;

		if (m_clientSocket->receive(packet) == sf::Socket::Disconnected)
		{
			m_server->DisconnectClient(m_clientSocket, m_index);
			break;
		}

		Message m;

		if (packet >> m)
		{			
			m_queue->Push(m);
		}
		else
		{
			std::cout << "message not received\n";
		}
	}
}

void Client::SendData(Message msg)
{
	if(msg.type == MSG_NONE)
	{
		return;
	}

	sf::Packet packet;

	packet << msg;

	if (m_clientSocket->send(packet) != sf::Socket::Done)
	{
		std::cout << "Error\n";
	}
}


int Client::GetIndex()
{
	return m_index;
}

sf::TcpSocket* Client::GetClientSocket()
{
	return m_clientSocket;
}



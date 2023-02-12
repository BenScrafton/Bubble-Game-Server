#include "Server.h"

Server::Server(int p_portNumber)
{
	m_portNumber = p_portNumber;
	m_queue = new Queue<Message>();
	std::cout << "Server Started\n";
}

void Server::DiscoverClients()
{
	sf::UdpSocket udpConnection;
	char msgReceived[7];
	size_t received;
	sf::IpAddress remoteIp;
	unsigned short remotePort;

	if(udpConnection.bind(53000) != sf::Socket::Done)
	{
		std::cout << "Error" << std::endl;
	}

	while(true)
	{
		if (udpConnection.receive(msgReceived, sizeof(msgReceived), received, remoteIp, remotePort) == sf::Socket::Done)
		{
			std::cout << "received msg" << std::endl;

			char expectedMsg[7] = "Client";

			if (strcmp(msgReceived, expectedMsg) == 0)
			{
				std::cout << "client found" << std::endl;

				char msg[7] = "Server";
				udpConnection.send(msg, sizeof(msg), remoteIp, remotePort);
			}
		}
	}
}

void Server::ConnectClients()
{
	unsigned int seed = time(NULL);

	if (m_listener.listen(m_portNumber) == sf::Socket::Done)
	{
		while (true)
		{
			sf::TcpSocket* clientSocket = new sf::TcpSocket();

			std::cout << "Incoming Connection\n";

			if (m_listener.accept(*clientSocket) == sf::Socket::Done)
			{
				std::cout << "Added Client: " << clientSocket->getRemoteAddress() << ":" << clientSocket->getRemotePort() << std::endl;

				Client* newClient = new Client(this, m_clients.size(), clientSocket, m_queue);

				Message m;
				m.type = MSG_REGISTER;
				m.myIndex = m_clients.size();
				m.seed = seed;
				newClient->SendData(m);

				m_clients.push_back(newClient);

				if(m_clients.size() == 2)
				{
					Message readyMsg;
					readyMsg.type = MSG_READY;

					for(Client* client : m_clients)
					{
						client->SendData(readyMsg);
					}
				}
			}
		}
	}
}

void Server::DisconnectClient(sf::TcpSocket* p_clientSocket, size_t p_index)
{
	std::cout << "Disconnected Client " << p_index << std::endl;

	if(p_index > m_clients.size() - 1)
	{
		p_index = m_clients.size() - 1;
	}
	delete(p_clientSocket);
	m_clients.erase(m_clients.begin() + p_index);
}

void Server::Run()
{
	std::thread(&Server::ManageMessages, this).detach();
	std::thread(&Server::DiscoverClients, this).detach();

	ConnectClients();
}


void Server::SendData(Message msg)
{
	for (Client* client : m_clients)
	{
		if(client->GetClientSocket() != m_clients[msg.myIndex]->GetClientSocket())
		{
			client->SendData(msg);
		}
	}
}

void Server::ManageMessages()
{
	while(true)
	{
		Message m;
		m = m_queue->Pop();
		SendData(m);
	}
}

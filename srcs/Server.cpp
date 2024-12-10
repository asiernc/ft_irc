/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anovio-c <anovio-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:24:51 by anovio-c          #+#    #+#             */
/*   Updated: 2024/12/10 12:30:27 by anovio-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

Server Server::instance;
bool Server::_initialized = false;
std::string Server::_srvName = "pikachu.server.com";

void	Server::init(int port, const std::string &pass) {
	if (_initialized) {
		std::cout << "Server is already running" << std::endl;
		return ;
	}
	if (port < 1024 || port > 49151)
		throw std::invalid_argument("Invalid port number");
	instance._port = port;
	instance._pass = pass;
	_initialized = true;
}

Client	*Server::getClientBySocket(int fd) {
	for (size_t i = 0; i < _clients.size(); ++i) {
		if (_clients[i]->getFd() == fd)
			return (_clients[i]);
	}
	return (NULL);
}

void	Server::cleanServer() {
			for (size_t i = 0; i < _clients.size(); ++i)
				delete _clients[i];
			_clients.clear();
			for (size_t i = 0; i < _channels.size(); ++i)
				delete _channels[i];
			_channels.clear();
	}

Server::~Server() {
	if (this->_listeningSocket > 0)
		close(_listeningSocket);
	cleanServer();
	std::cout << "Server destroyed..." << std::endl;
	}

void			Server::addChannel(Channel *channel) {
	Server::getInstance()._channels.push_back(channel);
	}

Channel	*Server::getCheckChannel(const std::string &name) {
	for (size_t i = 0; i < _channels.size(); ++i) {
		if (_channels[i]->getName() == name)
			return (_channels[i]);
	}
	return (NULL);
}


int	Server::start()
{
	// check if port is free ?

	this->_listeningSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_listeningSocket == -1)
	{
		std::cerr << "Error creating a socket" << std::endl;
		return 1;
	}

	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(6667);
	hint.sin_addr.s_addr = INADDR_ANY;

	if (bind(this->_listeningSocket, (struct sockaddr*)&hint, sizeof(hint)) == -1)
	{
		std::cerr << "Error binding IP/Port" << std::endl;
		return 1;
	}
	
	if (listen(this->_listeningSocket, FD_SETSIZE) == -1)
	{
		std::cerr << "Error listening" << std::endl;
		return 1;
	}
	
	pollfd listeningPoll;
	listeningPoll.fd = this->_listeningSocket;
	listeningPoll.events = POLLIN;
	pollfds.push_back(listeningPoll);

	while(42)
	{
		int pollCount = poll(pollfds.data(), pollfds.size(), -1); //poll espera infinito (-1), si consideramos ejem 1000 ms para ocntrolar clientes incactivos
		if (pollCount == -1)
		{
			std::cerr << "Error doing poll" << std::endl;
			break ;
		}

	/*	sockaddr_in client;
		socklen_t clientSize = sizeof(client);
		int clientSocket = accept(listening, (struct sockaddr *)&client, &clientSize);
		int clientSocket = accept(listening, NULL, NULL);
		if (clientSocket == -1)
		{
			std::cerr << "Error connecting with client" << std::endl;
			return 1;
		}
		char host[1024] = {0};
		int	received = recv(clientSocket, host, sizeof(host) - 1, 0);
		if (received > 0)
			host[received] = '\0';
		std::cout << "Message from client: " << host << std::endl;
	}*/
	Client *client;
	for (size_t i = 0; i < pollfds.size(); ++i)
	{
			if (pollfds[i].revents & POLLIN)
			{
				if (pollfds[i].fd == this->_listeningSocket)
				{
					// New connection
					sockaddr_in clientSock;
					socklen_t clientSize = sizeof(clientSock);
					int clientSocket = accept(this->_listeningSocket, (sockaddr*)&clientSock, &clientSize);
					if (clientSocket == -1)
					{
						std::cerr << "Error accepting client" << std::endl;
						continue;
					}
					std::cout << "New connection established" << std::endl;
					Client *newClient = new Client("", "", clientSocket);
					_clients.push_back(newClient);

					pollfd clientPoll;
					clientPoll.fd = clientSocket;
					clientPoll.events = POLLIN;
					pollfds.push_back(clientPoll);
					client->setFd(pollfds[i].fd);
				}
				else
				{
					// Old connection
					char buffer[1024] = {0}; //this atribute is in client.hpp
					int received = recv(pollfds[i].fd, buffer, sizeof(buffer) - 1, 0);
					if (received <= 0)
					{
						std::cout << "Client disconnected" << std::endl;
						// delete de client aqui??
						close(pollfds[i].fd);
						pollfds.erase(pollfds.begin() + i);
						--i;
					}
					else
					{
						buffer[received] = '\0';
						std::cout << "Message from client: " << buffer << std::endl;
						Client *client = getClientBySocket(pollfds[i].fd); // Implementar esta función según tu diseño
						if (client) {
							std::string message(buffer);
							// parsing buffer
							parser(client, buffer);
						}
						std::string response = "Message received: " + std::string(buffer) + "\r\n";
						send(pollfds[i].fd, response.c_str(), response.size(), 0);
					}
				}
			}
		}
	}
	close(this->_listeningSocket);
	return 0;
}

int			Server::getPort(void) {return this->_port;}
std::string	Server::getPass(void) {return this->_pass;}


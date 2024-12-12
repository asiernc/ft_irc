/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   USER.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbotargu <pbotargu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 11:12:30 by pbotargu          #+#    #+#             */
/*   Updated: 2024/12/12 13:50:16 by pbotargu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include "Utils.hpp"

static bool checkUser(std::string &username)
{
	
	std::vector<Client *>	vclient = Server::getInstance().getClients();
	for (long unsigned int i = 0; i < vclient.size(); i++)
	{
		if (vclient[i]->getUsername() == username)
			return false;
	}
	return true;
}

void	USER(Client *client, std::string pass)
{
    if (checkerIsLogged(client) == false)
        return ;
    std::vector<std::string> words = split(pass, ' ');
    if (words.size() < 4)
    {
        std::cout << "No hay los parametros necesarios" << std::endl;
        return ;
        //send error
    }
    //username es unic? en cas que si cal revisar si ja existeix o no. Mirar si el client ja esta registrat?
    if (!checkUser(words[0]))
    {    
        sendError(client, 433, "This username already exists!"); //revisar codi d'error  i missatge!!!    
        return ;
    }
    // <username> <hostname> <servername> <realname>
    client->setUsername(words[0]);
    client->setHostname(words[1]); // revisar com ha de ser hostname
    client->setServername(words[2]);
    client->setRealName(words[3]);
    
    std::cout << client->getUsername() << std::endl;
    std::cout << client->getHostname() << std::endl;
    std::cout << client->getServername() << std::endl;
    std::cout << client->getRealname() << std::endl;
	std::string response = "User created!\r\n";
    send(client->getFd(), response.c_str(), response.size(), 0);
    if (!client->getNickname().empty() && !client->getRegistered())
    {
        client->setRegistered(true);
        std::string response = "Welcome to FT_IRC!!!!\r\n";
        std::string pikachu_art = 
"quu..__\n"
" $$$b  `---.__\n"
"  \"$b        `--.                          ___.---uuudP\n"
"   `$$b           `.__.------.__     __.---'      $$$$\"              .\n"
"     \"$b          -'            `-.-'            $$$\"              .'|\n"
"       \".                                       d$\"             _.'  |\n"
"         `.   /                              ...\"             .'     |\n"
"           `./                           ..::-'            _.'       |\n"
"            /                         .:::-'            .-'         .'\n"
"           :                          ::''\\          _.'            |\n"
"          .' .-.             .-.           `.      .'               |\n"
"          : /'$$|           .@\"$\\           `.   .'              _.-'\n"
"         .'|$u$$|          |$$,$$|           |  <            _.-'\n"
"         | `:$$:'          :$$$$$:           `.  `.       .-'\n"
"         :                  `\"--'             |    `-.     \\\n"
"        :##.       ==             .###.       `.      `.    `\\\n"
"        |##:                      :###:        |        >     >\n"
"        |#'     `..'`..'          `###'        x:      /     /\n"
"         \\                                   xXX|     /    ./\n"
"          \\                                xXXX'|    /   ./\n"
"          /`-.                                  `.  /   /\n"
"         :    `-  ...........,                   | /  .'\n"
"         |         ``:::::::'       .            |<    `.\n"
"         |             ```          |           x| \\ `.:``.\n"
"         |                         .'    /'   xXX|  `:`M`M':.\n"
"         |    |                    ;    /:' xXXX'|  -'MMMMM:'\n"
"         `.  .'                   :    /:'       |-'MMMM.-'\n"
"          |  |                   .'   /'        .'MMM.-'\n"
"          `'`'                   :  ,'          |MMM<\n"
"            |                     `'            |tbap\\\n"
"             \\                                  :MM.-'\n"
"              \\                 |              .''\n"
"               \\.               `.            /\n"
"                /     .:::::::.. :           /\n"
"               |     .:::::::::::`.         /\n"
"               |   .:::------------\\       /\n"
"              /   .''               >::'  /\n"
"              `',:                 :    .'\n"
"                                   `:.:' \n";

        send(client->getFd(), response.c_str(), response.size(), 0);
        send(client->getFd(), pikachu_art.c_str(), pikachu_art.size(), 0);

    }
}

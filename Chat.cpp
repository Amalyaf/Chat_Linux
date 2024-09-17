#include "Chat.h"
#include <algorithm>
#include "bad_login.h"
#include "bad_password.h"
#include <filesystem>
#include <fstream>
#include <cstdlib>

using namespace std;
Chat::Chat()
{
	vector<Users>allUsers;
	
	if (getReadUsersStatus() == 1) {
		readUsers();}

	readMessage();
}

Chat::~Chat() {
	writeUsers();
	writeMessage();
}


void Chat::getChat()
{
	for (vector<Users>::iterator it = allUsers.begin(); it < allUsers.end(); it++)
	{
		cout << *it << endl;
	}
}

void Chat::enter()
{
	char c = 'y';
	while (c != 'n')
	{
		try
		{
			cout << "\nДля входа введите логин: \n";
			cin >> _login;
			Users user;
			user._login = _login;
			vector<Users>::iterator result = find(allUsers.begin(), allUsers.end(), user);

			if (result == allUsers.end())
			{
				throw BadLogin();
			}

			else
			{
				user = *result;
				std::cout << "Введите пароль: \n";
				std::cin >> _password;
				
				if (user._password != _password)
				{
					throw BadPassword();
				}
				else
				{
					_status = true;
					c = 'n';
					printMessage(_login);
				}
			}
		}
		catch (BadLogin& e)
		{
			std::cout << e.what() << std::endl;
			std::cin >> c;
		}
		catch (BadPassword& e)
		{
			std::cout << e.what() << std::endl;
			std::cin >> c;
		}
	}
}

void Chat::registration()
{
	char c = 'y';
		while (c != 'n')
		{
			cout << "Регистрация нового пользователя\n";
			Users user;
			user.setUser();
			vector<Users>::iterator result = find(allUsers.begin(), allUsers.end(), user);
			if (result != allUsers.end())
			{
				cout << "\nПользователь с таким логином уже существует!\nХотите повторить попытку?(y/n)\n";
				cin >> c;
				cout << endl;
			}
			else
			{
				allUsers.push_back(user);
				c = 'n';
			}
		}
}

void Chat::sendPrivateMessage()
{
	char c = 'y';
	_sender = _login;
	while (c != 'n')
	{
		std::cout << "Кому: ";
		cin >> _recipient;
		Users user;
		user._login = _recipient;
		vector<Users>::iterator result = find(allUsers.begin(), allUsers.end(), user);
		if (result == allUsers.end())
		{
			cout << "Получатель не найден!\nХотите повторить попытку?(y/n)";
			cin >> c;
		}

		else
		{
			Message message;
			message._recipient = _recipient;
			message._sender = _sender;
			std::cout << "Введите сообщение:\n";
			message.setMessage();
			allMessage.push_back(message);
			c = 'n';
		}
	}
}

void Chat::sendPublicMessage()
{
	Message message;
	message._sender = _login;
	std::cout << "Введите групповое сообщение:\n";
	message.setMessage();
	for (vector<Users>::iterator it = allUsers.begin(); it < allUsers.end(); it++)
	{
		if (it->_login != _login)
		{
			message._recipient = it->_login;
			allPublicMessage.push_back(message);
		}
	}

	message._recipient = "all";
	viewedMessage.push_back(message);
}


void Chat::printMessage(string recipient)
{
	int count = 0;
	for (vector<Message>::iterator it = allMessage.begin(); it < allMessage.end(); it++)
	{
		if (it->_recipient == recipient)
		{
			count++;
			if (count == 1)
			{
				std::cout << "\n------------------------------------------------------\n";
				std::cout << "У вас есть новые личные сообщения" << ": ";
			}
			cout << "\nОтправитель: " << it->_sender << endl <<
				"Получатель: " << it->_recipient << endl <<
				"Сообщение: " << it->_message << endl;
		}
	}
	if (count!=0)
	{
		std::cout << "\n------------------------------------------------------\n";
		deletePrivateMessage(recipient);
	}
	count = 0;
	for (vector<Message>::iterator it = allPublicMessage.begin(); it < allPublicMessage.end(); it++)
	{
		if (it->_recipient == recipient)
		{
			count++;
			if (count == 1)
			{
				std::cout << "\n------------------------------------------------------\n";
				std::cout << "У вас есть новые общие сообщения" << ": ";
			}
			cout << "\nОтправитель: " << it->_sender << endl <<
				"Сообщение: " << it->_message << endl;
		}
	}
	if (count!=0)
	{
		std::cout << "\n------------------------------------------------------\n";
		deletePublicMessage(recipient);
	}
}

void Chat::deletePrivateMessage(std::string recipient)
{
	for (vector<Message>::iterator it = allMessage.begin(); it < allMessage.end();)
	{
		if (it->_recipient == recipient)
		{
			viewedMessage.push_back(*it);
			it = allMessage.erase(it);
		}
		else
			++it;
	}
}

void Chat::deletePublicMessage(std::string recipient)
{
	for (vector<Message>::iterator it = allPublicMessage.begin(); it < allPublicMessage.end();)
	{
		if (it->_recipient == recipient)
		{
			it = allPublicMessage.erase(it);
		}
		else
			++it;
	}
}

void Chat::printAllMessage()
{
	for (vector<Message>::iterator it = viewedMessage.begin(); it != viewedMessage.end(); it++)
	{
		cout << "\nОтправитель: " << it->_sender << endl <<
			"Получатель: " << it->_recipient << endl <<
			"Сообщение: " << it->_message << endl;
	}
	cout << endl;
}

bool Chat::getstatus()
{
	return _status;
}

void Chat::exit()
{
	_status = false;
	_login.clear();
	_password.clear();
}

void Chat::readUsers() {
        std::fstream file = std::fstream(userData, std::ios::in);

        if (!file)
        {
                std::cout<<"No file"<< std::endl;
                return;
        }
      	
        const std::string delimiter = ":";
        
	if (file.is_open()) {
		std::string line;
		std::string title;
                std::string value;
		int user_cnt = 0;

		std::fstream user_cnt_file (user_count, std::ios::in);
		if (user_cnt_file.is_open()) {
			std::string line;
			getline(user_cnt_file, line);
			user_cnt = stoi(line);
		}
		for (int i = 0; i < user_cnt; i++) {
			Users user;
			int count = 0;
        	while (count < 3 && std::getline(file, line)) {
				size_t delimiterPosition = line.find(delimiter);
                		if (delimiterPosition > 0) {
                       			title = line.substr (0, delimiterPosition);
                       			value = line.substr (delimiterPosition + 1);

		       			if (title == "Name") {
                           			user._name = value;
                        		}
                       			else if (title == "Login") {
                           			user._login = value;
                        		}
                       			else if (title == "Password") {
                           			user._password = value;
                        		}
				       count++;
				}
			}
			allUsers.push_back(user);
        	}
        	file.close();
	}
}

int Chat::getReadUsersStatus() {
	std::fstream file (userData, std::ios::in);
	if (!file) {
        	return -1;
	}
	
	std::fstream user_cnt_file (user_count, std::ios::in);
	if (!user_cnt_file) {
		return -1;
	}

	else if (user_cnt_file.is_open()) {
		std::string line;
		int user_cnt = 0;
                getline(user_cnt_file, line);
                user_cnt = stoi(line);
		if (user_cnt == 0) {
			return -1;
		}
	}
		
	return 1;
}

void Chat::writeUsers() const {

	fstream file = fstream(userData, ios::in | ios::out | ios::trunc);

        auto permissions = std::filesystem::perms::group_all | std::filesystem::perms::others_all; // права, которые хотим отобрать
        std::filesystem::permissions (userData, permissions, std::filesystem::perm_options::remove);

	for (int i = 0; i < allUsers.size(); i++)
        {
                file << "Name:" << allUsers[i]._name << "\n";
        	file << "Login:" << allUsers[i]._login << "\n";
        	file << "Password:" << allUsers[i]._password << "\n";
        }

        file.close();
	
	fstream user_cnt_file = fstream(user_count, ios::in | ios::out | ios::trunc);
        std::filesystem::permissions (user_count, permissions, std::filesystem::perm_options::remove);

	user_cnt_file << allUsers.size();
	user_cnt_file.close();
}

void Chat::writeMessage() const {
	
	fstream file_prvt_msg = fstream(user_private_message, ios::in | ios::out | ios::trunc);
	
	auto permissions = std::filesystem::perms::group_all | std::filesystem::perms::others_all; // права, которые хотим отобрать
	std::filesystem::permissions (user_private_message, permissions, std::filesystem::perm_options::remove);

	for (int i = 0; i < allMessage.size(); i++) {
		file_prvt_msg << "Recipient:" << allMessage[i]._recipient << "\n";
		file_prvt_msg << "Sender:" << allMessage[i]._sender << "\n";
		file_prvt_msg << "Message:" << allMessage[i]._message << "\n";
	}
	file_prvt_msg.close();


	fstream file_pblc_msg = fstream(user_public_message, ios::in | ios::out | ios::trunc);
	std::filesystem::permissions (user_public_message, permissions, std::filesystem::perm_options::remove);
	
	for (int i = 0; i < allPublicMessage.size(); i++) {
		
		file_pblc_msg << "Pecipient:" << allPublicMessage[i]._recipient << "\n";
                file_pblc_msg << "Sender:" << allPublicMessage[i]._sender << "\n";
                file_pblc_msg << "Message:" << allPublicMessage[i]._message << "\n";
	}

	file_pblc_msg.close();

	fstream prvt_msg_cnt_file = fstream(prvt_msg_count, ios::in | ios::out | ios::trunc);
	std::filesystem::permissions (prvt_msg_count, permissions, std::filesystem::perm_options::remove);

        prvt_msg_cnt_file << allMessage.size();
        prvt_msg_cnt_file.close();
}

void Chat::readMessage() {
        
	std::fstream file = std::fstream(user_private_message, std::ios::in);

        if (!file)
        {
                std::cout<<"No file"<< std::endl;
                return;
        }
        
	const std::string delimiter = ":";
        
	if (file.is_open()) {
                std::string line;
                std::string title;
                std::string value;
                int prvt_msg_cnt = 0;

                std::fstream prvt_msg_cnt_file (prvt_msg_count, std::ios::in);
                if (prvt_msg_cnt_file.is_open()) {
                        std::string line;
                        getline(prvt_msg_cnt_file, line);
                        prvt_msg_cnt = stoi(line);
                }
                for (int i = 0; i < prvt_msg_cnt; i++) {
                        Message msg;
                        int count = 0;
                while (count < 3 && std::getline(file, line)) {
                                size_t delimiterPosition = line.find(delimiter);
                                if (delimiterPosition > 0) {
                                        title = line.substr (0, delimiterPosition);
                                        value = line.substr (delimiterPosition + 1);

                                        if (title == "Recipient") {
                                                msg._recipient = value;
                                        }
                                        else if (title == "Sender") {
                                                msg._sender = value;
                                        }
                                        else if (title == "Message") {
                                                msg._message = value;
                                        }
                                       count++;
                                }
                        }
                        allMessage.push_back(msg);
                }
                file.close();
        }
}



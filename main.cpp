#include "Chat.h"
using namespace std;

int main()
{
	setlocale(LC_ALL, "");
	Chat chat;
	char c = 'y'; // условие выхода из цикла
	
	

	if (chat.getReadUsersStatus() == -1) {
		while (c == 'y')
		{
			chat.registration();
			cout << "\nХотите зарегистрировать ещё одного пользователя?" << endl;
			cin >> c;
			cout << endl;
		}
	}
	chat.getChat();
	chat.enter(); // авторизация
	c = 'y';
	while (c != 'n')
	{
		if (chat.getstatus()) // проверяем был ли выполнен вход
		{
			char message;
			cout << "\nХотите отправить сообщение?(y/n)\n";
			cin >> c;

			if (c == 'y')
			{
				cout << "\nВыберите тип отправляемого сообщения: 1-private, 2-public\n";
				cin >> message;
				switch (message)
				{
				case '1':
					chat.sendPrivateMessage();
					break;
				case '2':
					chat.sendPublicMessage();
					break;
				default:
					cout << "Некорректный ввод!";
					break;
				}
			}
			if (c == 'n')
			{
				cout << "\nХотите выполнить вход под другой учетной записью?(y/n)\n";
				cin >> c;
				if (c == 'y')
				{
					chat.exit();
					chat.enter();
				}
				else
				{
					break;
				}
			}
		}
		else
		{
			c = 'n';
			cout << "Вход не выполнен!\n";
		}
	}
	chat.printAllMessage();
}

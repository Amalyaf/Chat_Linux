#include "bad_login.h"

const char* BadLogin::what() const noexcept
{
	return "Пользователь не найден!\nХотите повторить ввод?(y/n): ";
}

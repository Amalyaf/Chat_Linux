#include "bad_password.h"

const char* BadPassword::what() const noexcept
{
	return "Неправильный пароль!\nХотите повторить ввод?(y/n): ";
}

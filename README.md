При запуске происходит проверка на наличие файла userData, которая хранит информацию о ранее зарегистрированных пользователях.
Если файл присутствует, то выполняется чтение данных из всех необходимых файлов, а именно: пользователи, личные и общие сообщения.
Далее есть возможность зарегистрировать новых пользователей.
Для входа требуется ввести логин и пароль.
Выполянется проверка на корректность введённых данных.
После входа выполняется проверка на наличие непрочитанных сообщений для залогиневшегося пользователя.
При наличии сообщений вызывается метод вывода на экран с указанием типа сообщения (личное/ общее) и отправителя.
Далее, при необходимости, можно отправить сообщение (личное/ общее) и выполнить вход под другой учетной записью.
При выходе из программы происходит запись данных о зарегистрированных пользователях и непрочитанных личных и общих сообщениях в отдельные файлы.
Также создаются файлы с количеством всех пользователей, личных и общих сообщений.

#include "ClientToServerConnection.h"

void ClientToServerConnection::doStart(boost::asio::ip::tcp::endpoint* ep)
{
    _socket.async_connect(*ep, MEM_FN1(onConnect, boost::placeholders::_1));
}

void ClientToServerConnection::doStop()
{
    if (!_started) return;
    std::cout << "stopping " << _login << std::endl;
    _started = false;
    _socket.close();
}

bool ClientToServerConnection::isStarted()
{
    return _started;
}

void ClientToServerConnection::onConnect(const boost::system::error_code& err) // Реакция на успешное подключение - отправка данных аутентификации
{
    if (!err)      doWrite("login " + _login + "\n");
    else            doStop();
}

void ClientToServerConnection::onRead(const boost::system::error_code& err, size_t bytes) // Обработка полученных данных после чтения
{
    if (err) doStop();
    if (!isStarted()) return;
    // process the msg
    std::string msg(read_buffer_, bytes);
    if (msg.find("login ") == 0) onLogin();
    else if (msg.find("ping") == 0) onPing(msg);
    else if (msg.find("clients ") == 0) onClients(msg);
    else std::cerr << "invalid msg " << msg << std::endl;
}

void ClientToServerConnection::onLogin()   // Реакция на успешный вход
{
    std::cout << _login << " logged in" << std::endl;
    doAskClients();
}

void ClientToServerConnection::onPing(const std::string& msg)  // Ответ на пинг
{
    std::istringstream in(msg);
    std::string answer;
    in >> answer >> answer;
    if (answer == "client_list_changed") doAskClients();
    else doPostponePing();
}

void ClientToServerConnection::onClients(const std::string& msg)   // Вывод полученного списка пользователей
{
    std::string clients = msg.substr(8);
    std::cout << _login << ", new client list:" << clients;
    doPostponePing();
}

void ClientToServerConnection::onWrite(const boost::system::error_code& err, size_t bytes) // Действия после каждой отправки данных
{
    doRead();
}

void ClientToServerConnection::doRead()    // Чтение данных из сокета
{
    boost::asio::async_read(_socket, boost::asio::buffer(read_buffer_),
        MEM_FN2(readComplete, boost::placeholders::_1, boost::placeholders::_2), MEM_FN2(onRead, boost::placeholders::_1, boost::placeholders::_2));
}

void ClientToServerConnection::doWrite(const std::string& msg)
{
    if (!isStarted()) return;
    _writeBuffer = msg;
    _socket.async_write_some(boost::asio::buffer(_writeBuffer, _writeBuffer.size()),
        MEM_FN2(onWrite, boost::placeholders::_1, boost::placeholders::_2));
}

void ClientToServerConnection::doPing()    // Отправка пинга
{
    doWrite("ping\n");
}

void ClientToServerConnection::doPostponePing()  // Запуск ожидания перед отправкой пинга
{
    int millis = rand() % 7000;
    std::cout << _login << " postponing ping " << millis
        << " millis" << std::endl;
    _timer.expires_from_now(boost::posix_time::millisec(millis));
    _timer.async_wait(MEM_FN(doPing));
}

void ClientToServerConnection::doAskClients() // Отправка запроса на чтение списка пользователей
{
    doWrite("ask_clients\n");
}

size_t ClientToServerConnection::readComplete(const boost::system::error_code& err, size_t bytes)
{
    if (err) return 0;
    bool found = std::find(read_buffer_, read_buffer_ + bytes, '\n') < read_buffer_ + bytes;
    return found ? 0 : 1;
}

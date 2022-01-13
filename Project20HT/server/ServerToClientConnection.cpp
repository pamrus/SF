#include "ServerToClientConnection.h"

ServerToClientConnection::ServerToClientConnection (boost::asio::io_service* ioService) : 
    _ioService(ioService), _socket(*ioService), _timer(*ioService), _started(false), _clientsChanged(false) { }

void ServerToClientConnection::onStart() // Действия после подключения клиента к порту
{
    _started = true;
    clientsArray.push_back(shared_from_this());
    _lastPing = boost::posix_time::microsec_clock::local_time();    // Начитаем отсчёт времени для пинга
    doRead();   // После подключения клиент должен первым прислать сообщение с запросом авторизации - ждём
}

void ServerToClientConnection::onStop()   // Действия при отключении клиента
{
    if (!_started) return;
    _started = false;
    _socket.close();
    boost::shared_ptr<ServerToClientConnection> ptrToSelf = shared_from_this();
    clientsArrayType::iterator iterToSelf = std::find(clientsArray.begin(), clientsArray.end(), ptrToSelf);
    clientsArray.erase(iterToSelf);
    updateClientsChanged();
}

bool ServerToClientConnection::isStarted() const
{
    return _started;
}

boost::asio::ip::tcp::socket& ServerToClientConnection::getSocket()
{
    return _socket;
}

std::string ServerToClientConnection::getLogin() const
{
    return _login;
}

void ServerToClientConnection::setClientsChanged()
{
    _clientsChanged = true;
}

void ServerToClientConnection::onRead(const boost::system::error_code& err, size_t bytes)   // Обработка полученных данных по-окончании чтения
{
    if (err) onStop();
    if (!isStarted()) return;

    std::string msg(read_buffer_, bytes);

    // Ищем в сообщении клиента запрос:
    if (msg.find("login ") == 0) onLogin(msg);                          // Авторизация клиента
        else if (msg.find("ping") == 0) onPing();                       // Пинг-ответ
            else if (msg.find("ask_clients") == 0) onClients();         // Запрос о списке пользователей
                else std::cerr << "invalid msg " << msg << std::endl;   // Запрос не распознан
}

void ServerToClientConnection::onWrite(const boost::system::error_code& err, size_t bytes)  // Действия по-окончании отправки данных
{
    doRead();   // Читаем ответ клиента
}

void ServerToClientConnection::onLogin(const std::string& msg) // Реакция на запрос о входе в чат
{
    //TODO: проверка логина и пароля по базе пользователей, установка нового флага об успешном входе / флаг = не пустое имя
    std::istringstream in(msg);
    in >> _login >> _login;
    std::cout << _login << " logged in" << std::endl;
    doWrite("login ok\n");
    updateClientsChanged();
}

void ServerToClientConnection::onPing() // Ответ на пинг
{
    doWrite(_clientsChanged ? "ping client_list_changed\n" : "ping ok\n"); // Передача клиенту служебной информации об изменениях
    _clientsChanged = false;
}

void ServerToClientConnection::onClients() // Реакция на запрос о списке онлайн-пользователей
{
    std::string msg;
    for (clientsArrayType::const_iterator b = clientsArray.begin(), e = clientsArray.end(); b != e; ++b)
        msg += (*b)->getLogin() + " ";
    doWrite("clients " + msg + "\n");
}

void ServerToClientConnection::onCheckPing()    // Запускающаяся по таймеру проверка онлайн-статуса клиента
{
    boost::posix_time::ptime now = boost::posix_time::microsec_clock::local_time();
    if ((now - _lastPing).total_milliseconds() > 5000) {
        std::cout << "stopping " << _login << " - no ping in time" << std::endl;
        onStop();
    }
    _lastPing = boost::posix_time::microsec_clock::local_time();
}

void ServerToClientConnection::doRead() // Чтение данных из сокета _socket в буфер read_buffer_
{
    boost::asio::async_read(_socket, 
        boost::asio::buffer(read_buffer_),
        MEM_FN2(readComplete, boost::placeholders::_1, boost::placeholders::_2),
        MEM_FN2(onRead, boost::placeholders::_1, boost::placeholders::_2));      

    _timer.expires_from_now(boost::posix_time::millisec(1000));
    _timer.async_wait(MEM_FN(onCheckPing));
}

void ServerToClientConnection::doWrite(const std::string& msg) // Отправка данных в сокет
{
    if (!isStarted()) return;
    _writeBuffer = msg;
    _socket.async_write_some(boost::asio::buffer(_writeBuffer, _writeBuffer.size()),
        MEM_FN2(onWrite, boost::placeholders::_1, boost::placeholders::_2));
}

size_t ServerToClientConnection::readComplete(const boost::system::error_code& err, size_t bytes)   // Проверка завершения чтения
{
    if (err) return 0;
    bool found = std::find(read_buffer_, read_buffer_ + bytes, '\n') < read_buffer_ + bytes;
    // we read one-by-one until we get to enter, no buffering
    return found ? 0 : 1;
}

void updateClientsChanged()
{
    for (clientsArrayType::iterator b = clientsArray.begin(), e = clientsArray.end(); b != e; ++b)
        (*b)->setClientsChanged();  // Установка флага, чтобы при пинге сообщить каждому клиенту об изменениях
}

#include "../../include/server/ChatStateTable.h"

ChatStateTable::ChatStateTable()
{
    //for (unsigned int i = 0; i < TABLE_SIZE; ++i)
    //    addState(defaultTable[i]);
    _currentState = statePrintHelpNotLoggedIn; // Состояние при запуске
}

ChatStateTable::~ChatStateTable() { }

tableStates ChatStateTable::getCurrentState() const
{
    return _currentState;
}

void ChatStateTable::addState(const TableElement& newState)
{
    _stateTable.push_back(newState);
}

void ChatStateTable::addState(const tableStates& stateFrom, const unsigned char& stateKey, const tableStates& stateTo)
{
    TableElement newState = {stateFrom, stateKey, stateTo};
    addState(newState);
}

unsigned int ChatStateTable::changeState(const unsigned char& key)
{
    stateTableType::iterator p_entry = _stateTable.begin();
    stateTableType::iterator p_table_end = _stateTable.end();
    bool state_found = false;

    while ((!state_found) && (p_entry != p_table_end))
    {
        if (p_entry->_current == _currentState)
        {
            if (p_entry->_key == std::toupper(key)) // Делаем алгоритм нечувствительным к регистру символа
            {
                /*
                std::cout << "State found, transitioning"
                    << " from state " << _currentState
                    << ", to state " << p_entry->_next
                    << "\n";
                    */
                _currentState = p_entry->_next;
                state_found = true;
                break;
            }
        }
        ++p_entry;
    }
    if (!state_found)
    {
        //std::cerr << "[FAILED] Transition letter not found, current state not changed.\n";
    }
    return _currentState;
}

void ChatStateTable::changeStateForced(const tableStates& newState)
{
    _currentState = newState;
}

std::string ChatStateTable::availableKeys() const
{
    stateTableType::const_iterator p_entry = _stateTable.begin();
    stateTableType::const_iterator p_table_end = _stateTable.end();
    bool state_found = false;
    std::string keysList;

    while (p_entry != p_table_end)
    {
        if (p_entry->_current == _currentState)
        {
            if (!state_found)
                state_found = true;
            else
                keysList += ", ";
            keysList += p_entry->_key;
        }
        ++p_entry;
    }
    return keysList;
}

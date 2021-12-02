#include "ChatStateTable.h"

ChatStateTable::ChatStateTable()
{
    for (unsigned int i = 0; i < TABLE_SIZE; ++i)
        addState(defaultTable[i]);
    _currentState = 0;
}

ChatStateTable::~ChatStateTable() { }

unsigned int ChatStateTable::getCurrentState()
{
    return _currentState;
}

stateTableType::iterator ChatStateTable::chatStateTableBegin()
{
    return _stateTable.begin();
}

stateTableType::iterator ChatStateTable::chatStateTableEnd()
{
    return _stateTable.end();
}

void ChatStateTable::addState(TableElement newState)
{
    _stateTable.push_back(newState);
}

void ChatStateTable::addState(unsigned int stateFrom, unsigned char stateKey, unsigned int stateTo)
{
    TableElement newState = {stateFrom, stateKey, stateTo};
    addState(newState);
}

unsigned int ChatStateTable::changeState(const unsigned char& key)
{
    stateTableType::iterator p_entry = chatStateTableBegin();
    stateTableType::iterator p_table_end = chatStateTableEnd();
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
        //std::cerr << "Transition letter not found, current state not changed.\n";
    }
    return _currentState;
}

void ChatStateTable::changeStateForced(unsigned int newState)
{
    _currentState = newState;
}

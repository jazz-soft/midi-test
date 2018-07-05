#include <string>
#include <vector>
#include <iostream>
#include "miditest.h"

CSrc::CSrc(const std::string& name) : CMidi(name)
{
    std::cout << "Creating CSrc: " << m_name << "\n";
}


CSrc::~CSrc()
{
    std::cout << "Destroying CSrc\n";
}


bool CSrc::connect()
{
    std::cout << "Connecting CSrc: " << m_name << "\n";
    m_connected = true;
    return true;
}


bool CSrc::disconnect()
{
    std::cout << "Disconnecting CSrc: " << m_name << "\n";
    m_connected = false;
    return true;
}


bool CSrc::emit(const std::vector<unsigned char>& msg)
{
    if (!m_connected || !msg.size()) return false;
    std::cout << m_name << " emits:";
    for (auto c: msg) {
        std::cout << " ";
        std::cout.width(2);
        std::cout.fill('0');
        std::cout << std::hex << (int)c;
    }
    std::cout << "\n";
    return true;
}


CDst::CDst(const std::string& name) : CMidi(name)
{
    std::cout << "Creating CDst: " << m_name << "\n";
}


CDst::~CDst()
{
    std::cout << "Destroying CDst\n";
}


bool CDst::connect()
{
    std::cout << "Connecting CDst: " << m_name << "\n";
    m_connected = true;
    return true;
}


bool CDst::disconnect()
{
    std::cout << "Disconnecting CDst: " << m_name << "\n";
    m_connected = false;
    return true;
}

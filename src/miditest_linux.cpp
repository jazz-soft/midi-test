#include <string>
#include <vector>
#include <iostream>
#include <alsa/asoundlib.h>
#include "miditest.h"
#include "miditest_linux.h"

CMidiSrc* CMidi::CreateSrc(const std::string& name) { return new CSrc(name); }
CMidiDst* CMidi::CreateDst(const std::string& name) { return new CDst(name); }

CSrc::CSrc(const std::string& name) : CMidiSrc(name), m_Handle(0)
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
    if(!m_connected && !snd_rawmidi_open(0, &m_Handle, "virtual", 0)) {
        m_connected = true;
        return true;
    }
    return false;
}


bool CSrc::disconnect()
{
    std::cout << "Disconnecting CSrc: " << m_name << "\n";
    if (m_connected) {
        snd_rawmidi_close(m_Handle);
        m_connected = false;
        return true;
    }
    return false;
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


CDst::CDst(const std::string& name) : CMidiDst(name), m_Handle(0)
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

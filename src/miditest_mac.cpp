#include <string>
#include <vector>
#include <iostream>
#include <CoreMIDI/MIDIServices.h>
#include "miditest.h"
#include "miditest_mac.h"

CMidiSrc* CMidi::CreateSrc(const std::string& name) { return new CSrc(name); }
CMidiDst* CMidi::CreateDst(const std::string& name) { return new CDst(name); }

CSrc::CSrc(const std::string& name) : CMidiSrc(name)
{
    std::cout << "Creating MacOS CSrc: " << m_name << "\n";
    OSStatus result;
    result = MIDIClientCreate(CFSTR("midi-test"), NULL, NULL, &m_Client);
    std::cout << "MIDIClientCreate: " << result << "\n";
}


CSrc::~CSrc()
{
    std::cout << "Destroying MacOS CSrc\n";
}


bool CSrc::connect()
{
    std::cout << "Connecting MacOS CSrc: " << m_name << "\n";
    OSStatus result;
    result = MIDISourceCreate(m_Client, CFStringCreateWithCString(0, m_name.c_str(), kCFStringEncodingUTF8), &m_Midi);
    std::cout << "MIDISourceCreate: " << result << "\n";
    m_connected = true;
    return true;
}


bool CSrc::disconnect()
{
    std::cout << "Disconnecting MacOS CSrc: " << m_name << "\n";
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


CDst::CDst(const std::string& name) : CMidiDst(name)
{
    std::cout << "Creating MacOS CDst: " << m_name << "\n";
}


CDst::~CDst()
{
    std::cout << "Destroying MacOS CDst\n";
}


bool CDst::connect()
{
    std::cout << "Connecting MacOS CDst: " << m_name << "\n";
    m_connected = true;
    return true;
}


bool CDst::disconnect()
{
    std::cout << "Disconnecting MacOS CDst: " << m_name << "\n";
    m_connected = false;
    return true;
}

#include <string>
#include <vector>
#include <iostream>
#include <CoreMIDI/MIDIServices.h>
#include "miditest.h"
#include "miditest_mac.h"

using namespace miditest;


CMidiSrc* CMidi::CreateSrc(const std::string& name) { return new CSrc(name); }
CMidiDst* CMidi::CreateDst(const std::string& name) { return new CDst(name); }


MIDIClientRef GetMidiClient()
{
  static MIDIClientRef client = 0;
  if (!client) {
    MIDIClientCreate(CFSTR("midi-test"), 0, 0, &client);
  }
  return client;
}


CSrc::CSrc(const std::string& name) : CMidiSrc(name) {}


CSrc::~CSrc()
{
    disconnect();
    std::cout << "Destroying MacOS CSrc\n";
}


bool CSrc::connect()
{
    if (!MIDISourceCreate(GetMidiClient(), CFStringCreateWithCString(0, m_name.c_str(), kCFStringEncodingUTF8), &m_Midi)) {
      m_connected = true;
      return true;
    }
    return false;
}


bool CSrc::disconnect()
{
    if (m_connected && !MIDIEndpointDispose(m_Midi)) {
      m_connected = false;
      return true;
    }
    return false;
}


bool CSrc::emit(const std::vector<unsigned char>& msg)
{
    if (!m_connected || !msg.size()) return false;
    size_t bufsize = msg.size() + 64; // where does 64 come from?
    unsigned char* buf = new unsigned char(bufsize);
    MIDIPacketList* pktList = (MIDIPacketList*) buf;
    MIDIPacket *pkt;
    pkt = MIDIPacketListInit(pktList);
    pkt = MIDIPacketListAdd(pktList, bufsize, pkt, 0, msg.size(), msg.data());
    return (!MIDIReceived(m_Midi, pktList));
}


CDst::CDst(const std::string& name) : CMidiDst(name) {}


CDst::~CDst()
{
    disconnect();
    std::cout << "Destroying MacOS CDst\n";
}


void MidiProc(const MIDIPacketList* pktlist, void* data, void* dummy)
{
    MIDIPacket* pkt = (MIDIPacket*)pktlist->packet;
    size_t count = pktlist->numPackets;
    std::vector<unsigned char> msg;
    for (size_t i = 0; i < count; i++) {
        for (size_t j = 0; j < pkt->length; j++) msg.push_back(pkt->data[j]);
    }
    MidiCallback(data, msg);
}

bool CDst::connect()
{
    if (!MIDIDestinationCreate(GetMidiClient(), CFStringCreateWithCString(0, m_name.c_str(), kCFStringEncodingUTF8), MidiProc, this,  &m_Midi)) {
      m_connected = true;
      return true;
    }
    return false;
}


bool CDst::disconnect()
{
    if (m_connected && !MIDIEndpointDispose(m_Midi)) {
      m_connected = false;
      return true;
    }
    return false;
}

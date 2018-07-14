#include <string>
#include <vector>
#include <iostream>
#include <alsa/asoundlib.h>
#include "miditest.h"
#include "miditest_linux.h"

CMidiSrc* CMidi::CreateSrc(const std::string& name) { return new CSrc(name); }
CMidiDst* CMidi::CreateDst(const std::string& name) { return new CDst(name); }

CSrc::CSrc(const std::string& name) : CMidiSrc(name)
{
}


CSrc::~CSrc()
{
    std::cout << "Destroying CSrc\n";
}


bool CSrc::connect()
{
    if(!m_connected) {
        if (snd_seq_open(&m_Seq, "default", SND_SEQ_OPEN_OUTPUT, 0)) {
            std::cout << "error 1\n";
            return false;
        }
        m_Port = snd_seq_create_simple_port(m_Seq, m_name.c_str(), SND_SEQ_PORT_CAP_READ|SND_SEQ_PORT_CAP_SUBS_READ, SND_SEQ_PORT_TYPE_MIDI_GENERIC);
        m_connected = true;
        return true;
    }
    return false;
}


bool CSrc::disconnect()
{
    if (m_connected) {
        snd_seq_close(m_Seq);
        m_connected = false;
        return true;
    }
    return false;
}


bool CSrc::emit(const std::vector<unsigned char>& msg)
{
    if (!m_connected || !msg.size()) return false;
    snd_midi_event_t* midi;
    if (snd_midi_event_new(msg.size(), &midi)) return false;
    snd_seq_event_t ev;
    snd_seq_ev_clear(&ev);
    snd_seq_ev_set_source(&ev, m_Port);
    snd_seq_ev_set_subs(&ev);
    snd_seq_ev_set_direct(&ev);
    snd_midi_event_encode(midi, msg.data(), msg.size(), &ev);
    snd_midi_event_free(midi);
    snd_seq_event_output(m_Seq, &ev);
    snd_seq_drain_output(m_Seq);
    return true;
}


CDst::CDst(const std::string& name) : CMidiDst(name)
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

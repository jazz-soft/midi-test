#include <string>
#include <vector>
#include <iostream>
#include <thread>
#include <alsa/asoundlib.h>
#include "miditest.h"
#include "miditest_linux.h"

using namespace miditest;

CMidiSrc* CMidi::CreateSrc(const std::string& name) { return new CSrc(name); }
CMidiDst* CMidi::CreateDst(const std::string& name) { return new CDst(name); }

CSrc::CSrc(const std::string& name) : CMidiSrc(name)
{
}


CSrc::~CSrc()
{
    //std::cout << "Destroying CSrc\n";
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
}


CDst::~CDst()
{
    //std::cout << "Destroying CDst\n";
}


void CDst::loop(CDst* dst)
{
    int npfd = snd_seq_poll_descriptors_count(dst->m_Seq, POLLIN);
    pollfd *pfd = new pollfd[npfd + 1];
    snd_seq_poll_descriptors(dst->m_Seq, pfd, npfd, POLLIN);
    pfd[npfd].fd = dst->m_Pipe[0];
    pfd[npfd].events = POLLIN;
    npfd++;
    snd_seq_event_t *ev;
    const int SIZE = 256;
    unsigned char buff[SIZE];
    while (true) {
        if (poll(pfd, npfd, 10000) > 0) {
            if (!dst->m_connected) break;
            do {
                snd_seq_event_input(dst->m_Seq, &ev);
                snd_midi_event_t* midi;
                if (snd_midi_event_new(SIZE, &midi)) return;
                long len = snd_midi_event_decode(midi, buff, SIZE, ev);
                for (int i = 0; i < len; i++) dst->push(buff[i]);
                snd_seq_free_event(ev);
            } while (snd_seq_event_input_pending(dst->m_Seq, 0) > 0);
        }
    }
    delete[] pfd;
}


bool CDst::connect()
{
    if(!m_connected) {
        if (pipe(m_Pipe)) return false;
        if (snd_seq_open(&m_Seq, "default", SND_SEQ_OPEN_INPUT, 0)) {
            std::cout << "error 1\n";
            return false;
        }
        m_Port = snd_seq_create_simple_port(m_Seq, m_name.c_str(), SND_SEQ_PORT_CAP_WRITE|SND_SEQ_PORT_CAP_SUBS_WRITE, SND_SEQ_PORT_TYPE_MIDI_GENERIC);
        m_Queue.clear();
        m_connected = true;
        m_Thread = new std::thread(loop, this);
        return true;
    }
    return false;
}


bool CDst::disconnect()
{
    if (m_connected) {
        m_connected = false;
        write(m_Pipe[1], "0", 1);
        snd_seq_delete_simple_port(m_Seq, m_Port);
        snd_seq_close(m_Seq);
        m_Thread->join();
        close(m_Pipe[0]);
        close(m_Pipe[1]);
        return true;
    }
    return false;
}

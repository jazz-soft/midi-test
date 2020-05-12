#include <string>
#include <vector>
#include <iostream>
#include "miditest.h"

using namespace miditest;


static size_t MidiLen(unsigned char c)
{
    switch (c & 0xf0) {
        case 0x80:
        case 0x90:
        case 0xA0:
        case 0xB0:
        case 0xE0: return 3;
        case 0xC0:
        case 0xD0: return 2;
    }
    switch (c) {
        case 0xF2: return 3;
        case 0xF1:
        case 0xF3: return 2;
    }
    return 1;
}


void CMidiDst::push(unsigned char c)
{
    if (c < 0x80 && m_Queue.empty()) return;
    if (c >= 0x80 && m_Queue.size() && m_Queue[0] != 0xf0) m_Queue.clear();
    if (c == 0xf7 && m_Queue.empty()) return;
    m_Queue.push_back(c);
    if (m_Queue.size() < MidiLen(m_Queue[0])) return;
    if (m_Queue[0] == 0xf0 && c != 0xf7) return;
    spit();
}


void CMidiDst::spit()
{
    CMidiData* midi = new CMidiData(this);
    for (size_t i = 0; i < m_Queue.size(); i++) midi->msg.push_back(m_Queue[i]);
    m_Queue.clear();
    MidiCallback(midi);
}

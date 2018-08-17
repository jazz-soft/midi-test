class CSrc : public CMidiSrc
{
public:
    CSrc(const std::string& name);
    ~CSrc();
    bool connect();
    bool disconnect();
    bool emit(const std::vector<unsigned char>& msg);
protected:
    snd_seq_t* m_Seq;
    snd_midi_event_t* m_Encoder;
    int m_Port;
};


class CDst : public CMidiDst
{
public:
    CDst(const std::string& name);
    ~CDst();
    bool connect();
    bool disconnect();
protected:
    static void loop(CDst*);
    snd_seq_t* m_Seq;
    snd_midi_event_t* m_Encoder;
    int m_Port;
    std::thread* m_Thread;
    int m_Pipe[2];
};

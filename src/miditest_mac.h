class CSrc : public CMidiSrc
{
public:
    CSrc(const std::string& name);
    ~CSrc();
    bool connect();
    bool disconnect();
    void set_busy(bool);
    bool emit(const std::vector<unsigned char>& msg);
protected:
    MIDIEndpointRef m_Midi;
};


class CDst : public CMidiDst
{
public:
    CDst(const std::string& name);
    ~CDst();
    bool connect();
    bool disconnect();
    void set_busy(bool);
protected:
    MIDIEndpointRef m_Midi;
};

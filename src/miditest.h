class CMidiSrc;
class CMidiDst;
class CSrc;
class CDst;

class CMidi
{
public:
    virtual ~CMidi() {}
    virtual bool connect() = 0;
    virtual bool disconnect() = 0;
    virtual bool threaded() { return false; }
    const std::string& name() const { return m_name; }
    bool connected() const { return m_connected; }
    static CMidiSrc* CreateSrc(const std::string& name);
    static CMidiDst* CreateDst(const std::string& name);

protected:
    CMidi(const std::string& name) : m_connected(0), m_name(name) {}
    bool m_connected;
    std::string m_name;
friend class CSrc;
friend class CDst;
};


class CMidiSrc : public CMidi
{
public:
    CMidiSrc(const std::string& name) : CMidi(name) {}
    ~CMidiSrc() {}
    virtual bool emit(const std::vector<unsigned char>& msg) = 0;
};


class CMidiDst : public CMidi
{
public:
    CMidiDst(const std::string& name) : CMidi(name), m_RS(0) {}
    ~CMidiDst() {}
    virtual void push(unsigned char c);
    virtual void spit();
protected:
    virtual bool threaded() { return true; }
    unsigned char m_RS;
    std::vector<unsigned char> m_Queue;
};


struct CMidiData
{
    CMidiData(CMidiDst* d) : dst(d) {}
    CMidiDst* dst;
    std::vector<unsigned char> msg;
};


namespace miditest {
void MidiCallback(CMidiData* data);
}

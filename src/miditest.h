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
    CMidiDst(const std::string& name) : CMidi(name) {}
    ~CMidiDst() {}
};

namespace miditest {
void MidiCallback(void* obj, const std::vector<unsigned char>& msg);
}

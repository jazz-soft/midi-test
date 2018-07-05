class CMidi
{
public:
    CMidi(const std::string& name) : m_connected(0), m_name(name) {}
    virtual ~CMidi() {}
    virtual bool connect() = 0;
    virtual bool disconnect() = 0;
    bool m_connected;
    std::string m_name;
};


class CSrc : public CMidi
{
public:
    CSrc(const std::string& name);
    ~CSrc();
    bool connect();
    bool disconnect();
    bool emit(const std::vector<unsigned char>& msg);
};


class CDst : public CMidi
{
public:
    CDst(const std::string& name);
    ~CDst();
    bool connect();
    bool disconnect();
};

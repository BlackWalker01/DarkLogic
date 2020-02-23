#ifndef DBPROPERTY_H
#define DBPROPERTY_H
#include "unordered_map"
#include <vector>
#include "Boolean/iproposition.h"
//#include "theorem.h"
//#include "rule.h"

namespace N_LogicGame {

class IProposition;

template<typename Content_>
class LogicHash
{
public:
    typedef Content_ Content;

    LogicHash(){}
    LogicHash(LogicHash& logicHash)
    {
        for(auto it=logicHash.m_db.begin();it!=logicHash.m_db.end();it++)
        {
            m_db[it->first]=new Content(*it->second);
        }
    }

    void insert(Content& content)
    {
        m_db[content.name()]=&content;
    }

    Content* operator[](const std::string& name)
    {
        return m_db[name];
    }

    bool contains(const std::string& name) const
    {
        return m_db.find(name)!=m_db.end();
    }

    size_t size() const
    {
        return m_db.size();
    }

    std::unordered_map<std::string,Content*> getDb();

    ~LogicHash()
    {
        for(auto it=m_db.begin();it!=m_db.end();it++)
        {
            delete it->second;
        }
        m_db.clear();
    }


private:
    std::unordered_map<std::string,Content*> m_db;
};

typedef LogicHash<IProposition> DbProperty;


}
#endif // DBPROPERTY_H

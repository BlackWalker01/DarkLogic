#ifndef DBRULES_H
#define DBRULES_H

#include "unordered_map"
class Rule;

class DbRules
{
public:
    DbRules();

    Rule* find(std::string name);
    void insert(Rule* prop);
    unsigned int size();

    ~DbRules();

protected:
    std::unordered_map<std::string,Rule*> db;
};

#endif // DBRULES_H

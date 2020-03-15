#ifndef ACTION_H
#define ACTION_H
#include <string>
#include <vector>

namespace N_Logic
{
class Action
{
public:
    Action(const size_t& _id, const std::string& _ruleName, const std::string& _ruleStr, const std::vector<size_t>& _path);

    size_t id() const;
    std::string ruleName() const;
    std::string ruleStr() const;
    std::vector<size_t> path() const;
    std::string toString() const;

private:
    size_t m_id;
    std::string m_ruleName;
    std::string m_ruleStr;
    std::vector<size_t> m_path;
};
}
#endif // ACTION_H

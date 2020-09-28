#include "dbtheorem.h"
#include <random>
#include <ctime>
#include <stdexcept>

DbTheorem::Theorem::Theorem(const std::string& name_, const std::string& content_, unsigned short int elo_):
	m_name(name_), m_content(content_), m_elo(elo_)
{
}

std::string DbTheorem::Theorem::name() const
{
	return m_name;
}

std::string DbTheorem::Theorem::content() const
{
	return m_content;
}

unsigned short int DbTheorem::Theorem::elo() const
{
	return m_elo;
}

DbTheorem::DbTheorem()
{
	std::srand(std::time(nullptr));
	add("Non-contradiction", "!(a&&!a)", 500);
	add("Semi-Identity", "a=>a", 700);
	add("Semi-DoubleNot", "a=>!!a", 750);
	add("Identity", "a<=>a", 1200);
	add("DoubleNot", "a<=>!!a", 1450);
	add("Identity and True", "(a<=>a)&&True", 1750);
	add("Double-Not and True", "(a<=>!!a)&&True", 1750);
	add("Semi-Identity and Semi-Double-Not", "(a=>a)&&(a=>!!a)", 1800);
	add("Contraposition", "(a=>b)=>(!b=>!a)", 1850);
	add("Semi-Identity and Semi-Double-Not and True", "(a=>a)&&(a=>!!a)&&True", 1900);
	add("Contraposition and True", "(a=>b)=>(!b=>!a)", 1950);
	add("ExcludedMiddle", "a||!a", 2400);
	add("Semi-De Morgan law [R]", "!(a&&b)=>(!a||!b)", 2950);
	add("Semi-De Morgan law [L]", "(!a||!b)=>!(a&&b)", 2950);
	add("Semi-De Morgan law (2) [R]", "!(a||b)=>(!a&&!b)", 2950);
	add("Semi-De Morgan law (2) [L]", "(!a&&!b)<=>!(a||b)", 2950);
	add("Double-And", "a&&a<=>a", 3000);
	add("Double-Or", "a||a<=>a", 3000);
	add("Identity-Double-Not", "(a<=>a)&&(a<=>!!a)", 3000);
	add("Peirce law", "((a=>b)=>a)=>a", 3000);
	add("Modus ponens", "((a=>b)&&a)=>b", 3050);
	add("De Morgan law", "!(a&&b)<=>(!a||!b)", 3150);
	add("De Morgan law (2)", "!(a||b)<=>(!a&&!b)", 3250);
	add("Modus tollens", "((a=>b)&&!b)=>!a", 3300);
	add("Modus barbara", "((a=>b)&&(b=>c))=>(a=>c)", 3350);
	add("&& Distributivity", "(a&&(b||c))<=>((a&&b)||(a&&c))", 3350);
	add("|| Distributivity", "(a||(b&&c))<=>((a||b)&&(a||c))", 3350);
}

void DbTheorem::add(const std::string& name_, const std::string& content_, unsigned short int elo_)
{
	if (!m_db.contains(content_))
	{
		m_db.insert({ content_, DbTheorem::Theorem(name_, content_, elo_) });
	}
}

const DbTheorem::Theorem DbTheorem::getRandomTheorem(unsigned short int elo) const
{
	std::vector<std::string> allKeys;
	std::vector<std::string> remainKeys;
	for (auto& it : m_db)
	{
		allKeys.push_back(it.first);
		auto thm = it.second;
		if ((elo - MAX_ELO_RANGE <= thm.elo()) && (thm.elo() <= elo + MAX_ELO_RANGE))
		{
			remainKeys.push_back(it.first);
		}		
	}
	if (remainKeys.size() > 0)
	{
		return m_db.at(remainKeys[rand() % remainKeys.size()]);
	}
	else
	{
		return m_db.at(allKeys[rand() % allKeys.size()]);
	}
}
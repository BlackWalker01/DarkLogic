#pragma once
#include <string>
#include <unordered_map>
#include <climits>
#define MAX_ELO_RANGE 400

class DbTheorem
{
public:
	class Theorem
	{
	public:
		Theorem(const std::string& name_, const std::string& content_, unsigned short int elo_);

		std::string name() const;
		std::string content() const;
		unsigned short int elo() const;
	private:
		std::string m_name;
		std::string m_content;
		unsigned short int m_elo;
	};

	DbTheorem();

	void add(const std::string& name_, const std::string& content_, unsigned short int elo_);
	const Theorem getRandomTheorem(unsigned short int elo = USHRT_MAX) const;

private:
	std::unordered_map<std::string, DbTheorem::Theorem> m_db;
};
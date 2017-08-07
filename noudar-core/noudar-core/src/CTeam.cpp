//
// Created by monty on 04/01/17.
//
#include <iostream>
#include <string>
#include <memory>
#include "CTeam.h"

namespace Knights {
	CTeam::CTeam(std::string aName) : mName(aName) {
	}

	std::string CTeam::getName() {
		return mName;
	}

	std::string CTeam::to_string() const {
		return mName;
	}

	std::ostream& operator<<(std::ostream& os, const CTeam& aTeam) {
		os << aTeam.to_string();

		return os;
	}
}

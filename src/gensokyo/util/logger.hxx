#pragma once

#include "../../includes.hxx"

enum logTypes {
	GNSKY_INFO,
	GNSKY_ERROR
};

namespace gensokyo::util::logger {
	void log(logTypes logType, std::string_view info) noexcept;
};

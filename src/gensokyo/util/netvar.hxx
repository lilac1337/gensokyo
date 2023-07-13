#pragma once

#include "../../includes.hxx"

namespace gensokyo::util {
	void setupNetvars() noexcept;
	void dumpNetvar(const char* baseClass, struct RecvTable* table, uint32_t offset = 0) noexcept;

	inline std::unordered_map<uint32_t, uint32_t> netvars;
}

#define NETVAR(func_name, netvar, type) type& func_name() \
{ \
	static auto offset = gensokyo::util::netvars[gensokyo::util::fnv::HashConst(netvar)]; \
	return *reinterpret_cast<type*>(uint32_t(this) + offset); \
}


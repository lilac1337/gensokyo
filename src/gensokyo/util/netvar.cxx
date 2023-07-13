#include "../../includes.hxx"

void gensokyo::util::setupNetvars() noexcept {
	for (auto clientClass = csgo::interfaces::Client->GetAllClass(); clientClass; clientClass = clientClass->next) {
		if (clientClass->recvTable)
			gensokyo::util::dumpNetvar(clientClass->networkName, clientClass->recvTable);
	}
}

void gensokyo::util::dumpNetvar(const char* baseClass, RecvTable* table, uint32_t offset) noexcept {
	for (auto i = 0; i < table->propsCount; ++i) {
		const auto prop = &table->props[i];

		if (!prop || isdigit(prop->varName[0]))
			continue;

		if (fnv::Hash(prop->varName) == fnv::HashConst("baseclass"))
			continue;

		if (prop->recvType == SendPropType::DATATABLE && prop->dataTable && prop->dataTable->tableName[0] == 'D')
			dumpNetvar(baseClass, prop->dataTable, offset + prop->offset);

		const auto netvarName = std::format("{}->{}", baseClass, prop->varName);

		netvars[fnv::Hash(netvarName.c_str())] = offset + prop->offset;
	}
}

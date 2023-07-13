#include "../../includes.hxx"

void gensokyo::util::logger::log(logTypes logType, std::string_view info) noexcept {
	std::string prefix;

	std::time_t timeStamp = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

	std::string_view timeString = std::ctime(&timeStamp);

	switch (logType) {
	case GNSKY_ERROR:
		prefix = "ERROR";
		break;
	case GNSKY_INFO:
		prefix = "INFO";
		break;
	}

	std::cout << "[" << prefix << "] " << timeString.substr(0, timeString.length() - 1) << " " << info << std::endl;

	if (logType == GNSKY_ERROR)
		system("pause");
}

#pragma once

namespace gensokyo::cheats::options {
	namespace movement {
		inline bool bhopEnabled = false;
		inline bool optiEnabled = true;
		inline bool useSidemoveDir = false;
		inline bool useAtan = true;
		inline bool alignedDir = true;
		inline bool antiWall = true;
		inline bool forceLegalYaw = true;
		inline bool onlyOptiUnder = false;

		inline float optiPercent = 0.8f;
	};

	namespace visuals {
		inline bool alwaysShowCrosshair = true;
		inline bool nameTagsEnabled = true;
		inline bool radarEnabled = false;
	};
};

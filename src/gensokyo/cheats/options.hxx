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

		inline float optiPercent = 0.8f;
	};

	namespace visuals {
		inline bool alwaysShowCrosshair = true;
		inline bool radarEnabled = false;
	};
};

#pragma once


namespace CONFIG
{

	namespace WINDOW
	{
		constexpr unsigned int WIDTH = 1400;
		constexpr unsigned int HEIGHT = 1400;
		constexpr const char* TITLE = "Minesweeper";
	};


	namespace BOARD
	{
		constexpr float WIDTH = 1.55f;
		constexpr float HEIGHT = 1.55f;
		constexpr float X_OFFSET = -0.75f;
		constexpr float Y_OFFSET = -0.75f;
		constexpr unsigned int ROW_COUNT = 35;
		constexpr unsigned int COL_COUNT = 35;
		constexpr unsigned int MINE_COUNT = 178;
	};


	namespace SHADER
	{
		constexpr const char* VERTEX_PATH = "./default.vert";
		constexpr const char* FRAGMENT_PATH = "./default.frag";


		namespace TEXT_ATLAS
		{
			constexpr unsigned int ATLAS_WIDTH = 176;   // 10 numbers * 16 width
			constexpr unsigned int ATLAS_HEIGHT = 16;   // Single row
			constexpr unsigned int CHAR_WIDTH = 16;
			constexpr unsigned int CHAR_HEIGHT = 16;
			constexpr unsigned int NUM_DIGITS = 10;     // 0-9
            constexpr unsigned int FLAG_INDEX = 10;
			constexpr unsigned int TOTAL_CHARS = 11;
		};


		namespace COLORS
		{
			constexpr float CLEAR_COLOR_R = 0.7f;
			constexpr float CLEAR_COLOR_G = 0.0f;
			constexpr float CLEAR_COLOR_B = 0.3f;

			constexpr float MINE_BASE_COLOR_R = 0.6f;
			constexpr float MINE_BASE_COLOR_G = 0.6f;
			constexpr float MINE_BASE_COLOR_B = 0.6f;

		};

	};

};
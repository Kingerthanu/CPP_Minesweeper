#pragma once



#include "config.h"
#include <glad/glad.h>
#include <random>
#include "vertex.h"
#include <stack>


class Board
{

	private:

		// ~~~~~~ Initialization State ~~~~~~
		bool isInit = false;


		// ~~~~~~ Board Window ~~~~~~
		float width, height;
		float xOffset, yOffset;


		// ~~~~~~ Board State ~~~~~~
		enum class CellState { HIDDEN, REVEALED, FLAGGED };

		struct Cell
		{

			CellState state = CellState::HIDDEN;
			unsigned int adjacentMines = 0, vboOffset;
			bool isMine = false;

		};


		// ~~~~~~ Board Data ~~~~~~
		Cell** cells;
		unsigned int row_count, col_count;
		unsigned int mineCount;


		// ~~~~~~ Board Rendering ~~~~~~
		GLuint VBO, EBO;


		// ~~~~~ Vertex Data ~~~~~~
		Vertex* vertices;
		unsigned int* indices;


		// ~~~~~~ Flood Fill (Avoid Recursion) ~~~~~~
		bool visited[CONFIG::BOARD::ROW_COUNT][CONFIG::BOARD::COL_COUNT] = { false };


		// ~~~~~ Text Rendering ~~~~~~
		GLuint proceduralAtlasTextureID;


		/*
		
			Desc: Generates A Procedural Texture Atlas Containing Digit Patterns
			For Numbers 0-9 And A Flag Symbol For Use In Rendering Game Elements.

			Preconditions:
				1.) OpenGL Context Must Be Active And Valid
				2.) CONFIG::SHADER::TEXT_ATLAS Constants Must Be Properly Defined

			Postconditions:
				1.) proceduralAtlasTextureID Will Contain A Valid OpenGL Texture ID
				2.) Texture Will Be Uploaded To GPU Memory With Digit And Flag Patterns
				3.) Texture Parameters Will Be Set For Nearest Neighbor Filtering

		*/
		void generateDigits()
		{

			// Generate Texture
			glGenTextures(1, &this->proceduralAtlasTextureID);
			glBindTexture(GL_TEXTURE_2D, this->proceduralAtlasTextureID);

			// Texture Parameters
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

			// Initialize Texture Data (Grayscale: 0 = Black, 255 = White)
			unsigned char data[CONFIG::SHADER::TEXT_ATLAS::ATLAS_WIDTH * CONFIG::SHADER::TEXT_ATLAS::ATLAS_HEIGHT];
			memset(data, 0, sizeof(data)); // Initialize To Black

			// 8x16 Bit Patterns For Digits 0-9 (Each Row Is A Byte)
			const unsigned char digitPatterns[10][16] = {
				// 0
				{0b01111110, 0b11000011, 0b11000011, 0b11000011,
				 0b11000011, 0b11001011, 0b11010011, 0b11100011,
				 0b11000011, 0b11000011, 0b11000011, 0b11000011,
				 0b11000011, 0b11000011, 0b11000011, 0b01111110},

				 // 1
				 {0b00011000, 0b00111000, 0b01111000, 0b00011000,
				  0b00011000, 0b00011000, 0b00011000, 0b00011000,
				  0b00011000, 0b00011000, 0b00011000, 0b00011000,
				  0b00011000, 0b00011000, 0b00011000, 0b11111111},

				  // 2
				  {0b01111110, 0b11000011, 0b11000011, 0b00000011,
				   0b00000011, 0b00000110, 0b00001100, 0b00011000,
				   0b00110000, 0b01100000, 0b11000000, 0b11000000,
				   0b11000000, 0b11000000, 0b11111111, 0b11111111},

				   // 3
				   {0b01111110, 0b11000011, 0b11000011, 0b00000011,
					0b00000011, 0b00000110, 0b00111100, 0b00000110,
					0b00000011, 0b00000011, 0b11000011, 0b11000011,
					0b11000011, 0b11000011, 0b11000011, 0b01111110},

					// 4
					{0b11000011, 0b11000011, 0b11000011, 0b11000011,
					 0b11000011, 0b11000011, 0b11111111, 0b11111111,
					 0b00000011, 0b00000011, 0b00000011, 0b00000011,
					 0b00000011, 0b00000011, 0b00000011, 0b00000011},

					 // 5
					 {0b11111111, 0b11111111, 0b11000000, 0b11000000,
					  0b11000000, 0b11000000, 0b11111110, 0b01111111,
					  0b00000011, 0b00000011, 0b00000011, 0b11000011,
					  0b11000011, 0b11000011, 0b11000011, 0b01111110},

					  // 6
					  {0b01111110, 0b11000011, 0b11000011, 0b11000000,
					   0b11000000, 0b11000000, 0b11111110, 0b11111111,
					   0b11000011, 0b11000011, 0b11000011, 0b11000011,
					   0b11000011, 0b11000011, 0b11000011, 0b01111110},

					   // 7
					   {0b11111111, 0b11111111, 0b00000011, 0b00000011,
						0b00000110, 0b00000110, 0b00001100, 0b00001100,
						0b00011000, 0b00011000, 0b00110000, 0b00110000,
						0b01100000, 0b01100000, 0b11000000, 0b11000000},

						// 8
						{0b01111110, 0b11000011, 0b11000011, 0b11000011,
						 0b11000011, 0b01111110, 0b01111110, 0b01111110,
						 0b11000011, 0b11000011, 0b11000011, 0b11000011,
						 0b11000011, 0b11000011, 0b11000011, 0b01111110},

						 // 9
						 {0b01111110, 0b11000011, 0b11000011, 0b11000011,
						  0b11000011, 0b11000011, 0b11000011, 0b11111111,
						  0b01111111, 0b00000011, 0b00000011, 0b00000011,
						  0b11000011, 0b11000011, 0b11000011, 0b01111110}
			};

			const unsigned char flagPattern[16] = {
				0b00000100, // Flagpole
				0b11111100, // Full flag with pole
				0b11111100, // Full flag
				0b11111100, // Full flag
				0b11111100, // Full flag
				0b11111100, // Full flag
				0b11111100, // Full flag
				0b00001000, // Just pole
				0b00001000, // Pole
				0b00001000, // Pole
				0b00001000, // Pole
				0b00001000, // Pole
				0b00001000, // Pole
				0b00001000, // Pole
				0b00001000, // Pole
				0b11111111  // Base
			};

			// Generate Each Digit In The Atlas With Padding
			for (unsigned int digit = 0; digit < 10; ++digit) {
				unsigned int charSlotWidth = CONFIG::SHADER::TEXT_ATLAS::CHAR_WIDTH;
				unsigned int charSlotHeight = CONFIG::SHADER::TEXT_ATLAS::ATLAS_HEIGHT;

				// Calculate Padding (20% On Each Side)
				float paddingRatio = 0.2f;
				unsigned int horizontalPadding = (unsigned int)(charSlotWidth * paddingRatio);
				unsigned int verticalPadding = (unsigned int)(charSlotHeight * paddingRatio);

				// Calculate Actual Character Area
				unsigned int actualCharWidth = charSlotWidth - 2 * horizontalPadding;
				unsigned int actualCharHeight = charSlotHeight - 2 * verticalPadding;

				// Starting Position For This Digit (With Padding)
				unsigned int startX = digit * charSlotWidth + horizontalPadding;
				unsigned int startY = verticalPadding;

				// Process Each Row Of The Digit Pattern
				for (unsigned int row = 0; row < 16; ++row) {
					unsigned char pattern = digitPatterns[digit][row];

					// Scale Row To Fit In Actual Character Area
					unsigned int scaledY = startY + (row * actualCharHeight) / 16;

					// Process Each Bit In The Row
					for (unsigned int col = 0; col < 8; ++col) {
						if (pattern & (0b10000000 >> col)) {
							// Scale Column To Fit In Actual Character Area
							unsigned int scaledX = startX + (col * actualCharWidth) / 8;

							// Draw 2x2 Pixel Block For Better Visibility
							for (unsigned int scaleY = 0; scaleY < 2 && scaledY + scaleY < CONFIG::SHADER::TEXT_ATLAS::ATLAS_HEIGHT; ++scaleY) {
								for (unsigned int scaleX = 0; scaleX < 2 && scaledX + scaleX < CONFIG::SHADER::TEXT_ATLAS::ATLAS_WIDTH; ++scaleX) {
									unsigned int x = scaledX + scaleX;
									unsigned int y = scaledY + scaleY;

									if (x < CONFIG::SHADER::TEXT_ATLAS::ATLAS_WIDTH && y < CONFIG::SHADER::TEXT_ATLAS::ATLAS_HEIGHT) {
										data[y * CONFIG::SHADER::TEXT_ATLAS::ATLAS_WIDTH + x] = 255; // White Pixel
									}
								}
							}
						}
					}
				}
			}

			{
				unsigned int charSlotWidth = CONFIG::SHADER::TEXT_ATLAS::CHAR_WIDTH;
				unsigned int charSlotHeight = CONFIG::SHADER::TEXT_ATLAS::ATLAS_HEIGHT;

				// Calculate Padding (20% On Each Side)
				float paddingRatio = 0.2f;
				unsigned int horizontalPadding = (unsigned int)(charSlotWidth * paddingRatio);
				unsigned int verticalPadding = (unsigned int)(charSlotHeight * paddingRatio);

				// Calculate Actual Character Area
				unsigned int actualCharWidth = charSlotWidth - 2 * horizontalPadding;
				unsigned int actualCharHeight = charSlotHeight - 2 * verticalPadding;

				// Starting Position For This Digit (With Padding)
				unsigned int startX = CONFIG::SHADER::TEXT_ATLAS::NUM_DIGITS * charSlotWidth + horizontalPadding;
				unsigned int startY = verticalPadding;

				// Process Each Row Of The Flag Pattern
				for (unsigned int row = 0; row < 16; ++row) {
					unsigned char pattern = flagPattern[row];
					unsigned int scaledY = startY + (row * actualCharHeight) / 16;

					// Process Each Bit In The Row
					for (unsigned int col = 0; col < 8; ++col) {
						if (pattern & (0b10000000 >> col)) {
							// Scale Column To Fit In Actual Character Area
							unsigned int scaledX = startX + (col * actualCharWidth) / 8;

							// Draw 2x2 Pixel Block For Better Visibility
							for (unsigned int scaleY = 0; scaleY < 2 && scaledY + scaleY < CONFIG::SHADER::TEXT_ATLAS::ATLAS_HEIGHT; ++scaleY) {
								for (unsigned int scaleX = 0; scaleX < 2 && scaledX + scaleX < CONFIG::SHADER::TEXT_ATLAS::ATLAS_WIDTH; ++scaleX) {
									unsigned int x = scaledX + scaleX;
									unsigned int y = scaledY + scaleY;

									if (x < CONFIG::SHADER::TEXT_ATLAS::ATLAS_WIDTH && y < CONFIG::SHADER::TEXT_ATLAS::ATLAS_HEIGHT) {
										data[y * CONFIG::SHADER::TEXT_ATLAS::ATLAS_WIDTH + x] = 255; // White Pixel
									}
								}
							}
						}
					}
				}
			}

			// Upload Texture Data To GPU
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, CONFIG::SHADER::TEXT_ATLAS::ATLAS_WIDTH, CONFIG::SHADER::TEXT_ATLAS::ATLAS_HEIGHT, 0, GL_RED, GL_UNSIGNED_BYTE, data);
	
		}


		/*
		
			Desc: Sets The UV Coordinates For A Cell To Display The Flag Symbol
			From The Procedural Texture Atlas.

			Preconditions:
				1.) row And col Must Be Valid Cell Coordinates Within The Board
				2.) vertices Array Must Be Properly Initialized
				3.) Texture Atlas Must Have Been Generated With Flag Symbol

			Postconditions:
				1.) The Four Vertices Of The Specified Cell Will Have UV Coordinates
				2.) UV Coordinates Will Map To The Flag Symbol In The Texture Atlas

		*/
		void setFlagUVs(unsigned int row, unsigned int col)
		{

			// Calculate UV Coordinates For The Flag
			float symbolWidth = 1.0f / CONFIG::SHADER::TEXT_ATLAS::TOTAL_CHARS;

			float paddingRatio = 0.0f; // Match Your Digit Padding
			float actualSymbolWidth = symbolWidth * (1.0f - 2.0f * paddingRatio);
			float paddingWidth = symbolWidth * paddingRatio;

			float u1 = CONFIG::SHADER::TEXT_ATLAS::FLAG_INDEX * symbolWidth + paddingWidth;
			float v1 = paddingRatio;
			float u2 = u1 + actualSymbolWidth;
			float v2 = 1.0f - paddingRatio;

			unsigned int vertexIndex = (row * this->col_count + col) * 4;

			// Apply flag UV coordinates
			this->vertices[vertexIndex + 0].uvs[0] = u2; this->vertices[vertexIndex + 0].uvs[1] = v1; // Bottom Right
			this->vertices[vertexIndex + 1].uvs[0] = u2; this->vertices[vertexIndex + 1].uvs[1] = v2; // Top Right  
			this->vertices[vertexIndex + 2].uvs[0] = u1; this->vertices[vertexIndex + 2].uvs[1] = v2; // Top Left
			this->vertices[vertexIndex + 3].uvs[0] = u1; this->vertices[vertexIndex + 3].uvs[1] = v1; // Bottom Left
		}


		/*

			Desc: Sets The UV Coordinates For A Cell To Display A Specific Number
			From The Procedural Texture Atlas.

			Preconditions:
				1.) row And col Must Be Valid Cell Coordinates Within The Board
				2.) number Must Be Between 0 And 9 Inclusive
				3.) vertices Array Must Be Properly Initialized
				4.) Texture Atlas Must Have Been Generated With Number Patterns

			Postconditions:
				1.) The Four Vertices Of The Specified Cell Will Have UV Coordinates
				2.) UV Coordinates Will Map To The Specified Number In The Texture Atlas
				3.) If number > 9, Function Will Return Without Modification

		*/
		void setNumberUVs(unsigned int row, unsigned int col, unsigned int number)
		{
			if (number > 9) return;

			// Calculate UV Coordinates For The Digit With Padding
			float digitWidth = 1.0f / CONFIG::SHADER::TEXT_ATLAS::TOTAL_CHARS;

			// Add Padding Within The Texture Atlas Slot
			float paddingRatio = 0.0f;  // 20% Padding On Each Side = 60% Character Size
			float actualDigitWidth = digitWidth * (1.0f - 2.0f * paddingRatio);
			float paddingWidth = digitWidth * paddingRatio;

			float u1 = number * digitWidth + paddingWidth;
			float v1 = paddingRatio;  // Top And Bottom Padding
			float u2 = u1 + actualDigitWidth;
			float v2 = 1.0f - paddingRatio;

			unsigned int vertexIndex = (row * this->col_count + col) * 4;

			// Apply The Padded UV Coordinates
			this->vertices[vertexIndex + 0].uvs[0] = u2; this->vertices[vertexIndex + 0].uvs[1] = v1; // Bottom Right
			this->vertices[vertexIndex + 1].uvs[0] = u2; this->vertices[vertexIndex + 1].uvs[1] = v2; // Top Right  
			this->vertices[vertexIndex + 2].uvs[0] = u1; this->vertices[vertexIndex + 2].uvs[1] = v2; // Top Left
			this->vertices[vertexIndex + 3].uvs[0] = u1; this->vertices[vertexIndex + 3].uvs[1] = v1; // Bottom Left
	
		}


		/*

			Desc: Creates And Initializes The Minesweeper Board By Allocating Memory
			For Cells And Setting Up Vertex Data For Each Cell's Quad Geometry.

			Preconditions:
				1.) width, height, xOffset, yOffset Must Be Initialized
				2.) row_count And col_count Must Be Valid Board Dimensions
				3.) vertices And indices Arrays Must Be Allocated

			Postconditions:
				1.) cells 2D Array Will Be Allocated And Initialized
				2.) vertices Array Will Contain Quad Geometry For Each Cell
				3.) indices Array Will Contain Triangle Indices For Rendering
				4.) Each Cell Will Have Its vboOffset Set Correctly

		*/
		void populateBoard()
		{
			this->cells = new Cell * [this->row_count];

			// Calculate Cell Dimensions
			float cellWidth = this->width / this->col_count;
			float cellHeight = this->height / this->row_count;

			for (unsigned int i = 0; i < this->row_count; ++i)
			{
				this->cells[i] = new Cell[this->col_count];

				for (unsigned int j = 0; j < this->col_count; ++j)
				{
					// Use Cell Dimensions For Positioning
					float x = this->xOffset + j * cellWidth;
					float y = this->yOffset + i * cellHeight;
					unsigned int vertexIndex = (i * this->col_count + j) * 4;
					unsigned int indexIndex = (i * this->col_count + j) * 6;
					this->cells[i][j].vboOffset = vertexIndex;

					// Define Vertices Using Cell Dimensions
					this->vertices[vertexIndex + 0] = { { x + cellWidth, y + cellHeight, 0.0f }, { CONFIG::SHADER::COLORS::MINE_BASE_COLOR_R,
								   CONFIG::SHADER::COLORS::MINE_BASE_COLOR_G,
								   CONFIG::SHADER::COLORS::MINE_BASE_COLOR_B },
								   { 0.0f, 0.0f } }; // Top Right
					this->vertices[vertexIndex + 1] = { { x + cellWidth, y, 0.0f },  { CONFIG::SHADER::COLORS::MINE_BASE_COLOR_R,
								   CONFIG::SHADER::COLORS::MINE_BASE_COLOR_G,
								   CONFIG::SHADER::COLORS::MINE_BASE_COLOR_B },
								   { 0.0f, 0.0f } }; // Bottom Right
					this->vertices[vertexIndex + 2] = { { x, y, 0.0f },  { CONFIG::SHADER::COLORS::MINE_BASE_COLOR_R,
								   CONFIG::SHADER::COLORS::MINE_BASE_COLOR_G,
								   CONFIG::SHADER::COLORS::MINE_BASE_COLOR_B },
								   { 0.0f, 0.0f } }; // Bottom Left
					this->vertices[vertexIndex + 3] = { { x, y + cellHeight, 0.0f },  { CONFIG::SHADER::COLORS::MINE_BASE_COLOR_R,
								   CONFIG::SHADER::COLORS::MINE_BASE_COLOR_G,
								   CONFIG::SHADER::COLORS::MINE_BASE_COLOR_B },
								   { 0.0f, 0.0f } }; // Top Left

					// Indices Remain The Same
					this->indices[indexIndex + 0] = vertexIndex + 0;
					this->indices[indexIndex + 1] = vertexIndex + 1;
					this->indices[indexIndex + 2] = vertexIndex + 3;
					this->indices[indexIndex + 3] = vertexIndex + 1;
					this->indices[indexIndex + 4] = vertexIndex + 2;
					this->indices[indexIndex + 5] = vertexIndex + 3;
				}
			}

		}

		unsigned int linesOffset;

		/*
		
			Desc: Generates Grid Line Geometry To Create Visual Separation Between
			Minesweeper Cells For Better Game Visibility.

			Preconditions:
				1.) width, height, xOffset, yOffset Must Be Initialized
				2.) row_count And col_count Must Be Valid Board Dimensions
				3.) vertices And indices Arrays Must Be Allocated With Sufficient Space

			Postconditions:
				1.) linesOffset Will Be Set To The Starting Index For Grid Lines
				2.) vertices Array Will Contain Line Geometry After Cell Data
				3.) indices Array Will Contain Line Indices For Rendering Grid
				4.) Both Horizontal And Vertical Grid Lines Will Be Generated

		*/
		void generateGridLines()
		{

			// Set Where We Start Writing Grid Lines In The Vertex Buffer
			this->linesOffset = this->row_count * this->col_count * 4;

			float cellWidth = this->width / this->col_count;
			float cellHeight = this->height / this->row_count;

			unsigned int currentVertexIndex = this->linesOffset;
			unsigned int currentIndexOffset = this->row_count * this->col_count * 6;

			// Vertical Lines
			for (unsigned int i = 0; i <= this->col_count; ++i)
			{
				float x = this->xOffset + i * cellWidth;
				float yStart = this->yOffset;
				float yEnd = this->yOffset + this->height;

				this->vertices[currentVertexIndex] = { { x, yStart, 0.0f }, { 0.1f, 0.1f, 0.1f }, {0.0f, 0.0f} };
				this->vertices[currentVertexIndex + 1] = { { x, yEnd, 0.0f }, { 0.1f, 0.1f, 0.1f }, {0.0f, 0.0f} };

				this->indices[currentIndexOffset] = currentVertexIndex;
				this->indices[currentIndexOffset + 1] = currentVertexIndex + 1;

				currentVertexIndex += 2;
				currentIndexOffset += 2;
			}

			// Horizontal Lines
			for (unsigned int i = 0; i <= this->row_count; ++i)
			{
				float y = this->yOffset + i * cellHeight;
				float xStart = this->xOffset;
				float xEnd = this->xOffset + this->width;

				this->vertices[currentVertexIndex] = { { xStart, y, 0.0f }, { 0.1f, 0.1f, 0.1f }, {0.0f, 0.0f} };
				this->vertices[currentVertexIndex + 1] = { { xEnd, y, 0.0f }, { 0.1f, 0.1f, 0.1f }, {0.0f, 0.0f} };

				this->indices[currentIndexOffset] = currentVertexIndex;
				this->indices[currentIndexOffset + 1] = currentVertexIndex + 1;

				currentVertexIndex += 2;
				currentIndexOffset += 2;
			}

		}


		/*
		
			Desc: Resets All Cells On The Board To Their Default Hidden State
			Clearing All Mine And Adjacent Count Data.

			Preconditions:
				1.) cells Array Must Be Properly Allocated And Initialized
				2.) row_count And col_count Must Be Valid Board Dimensions

			Postconditions:
				1.) All Cells Will Have state Set To CellState::HIDDEN
				2.) All Cells Will Have adjacentMines Reset To 0
				3.) All Cells Will Have isMine Set To false

		*/
		void resetBoard()
		{

			for (unsigned int i = 0; i < this->row_count; ++i)
			{
				for (unsigned int j = 0; j < this->col_count; ++j)
				{
					this->cells[i][j].state = CellState::HIDDEN;
					this->cells[i][j].adjacentMines = 0;
					this->cells[i][j].isMine = false;
				}
			}

		}


		/*
		
			Desc: Randomly Places Mines On The Board And Calculates Adjacent Mine
			Counts For All Non-Mine Cells.

			Preconditions:
				1.) cells Array Must Be Properly Allocated And Initialized
				2.) row_count, col_count, And mineCount Must Be Valid
				3.) Random Number Generator Must Be Seeded

			Postconditions:
				1.) Mines Will Be Randomly Distributed Across The Board
				2.) Each Non-Mine Cell Will Have Correct adjacentMines Count
				3.) All Cells Adjacent To Mines Will Have Their Count Updated

		*/
		void loadBoard()
		{

			for (unsigned int i = 0; i < this->row_count; ++i)
			{
				for (unsigned int j = 0; j < this->col_count; ++j)
				{
					if (std::rand() / (float)RAND_MAX < (static_cast<float>(this->mineCount) / (this->row_count * this->col_count)))
					{
						this->cells[i][j].isMine = true;

						// Update Adjacent Cells
						for (int di = -1; di <= 1; ++di)
						{
							for (int dj = -1; dj <= 1; ++dj)
							{
								int ni = i + di;
								int nj = j + dj;

								if (ni >= 0 && ni < this->row_count && nj >= 0 && nj < this->col_count && !(di == 0 && dj == 0))
								{
									this->cells[ni][nj].adjacentMines++;
								}
							}
						}
					}
				}
			}

		}


		/*
		
			Desc: Implements A Flood Fill Algorithm To Automatically Reveal
			Connected Empty Cells When A Cell With No Adjacent Mines Is Clicked.

			Preconditions:
				1.) row And col Must Be Valid Cell Coordinates
				2.) cells Array Must Be Properly Initialized
				3.) visited Array Must Be Available For Tracking

			Postconditions:
				1.) All Connected Empty Cells Will Be Revealed
				2.) Cells With Adjacent Mines Will Be Revealed But Not Expanded
				3.) visited Array Will Track All Processed Cells
				4.) Cell Colors Will Be Updated For All Revealed Cells

		*/
		void bloomOut(unsigned int row, unsigned int col)
		{

			if (this->cells[row][col].adjacentMines > 0 || this->cells[row][col].isMine)
			{
				return; // Only Bloom Out From Cells With 0 Adjacent Mines
			}

			std::stack<std::pair<unsigned int, unsigned int>> toVisit;

			// Start From All Revealed Cells With 0 Adjacent Mines
			toVisit.push({ row, col });
			visited[row][col] = true;

			while (!toVisit.empty())
			{
				std::pair<unsigned int, unsigned int> current = toVisit.top();
				toVisit.pop();

				unsigned int r = current.first;
				unsigned int c = current.second;
				this->cells[r][c].state = CellState::REVEALED;
				this->colorCell(r, c);

				for (int dr = -1; dr <= 1; ++dr)
				{
					for (int dc = -1; dc <= 1; ++dc)
					{
						int nr = r + dr;
						int nc = c + dc;

						if (nr >= 0 && nr < this->row_count && nc >= 0 && nc < this->col_count && !visited[nr][nc])
						{
							visited[nr][nc] = true;

							if (this->cells[nr][nc].state == CellState::HIDDEN && this->cells[nr][nc].adjacentMines == 0 && !this->cells[nr][nc].isMine)
							{
								toVisit.push({ nr, nc });
							}
						}
					}
				}
			}

		}

		/*
		
			Desc: Updates The Visual Appearance Of A Cell Based On Its Type
			Setting Appropriate Colors And Textures For Mines, Numbers, Or Empty Cells.

			Preconditions:
				1.) row And col Must Be Valid Cell Coordinates
				2.) vertices Array Must Be Properly Initialized
				3.) cells Array Must Contain Valid Cell Data

			Postconditions:
				1.) Cell Vertices Will Have Colors Updated Based On Cell Type
				2.) Mine Cells Will Be Colored Red
				3.) Cells With Adjacent Mines Will Be Light Gray With Number Texture
				4.) Empty Safe Cells Will Be Colored White

		*/
		void colorCell(unsigned int row, unsigned int col)
		{

			// Update Vertex Colors For Visual Feedback
			unsigned int vertexIndex = (row * this->col_count + col) * 4;

			// Change Color Based On Cell Type
			if (this->cells[row][col].isMine)
			{
				// Red For Mines
				for (int i = 0; i < 4; ++i)
				{
					this->vertices[vertexIndex + i].color[0] = 1.0f;
					this->vertices[vertexIndex + i].color[1] = 0.0f;
					this->vertices[vertexIndex + i].color[2] = 0.0f;
				}
			}
			else if (this->cells[row][col].state == CellState::FLAGGED)
			{
				// Yellow For Flagged Cells
				for (int i = 0; i < 4; ++i)
				{
					this->vertices[vertexIndex + i].color[0] = 1.0f;
					this->vertices[vertexIndex + i].color[1] = 1.0f;
					this->vertices[vertexIndex + i].color[2] = 0.0f;
				}
				this->setFlagUVs(row, col);
			}
			else
			{
				if (this->cells[row][col].adjacentMines > 0)
				{
					// Dark White For Safe Cells
					for (int i = 0; i < 4; ++i)
					{
						this->vertices[vertexIndex + i].color[0] = 0.9f;
						this->vertices[vertexIndex + i].color[1] = 0.9f;
						this->vertices[vertexIndex + i].color[2] = 0.9f;
					}
					this->setNumberUVs(row, col, this->cells[row][col].adjacentMines);
				}
				else
				{
					// White For Safe Cells
					for (int i = 0; i < 4; ++i)
					{
						this->vertices[vertexIndex + i].color[0] = 1.0f;
						this->vertices[vertexIndex + i].color[1] = 1.0f;
						this->vertices[vertexIndex + i].color[2] = 1.0f;
					}
				}
			}

		}



	public:

		/*
		
			Desc: Constructs A New Board Instance With Specified Dimensions
			And Mine Count, Initializing All Board Parameters.

			Preconditions:
				1.) All Parameters Must Be Valid Positive Values
				2.) CONFIG Values Must Be Properly Defined

			Postconditions:
				1.) Board Instance Will Be Created With Specified Dimensions
				2.) All Member Variables Will Be Initialized
				3.) Board Will Be Ready For initialize() Call

		*/
		Board(const float& width = CONFIG::BOARD::WIDTH,
			const float& height = CONFIG::BOARD::HEIGHT,
			const float& xOffset = CONFIG::BOARD::X_OFFSET,
			const float& yOffset = CONFIG::BOARD::Y_OFFSET,
			const unsigned int& row_count = CONFIG::BOARD::ROW_COUNT,
			const unsigned int& col_count = CONFIG::BOARD::COL_COUNT,
			const unsigned int& mineCount = CONFIG::BOARD::MINE_COUNT) :
			width(width), height(height), xOffset(xOffset), yOffset(yOffset),
			row_count(row_count), col_count(col_count), mineCount(mineCount)
		{
		}


		/*
		
			Desc: Handles Cell Updates When A Cell Is Clicked, Revealing The Cell
			And Triggering Game Over If A Mine Is Hit.

			Preconditions:
				1.) row And col Must Be Valid Cell Coordinates
				2.) cells Array Must Be Properly Initialized
				3.) Cell Must Not Be In FLAGGED State

			Postconditions:
				1.) If Cell Is A Mine, All Mines Will Be Revealed And Game Over Triggered
				2.) If Cell Is Safe, It Will Be Revealed With Appropriate Visual Updates
				3.) If Cell Is Empty, bloomOut Will Be Called For Flood Fill
				4.) Vertex Buffer Will Be Updated If Mine Is Hit

		*/
		void updateCell(unsigned int row, unsigned int col)
		{

			if (this->cells[row][col].state == CellState::FLAGGED)
			{
				return; // Ignore Clicks On Revealed Or Flagged Cells
			}

			if (this->cells[row][col].isMine)
			{
				// Reveal All Mines
				for (unsigned int i = 0; i < this->row_count; ++i)
				{
					for (unsigned int j = 0; j < this->col_count; ++j)
					{
						if (this->cells[i][j].isMine)
						{
							this->cells[i][j].state = CellState::REVEALED;
							this->colorCell(i, j);
						}
					}
				}
				this->updateVertexBuffer();
				std::cout << "Game Over! You clicked on a mine at (" << row << ", " << col << ").\n";
				return;
			}

			// Update Cell State
			if (this->cells[row][col].state == CellState::HIDDEN)
			{
				this->cells[row][col].state = CellState::REVEALED;
				this->bloomOut(row, col);
				this->colorCell(row, col);
			}

		}


		/*
		
			Desc: Processes Mouse Clicks On The Board, Converting Screen Coordinates
			To Board Coordinates And Handling Left/Right Click Actions.

			Preconditions:
				1.) normalizedX And normalizedY Must Be In Valid Screen Space
				2.) Board Must Be Properly Initialized
				3.) isRightClick Indicates Left (false) Or Right (true) Click

			Postconditions:
				1.) If Click Is Within Board Bounds, Appropriate Action Will Be Taken
				2.) Left Click Will Call updateCell For Cell Revelation
				3.) Right Click Will Call toggleFlag For Flag Management
				4.) Vertex Buffer Will Be Updated After Any Cell Modification

		*/
		void handleClick(float normalizedX, float normalizedY, bool isRightClick)
		{

			// Convert Normalized Coordinates To Board Indices
			if (normalizedX < this->xOffset || normalizedX > this->xOffset + this->width ||
				normalizedY < this->yOffset || normalizedY > this->yOffset + this->height)
			{
				return; // Click Outside The Board
			}

			float cellWidth = this->width / this->col_count;
			float cellHeight = this->height / this->row_count;
			unsigned int col = static_cast<unsigned int>((normalizedX - this->xOffset) / cellWidth);
			unsigned int row = static_cast<unsigned int>((normalizedY - this->yOffset) / cellHeight);

			if (row < this->row_count && col < this->col_count)
			{
				if (isRightClick)
				{
					this->toggleFlag(row, col);
				}
				else
				{
					this->updateCell(row, col);
				}
				this->updateVertexBuffer();
			}

		}


		/*
		
			Desc: Toggles The Flag State Of A Cell Between Hidden And Flagged
			Allowing Players To Mark Suspected Mine Locations.

			Preconditions:
				1.) row And col Must Be Valid Cell Coordinates
				2.) cells Array Must Be Properly Initialized
				3.) Cell Must Not Be In REVEALED State

			Postconditions:
				1.) HIDDEN Cells Will Become FLAGGED With Flag Visuals
				2.) FLAGGED Cells Will Become HIDDEN With Default Visuals
				3.) REVEALED Cells Will Remain Unchanged

		*/
		void toggleFlag(unsigned int row, unsigned int col)
		{

			if (this->cells[row][col].state == CellState::HIDDEN)
			{
				this->cells[row][col].state = CellState::FLAGGED;
				this->colorCell(row, col);
			}
			else if (this->cells[row][col].state == CellState::FLAGGED)
			{
				this->cells[row][col].state = CellState::HIDDEN;
				this->resetCellVisuals(row, col);
			}

		}
	

		/*
	
			Desc: Resets A Cell's Visual Appearance To Default Hidden State
			Clearing Any Special Colors Or Texture Coordinates.

			Preconditions:
				1.) row And col Must Be Valid Cell Coordinates
				2.) vertices Array Must Be Properly Initialized
				3.) CONFIG Color Values Must Be Available

			Postconditions:
				1.) Cell Vertices Will Be Reset To Default Hidden Cell Colors
				2.) Cell UV Coordinates Will Be Reset To Default (0,0)

		*/
		void resetCellVisuals(unsigned int row, unsigned int col)
		{

			unsigned int vertexIndex = (row * this->col_count + col) * 4;

			// Reset To Default Hidden Cell Color
			for (int i = 0; i < 4; ++i)
			{
				this->vertices[vertexIndex + i].color[0] = CONFIG::SHADER::COLORS::MINE_BASE_COLOR_R;
				this->vertices[vertexIndex + i].color[1] = CONFIG::SHADER::COLORS::MINE_BASE_COLOR_G;
				this->vertices[vertexIndex + i].color[2] = CONFIG::SHADER::COLORS::MINE_BASE_COLOR_B;
			}

			// Reset UV Coordinates To Default (No Texture)
			for (int i = 0; i < 4; ++i)
			{
				this->vertices[vertexIndex + i].uvs[0] = 0.0f;
				this->vertices[vertexIndex + i].uvs[1] = 0.0f;
			}

		}


		/*
	
			Desc: Updates The OpenGL Vertex Buffer Object With Current Vertex Data
			To Reflect Any Visual Changes Made To The Board.

			Preconditions:
				1.) VBO Must Be A Valid OpenGL Buffer Object
				2.) vertices Array Must Contain Current Vertex Data
				3.) OpenGL Context Must Be Active

			Postconditions:
				1.) GPU Vertex Buffer Will Be Updated With Current Vertex Data
				2.) All Visual Changes Will Be Ready For Next Render Call

		*/
		void updateVertexBuffer()
		{

			glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex) * (this->row_count * this->col_count * 4 + (this->row_count + 1 + this->col_count + 1) * 2), this->vertices);
	
		}


		/*
	
			Desc: Initializes The Board For Rendering By Setting Up All OpenGL
			Resources, Vertex Data, And Game Logic Components.

			Preconditions:
				1.) Board Constructor Must Have Been Called
				2.) OpenGL Context Must Be Active And Valid
				3.) Board Must Not Already Be Initialized

			Postconditions:
				1.) All Vertex And Index Data Will Be Allocated And Populated
				2.) OpenGL Buffers (VBO, EBO) Will Be Created And Loaded
				3.) Vertex Attributes Will Be Configured For Rendering
				4.) Board Game Logic Will Be Initialized With Mines Placed
				5.) isInit Flag Will Be Set To true

		*/
		void initialize()
		{

			if (isInit)
			{
				std::cerr << "Board Has Been Initialized...\n";
				return;
			}

			// Calculate Grid Line Counts
			unsigned int totalVertices = (this->row_count * this->col_count * 4) + ((this->row_count + 1) + (this->col_count + 1)) * 2;
			unsigned int totalIndices = (this->row_count * this->col_count * 6) + ((this->row_count + 1) + (this->col_count + 1)) * 2;

			// Create Vertex Data With Enough Space For Grid Lines
			this->vertices = new Vertex[totalVertices];
			this->indices = new unsigned int[totalIndices];

			this->populateBoard();
			this->generateGridLines();
			this->generateDigits();
			this->loadBoard();

			// Generate Buffers
			glGenBuffers(1, &this->VBO);
			glGenBuffers(1, &this->EBO);

			// Bind Buffers
			glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);

			// Load Data Into Buffers With Correct Sizes
			glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * totalVertices, this->vertices, GL_STATIC_DRAW);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * totalIndices, this->indices, GL_STATIC_DRAW);

			// Set Vertex Attribute Pointers
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float) * 3));

			this->isInit = true;

		}


		/*
	
			Desc: Renders The Minesweeper Board Including All Cells And Grid Lines
			Using OpenGL Draw Calls With Appropriate Vertex Attributes.

			Preconditions:
				1.) Board Must Be Properly Initialized
				2.) OpenGL Buffers Must Contain Current Vertex Data
				3.) Shader Program Must Be Active
				4.) Texture Atlas Must Be Bound To Texture Unit 0

			Postconditions:
				1.) All Board Cells Will Be Rendered As Triangles
				2.) Grid Lines Will Be Rendered As Lines
				3.) Texture Atlas Will Be Available For Number/Flag Rendering

		*/
		void render()
		{

			// Bind Our Buffers
			glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float) * 3));
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float) * 6));

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, this->proceduralAtlasTextureID);

			// Draw The Board
			glDrawElements(GL_TRIANGLES, this->row_count * this->col_count * 6, GL_UNSIGNED_INT, 0);

			// Draw The Grid Lines Based On Our linesOffset
			glDrawElements(GL_LINES, (this->row_count + 1) * 2 + (this->col_count + 1) * 2, GL_UNSIGNED_INT, (void*)(sizeof(unsigned int) * this->row_count * this->col_count * 6));
	
		}

};
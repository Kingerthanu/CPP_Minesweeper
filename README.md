# CPP_Minesweeper

A Game Of Minesweeper Developed In C++ OpenGL. The Game Has All The Features Of Minesweeper, Including The Selection Of Cells To Open, Mines, Blooming Of Open Sections, As Well As Flagging With The Inclusion Of A Procedural Texture Atlas For Characters. Game Has A Main Menu Page Derived From A Sprite Textured Menu As Well As Sprite Textured Game Over And Settings Menu. 


-----------

<img src="https://github.com/user-attachments/assets/fcf72b92-0148-45cf-87fd-7dfdb2dddd99" alt="Cornstarch <3" width="65" height="59"> <img src="https://github.com/user-attachments/assets/fcf72b92-0148-45cf-87fd-7dfdb2dddd99" alt="Cornstarch <3" width="65" height="59"> <img src="https://github.com/user-attachments/assets/fcf72b92-0148-45cf-87fd-7dfdb2dddd99" alt="Cornstarch <3" width="65" height="59"> <img src="https://github.com/user-attachments/assets/fcf72b92-0148-45cf-87fd-7dfdb2dddd99" alt="Cornstarch <3" width="65" height="59"> 

<h3>🔍 The Breakdown:</h3>

<h4>Window Class</h4>

  The Program Starts By Initializing A Instance Of The `Window` Class Which Encapsulates The OpenGL GLFW Window Which We Render Upon. In The `Window` Constructor, We Initialize Members By Either User Inputted Settings Or The `CONFIG` Located In `config.h` Presets. In Our Constructor We Also Call `Window::initialize(...)` Which Will Go Through And Initialize GLAD As Well As The GLFW Window Instance (Our `this->window` Member); Likewise We Will Initialize Rendering-Based Members Like Our Defined Class  `Shader` For Rendering As Well As Our HUD Texture Utilizing `Window::loadHUDTexture(...)` And Also Our Vertex Data For Both Our Window Instance, As Well As Our `Board` class Member Inside Of `Window::initializeVertexData(...)`. 


<h4>Shader Class</h4>

  The `Shader` Class Will Require Two Defined Files: .frag & .vert Files. These Two Files Will Be Compiled Into Source Code For The GPU Through `Shader::loadShader(...)` And Linked To The Given Shader Instance. Where These Files' Are Specified Is Defined Through The `config.h` File Which Holds Our `CONFIG` Namespace.

<h4>Board Class</h4>

  The `Board` Class Will Hold All Gamestate Derived Logic And Data, This Includes The Rendering Of The Game Board, Handling Click Events As Well As Populating The Given Game Board. Initial Configuration Of The Default Board Will Be Outlined Through The `CONFIG` Namespace. In `Window::initializeVertexData(...)` We Will Call `Board::initialize(...)` As We Will Be Utilizing A Shared VAO For The Parent `Window` As Well Our `Board` Instance. In This `Board::initialize(...)` Function, The Board Will Call Its Population Function For The Board (`Board::populateBoard(...)`) Which Will Create Our Game Grid Through A Dynamic Member Array Providing The Vertex Data For Each Cube. After, It Will Generate Grid Lines For Our Game Of Minesweeper Using `Board::generateGridLines(...)` As Well As The Procedural Text Texture Atlas Through `Board::generateDigits(...)`. After This Setup Is Done, We Do One Final Sweep Through The Board Using `Board::loadBoard(...)` Which Will Then Populate With Our Bombs And Update Adjacent Cells To Increment Their Adjacent Bomb Count. 

<h4>Main Loop</h4>

  After All Is Initialized, The `Window::mainLoop(...)` Will Be Called In Which Will Bring Us Into Our Main Game Loop, Rendering Our Window As Well As The Board. When The User Clicks On Our Window Is When We Will Provide Any Updating Via Logic Or Rendering As We Have No Per-Tick Animations Or Logic To Run Allowing The Process To Run Quite Smoothly Under Varying Hardware Limits.



<img src="https://github.com/user-attachments/assets/3f0d1297-da30-4f13-a56c-c416ae053653" alt="Cornstarch <3" width="65" height="59"> <img src="https://github.com/user-attachments/assets/3f0d1297-da30-4f13-a56c-c416ae053653" alt="Cornstarch <3" width="65" height="59"> <img src="https://github.com/user-attachments/assets/3f0d1297-da30-4f13-a56c-c416ae053653" alt="Cornstarch <3" width="65" height="59"> <img src="https://github.com/user-attachments/assets/3f0d1297-da30-4f13-a56c-c416ae053653" alt="Cornstarch <3" width="65" height="59"> 


-----------

<img src="https://github.com/user-attachments/assets/4e952549-dfb2-4635-a8f9-e3cbff7c0354" alt="Cornstarch <3" width="65" height="59"> <img src="https://github.com/user-attachments/assets/4e952549-dfb2-4635-a8f9-e3cbff7c0354" alt="Cornstarch <3" width="65" height="59"> <img src="https://github.com/user-attachments/assets/4e952549-dfb2-4635-a8f9-e3cbff7c0354" alt="Cornstarch <3" width="65" height="59"> <img src="https://github.com/user-attachments/assets/4e952549-dfb2-4635-a8f9-e3cbff7c0354" alt="Cornstarch <3" width="65" height="59"> 



<h3>🌟 Features:</h3>


<img src="https://github.com/user-attachments/assets/6a82e173-9614-47bb-af26-d730090a8152" alt="Cornstarch <3" width="65" height="59"> <img src="https://github.com/user-attachments/assets/6a82e173-9614-47bb-af26-d730090a8152" alt="Cornstarch <3" width="65" height="59"> <img src="https://github.com/user-attachments/assets/6a82e173-9614-47bb-af26-d730090a8152" alt="Cornstarch <3" width="65" height="59"> <img src="https://github.com/user-attachments/assets/6a82e173-9614-47bb-af26-d730090a8152" alt="Cornstarch <3" width="65" height="59">

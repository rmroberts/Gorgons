//Programmer: Rebecca Roberts
//File: main.cpp
//Date: April 1, 2013

//Narrative:
//	This program is a game with four combatants: a human, two gorgons, 
//	and a cyclops.  Each one moves through a square grid with some
//  combination of randomness and purposefulness.  When a cyclops is close 
//	to another combatant, the combatant dies.  When the human is close to 
//	a gorgon, they do battle, and each has a certain chance of winning.
//	The game ends when either both gorgons die or the human dies.
//Interface:
//  Introduction Message: "Welcome to 'Kill the Gorgons'!"
//						  "Enter coordinates with a space between them."
//  Exit Message: "Game Over"
//  Inputs:  The user inputs the initial coordinates of the gorgons
//			 and the human.
//  Outputs:  Outputs are to the screen.
//	   1.  There is one move per second, and the coordinates of each 
//		   combatant are output to the screen each second, along with 
//		   messages any time a combatant is killed.
//Constants: none.


#include <iostream>
#include <conio.h>
#include <windows.h>
#include <ctime>
#include <string>
using namespace std;
#define gridSize 20	//this should be an even number

void Intro();
// Requires : none
// Ensures : greeting and brief instructions
// Checks : none
void InitializeGrid(char grid[][gridSize]);
// Requires : grid has been declared
// Ensures : initializes grid with dashes
// Checks : none
void GetCoordinates(char grid[][gridSize], int HG[], int VG[], int H[]);
// Requires : grid, HG, VG, and H have been declared
// Ensures : prompts the user to input the initial coordinates of
//			 human, vertical gorgon, and horizontal gorgon.  The 
//			 arrays then have these coordinates, and they are placed
//			 in the grid.
// Checks : none
void PrintCoordinates(char grid[][gridSize], int HG[], int VG[], int H[], int C[]);
// Requires : grid, HG, VG, C, and H have been declared
// Ensures : prints the coordinates of HG, VG, H, and C
// Checks : none
void RefreshGrid(char grid[][gridSize], int HG[], int VG[], int H[], int C[]);
// Requires : grid, HG, VG, C, and H have been declared
// Ensures : refreshes the grid with new coordinates of HG, VG, H, and C
// Checks : none
void GorgonPatrol(int G[], char gorgon, int H[]);
// Requires : HG, VG, and H have been declared
// Ensures : uses random numbers to move the gorgon - random numbers are added 
//			 or subtracted from the appropriate element to move the gorgon.
//			 If the gorgon is within 10 spaces of human, it moves toward human.
//			 Takes a character to distinguish between vertical and horizontal gorgon.
// Checks : none
void CyclopsPatrol(int C[]);
// Requires : C has been declared
// Ensures : uses random numbers to move the cyclops - random numbers are added 
//			 or subtracted from the appropriate element to move the cyclops,
//			 and also to decide which direction to go.
// Checks : none
void HumanMove(int HG[], int VG[], int H[]);
// Requires : HG, VG, and H have been declared
// Ensures : uses random numbers to decide how many spaces (between 0 and 8)
//			 the human will move.  The human moves toward whichever gorgon
//			 is closest, first moving toward the row or column of the
//			 horizontal or vertical gorgon respectively, then toward the
//			 gorgon on its row or column.
// Checks : none
bool CyclopsDistance(int HG[], int VG[], int H[], int C[]);
// Requires : HG, VG, H, and C have been declared
// Ensures : returns true if cyclops is within 3 spaces of human or gorgon
// Checks : none
void CyclopsVictim(int HG[], int VG[], int H[], int C[], bool &HGalive, bool &VGalive, bool &Halive);
// Requires : HG, VG, H, C have been declared
// Ensures : determines which of the combatants, HG, VG, or H, is within
//			 3 spaces of the cyclops, changes their alive variable to false,
//			 and their coordinates to 999 999 so there is no more interaction
// Checks : none
bool BattleDistance(int HG[], int VG[], int H[]);
// Requires : HG, VG, and H have been declared
// Ensures : returns true if human is within 3 spaces of one of the gorgons
// Checks : none
char BattleGorgon(int HG[], int VG[], int H[]);
// Requires : HG, VG, and H have been declared
// Ensures : returns character indicating which gorgon is within battle distance
// Checks : none
void Battle(int G[], int H[], bool & Halive, bool & Galive, int battlenumber);
// Requires : HG, VG, and H have been declared
// Ensures : takes battlenumber to determine which of two battles to run.  
//			 In both battles, each side, human and gorgon, plays its 
//			 Nash equilibrium strategy, based on the probability of losing.
//			 In the first battle, human has weapons and gorgon defends.  In
//			 the second battle, vice versa.
// Checks : none
void Exit();
// Requires : none
// Ensures : exit message
// Checks : none

int main()
{
	char grid[gridSize][gridSize];
	int HG[2], VG[2], H[2], tempHG[2], tempVG[2], C[2] = { gridSize / 2, gridSize/2 };
	bool end = false, HGalive = true, VGalive = true, Halive = true;
	char gorgon;
	int battlenumber, count;
	unsigned seed = time(0);
	srand(seed);

	//Introductory message
	Intro();

	//Initialize grid with dashes
	InitializeGrid(grid);

	//Gets coordinates for gorgons and human from user
	GetCoordinates(grid, HG, VG, H);

	RefreshGrid(grid, HG, VG, H, C);

	cout << endl;

	//Prints initial coordinates
	PrintCoordinates(grid, HG, VG, H, C);

	//Plays game until either both gorgons die or human dies
	while (end == false)
	{
		//temporary arrays hold values of HG and VG so that when human decides which direction
		//to move, he goes by the values of HG and VG at the last move, so that they appear
		//to decide simultaneously.  Human hasn't moved yet so no temp human array is necessary.
		tempHG[0] = HG[0];
		tempHG[1] = HG[1];
		tempVG[0] = VG[0];
		tempVG[1] = VG[1];

		//If cyclops is within 3 spaces of combatant, combatant dies
		if (CyclopsDistance(HG, VG, H, C))
			CyclopsVictim(HG, VG, H, C, HGalive, VGalive, Halive);

		//If human is not within 3 spaces of gorgon, human and at least
		//one ofthe gorgons is still alive, living gorgons patrol,
		//human moves toward closest gorgon, and cyclops patrols.
		if (!BattleDistance(HG, VG, H) && Halive && (HGalive || VGalive))
		{
			if (HGalive)
				GorgonPatrol(HG, 'H', H);
			if (VGalive)
				GorgonPatrol(VG, 'V', H);
			HumanMove(tempHG, tempVG, H);
			CyclopsPatrol(C);
			RefreshGrid(grid, HG, VG, H, C);
			PrintCoordinates(grid, HG, VG, H, C);
			Sleep(1000);				//wait one second for next set of moves
		}
		//If human is within 3 spaces of a gorgon, human and gorgon do battle
		else if (BattleDistance(HG, VG, H) && Halive && (HGalive || VGalive))
		{
			gorgon = BattleGorgon(HG, VG, H);
			if (gorgon == 'H')
			{
				count = 0;				//reset counter so battle 1 is always first
				while (Halive && HGalive)	//Battle continues while both are alive
				{
					battlenumber = (count % 2) + 1;	//alternates between 1 and 2
					Battle(HG, H, Halive, HGalive, battlenumber);

					if (VGalive)				//other gorgon patrols if alive
						GorgonPatrol(VG, 'V', H);

					CyclopsPatrol(C);			//Cyclops randomly patrols

					RefreshGrid(grid, HG, VG, H, C);

					if (Halive && (HGalive || VGalive))
						PrintCoordinates(grid, HG, VG, H, C);

					//Checks to see if any combatant within 3 spaces of cyclops.
					//If so, the corresponding alive variable changed to false.
					if (CyclopsDistance(HG, VG, H, C))
						CyclopsVictim(HG, VG, H, C, HGalive, VGalive, Halive);

					count += 1;		//increments counter to switch between battles

					Sleep(1000);
				}
			}
			else
			{
				count = 0;				//reset counter so battle 1 is always first
				while (Halive && VGalive)	//Battle continues while both are alive
				{
					battlenumber = (count % 2) + 1;	//alternates between 1 and 2
					Battle(VG, H, Halive, VGalive, battlenumber);

					if (HGalive)				//other gorgon patrols if alive
						GorgonPatrol(HG, 'H', H);

					CyclopsPatrol(C);

					RefreshGrid(grid, HG, VG, H, C);

					//Checks to see if any combatant within 3 spaces of cyclops.
					//If so, the corresponding alive variable changed to false.
					if (CyclopsDistance(HG, VG, H, C))
						CyclopsVictim(HG, VG, H, C, HGalive, VGalive, Halive);

					count += 1;	//increments counter to switch between battles

					if (Halive && (HGalive || VGalive))
						PrintCoordinates(grid, HG, VG, H, C);

					Sleep(1000);
				}
			}
		}

		//continues as long as human and at least one gorgon are alive
		if (Halive && (HGalive || VGalive))
			end = false;
		else
			end = true;
	}

	if (Halive)
		cout << "Human wins\n";

	Exit();

	system("pause");

	return 0;
}

void Intro()
{
	cout << "Welcome to 'Kill the Gorgons'!\n"
		"Enter coordinates between 1 and " << gridSize << " with a space between them.\n\n";
}

void Exit()
{
	cout << "Game Over\n";
}

void InitializeGrid(char grid[][gridSize])
{
	for (int i = 0; i<gridSize; i++){
		for (int j = 0; j<gridSize; j++){
			grid[i][j] = '-';
		}
	}
}

void GetCoordinates(char grid[][gridSize], int HG[], int VG[], int H[])
{
	int HGrow, HGcol, VGrow, VGcol, Hrow, Hcol;

	cout << "Enter coordinates of Horizontal Gorgon: ";
	cin >> HGrow >> HGcol;
	cout << "Enter coordinates of Vertical Gorgon: ";
	cin >> VGrow >> VGcol;
	cout << "Enter coordinates of Human: ";
	cin >> Hrow >> Hcol;

	HG[0] = HGrow;
	HG[1] = HGcol;
	VG[0] = VGrow;
	VG[1] = VGcol;
	H[0] = Hrow;
	H[1] = Hcol;
}

void GorgonPatrol(int G[], char gorgon, int H[])
{
	int spaces, dir, move, dist;
	unsigned seed = time(0);
	srand(seed);
	spaces = (rand() % 10) - 5;	//produces a number between -5 and 5

	//manhattan distance between gorgon and human
	dist = abs(G[0] - H[0]) + abs(G[1] - H[1]);

	//determines whether to change rows or columns depending on
	//which gorgon was passed to the function
	if (gorgon == 'H')		//horizontal gorgon means column changes
		dir = 1;
	else				//vertical gorgon means row changes
		dir = 0;

	if ((dist <= 10) && (H[dir] >= G[dir]))	//if the gorgon is within 10 spaces of human
	{										//and need to go in positive direction to get to human
		if ((H[dir] - G[dir]) >= 5)
			G[dir] += 5;		   //if distance to gorgon's row or col > 5, go 5 in that direction
		else
			G[dir] = H[dir];		//if distance to gorgon's row or col < 5, go to that row or col
	}
	else if ((dist <= 10) && (H[dir] < G[dir]))	//if the gorgon is within 10 spaces of human and 
	{											//needs to go in negative direction to get to human
		if ((G[dir] - H[dir]) >= 5)
			G[dir] -= 5;			//if distance to gorgon's row or col > 5, go 5 in that direction
		else
			G[dir] = H[dir];		//if distance to gorgon's row or col < 5, go to that row or col
	}
	//if the manhattan distance to human is greater than 10 ...
	else
		G[dir] += spaces;	//adds number between -5 and 5 to gorgon's position

	//This causes the gorgon to turn around when it
	//reaches the edge of the grid
	if (G[dir] > gridSize - 1)
		G[dir] = gridSize - 1 - (G[dir] - (gridSize - 1));
	else if (G[dir] < 0)
		G[dir] = -G[dir];
}

void CyclopsPatrol(int C[])
{
	int spaces, dir;
	spaces = (rand() % 9) - 4;	//produces a number between -4 and 4
	dir = rand() % 2;			//0 or 1 decides rows or columns

	C[dir] += spaces;

	//This causes the cyclops to turn around when it
	//reaches the edge of the grid
	if (C[dir] > gridSize - 1)
		C[dir] = gridSize - 1 - (C[dir] - (gridSize - 1));
	else if (C[dir] < 0)
		C[dir] = -C[dir];
}

void HumanMove(int HG[], int VG[], int H[])
{
	int HGdist, VGdist, temp;
	int spaces = rand() % 9;

	//determines manhattan distance to the two gorgons
	HGdist = abs(HG[0] - H[0]) + abs(HG[1] - H[1]);
	VGdist = abs(VG[0] - H[0]) + abs(VG[1] - H[1]);

	//if HG is closest ...
	if (HGdist < VGdist)
	{
		//human row > gorgon row and vert dist btwn them > spaces
		if ((HG[0] < H[0]) && ((H[0] - HG[0]) >= spaces))
			H[0] = H[0] - spaces;					//move vert toward gorgon for all spaces
		else if ((HG[0] > H[0]) && ((HG[0] - H[0]) >= spaces))	//gorgon row > human row etc.
			H[0] = H[0] + spaces;					//move vert toward gorgon for all spaces
		else if ((HG[0] <= H[0]) && ((H[0] - HG[0]) < spaces))	//human row > gorgon row and 
		{														//vert dist btwn them < spaces
			temp = spaces - (H[0] - HG[0]);	//temp holds remaining spaces left to cover after 
			H[0] = HG[0];					//human moves to gorgon's row, then
			//move human to gorgon's row
			if (HG[1]>H[1])					//if gorgon's column > human's column,													
				H[1] = H[1] + temp;	//human moves right, towards gorgon, remaining number of spaces
			else							//if gorgon's col is not greater than human's col,
				H[1] = H[1] - temp;			//human moves left remaining number of spaces
		}
		else if ((HG[0] >= H[0]) && ((HG[0] - H[0]) < spaces))	//gorgon row > human row and 
		{														//vert dist btwn them < spaces
			temp = spaces - (HG[0] - H[0]);	//temp holds remaining spaces left to cover 
			H[0] = HG[0];					//after human moves to gorgon's row, then 
			//move human to gorgon's row
			if (HG[1]>H[1])					//if gorgon's column > human's column,													
				H[1] = H[1] + temp;	//human moves right, towards gorgon, remaining number of spaces
			else							//if gorgon's col is not greater than human's col,
				H[1] = H[1] - temp;			//human moves left remaining number of spaces
		}
	}
	//if VG is closest ...
	else if (VGdist < HGdist)
	{
		//human col > gorgon col and horiz dist btwn them > spaces
		if ((VG[1] < H[1]) && ((H[1] - VG[1]) >= spaces))
			H[1] = H[1] - spaces;					//move left toward gorgon for all spaces
		else if ((VG[1] > H[1]) && ((VG[1] - H[1]) >= spaces))	//gorgon col > human col etc.
			H[1] = H[1] + spaces;					//move right toward gorgon for all spaces
		else if ((VG[1] <= H[1]) && ((H[1] - VG[1]) < spaces))	//human col > gorgon col and 
		{														//horiz dist btwn them < spaces
			temp = spaces - (H[1] - VG[1]);	//temp holds remaining spaces left to cover
			H[1] = VG[1];					// after human moves to gorgon's col. 
			//move human to gorgon's col
			if (VG[0]>H[0])					//if gorgon's row > human's row,													
				H[0] = H[0] + temp;		//human moves up, towards gorgon, remaining number of spaces
			else							//if gorgon's row is not greater than human's row,
				H[0] = H[0] - temp;			//human moves down remaining number of spaces
		}
		else if ((VG[1] >= H[1]) && ((VG[1] - H[1]) < spaces))	//gorgon col > human col and horiz 
		{														//dist btwn them < spaces
			temp = spaces - (VG[1] - H[1]);	//temp holds remaining spaces left to cover
			H[1] = VG[1];					//move human to gorgon's col

			if (VG[0]>H[0])					//if gorgon's row > human's row,													
				H[0] = H[0] + temp;		//human moves up, towards gorgon, remaining number of spaces
			else							//if gorgon's row is not greater than human's row,
				H[0] = H[0] - temp;			//human moves down remaining number of spaces
		}
	}
}

bool CyclopsDistance(int HG[], int VG[], int H[], int C[])
{
	int HGdist, VGdist, Hdist;

	//determines manhattan distance to gorgons and human
	HGdist = abs(HG[0] - C[0]) + abs(HG[1] - C[1]);
	VGdist = abs(VG[0] - C[0]) + abs(VG[1] - C[1]);
	Hdist = abs(H[0] - C[0]) + abs(H[1] - C[1]);

	if ((HGdist <= 3) || (VGdist <= 3) || (Hdist <= 3))
		return true;
	return false;
}

void CyclopsVictim(int HG[], int VG[], int H[], int C[], bool &HGalive, bool &VGalive, bool &Halive)
{
	int HGdist, VGdist, Hdist;

	//determines manhattan distance from cyclops to gorgons and human
	HGdist = abs(HG[0] - C[0]) + abs(HG[1] - C[1]);
	VGdist = abs(VG[0] - C[0]) + abs(VG[1] - C[1]);
	Hdist = abs(H[0] - C[0]) + abs(H[1] - C[1]);

	//no else statements since cyclops could have more than one victim
	if (HGdist <= 3)
	{
		HGalive = false;
		HG[0] = 999;
		HG[1] = 999;
		cout << "Horizontal Gorgon killed by Cyclops\n\n";
	}
	if (VGdist <= 3)
	{
		VGalive = false;
		VG[0] = 999;
		VG[1] = 999;
		cout << "Vertical Gorgon killed by Cyclops\n\n";
	}
	if (Hdist <= 3)
	{
		Halive = false;
		cout << "Human killed by Cyclops\n";
	}
}

bool BattleDistance(int HG[], int VG[], int H[])
{
	int HGdist, VGdist;
	//determines manhattan distance to the two gorgons
	HGdist = abs(HG[0] - H[0]) + abs(HG[1] - H[1]);
	VGdist = abs(VG[0] - H[0]) + abs(VG[1] - H[1]);

	if ((HGdist <= 3) || (VGdist <= 3))
		return true;
	return false;
}

char BattleGorgon(int HG[], int VG[], int H[])
{
	int HGdist, VGdist;
	//determines manhattan distance to the two gorgons
	HGdist = abs(HG[0] - H[0]) + abs(HG[1] - H[1]);
	VGdist = abs(VG[0] - H[0]) + abs(VG[1] - H[1]);

	if (HGdist <= 3)
		return 'H';
	else if (VGdist <= 3)
		return 'V';
	else
		return 'X';
}

void Battle(int G[], int H[], bool & Halive, bool & Galive, int battlenumber)
{
	unsigned seed = time(0);
	srand(seed);
	string Hweapon, Gdefense, Gweapon, Hdefense;
	int Hweapondecision = (rand() % 10) + 1;	//produces a number between 1 and 10
	int Gdefensedecision = (rand() % 5) + 1;	//produces a number between 1 and 5
	int live = (rand() % 10) + 1;				//produces a number between 1 and 10
	int Hdefensedecision = (rand() % 2) + 1;	//produces a number between 1 and 2
	int Gweapondecision = (rand() % 4) + 1;		//produces a number between 1 and 4

	if (battlenumber == 1)
	{
		// H (max) strategy: (P,K) = (7/10, 3/10)
		// G (min) strategy: (V,S) = (3/5, 2/5)
		if (Hweapondecision <= 7)	// 7/10 of the time H chooses pistol
			Hweapon = "pistol";
		else
			Hweapon = "knife";		// 3/10 of the time H chooses knife
		if (Gdefensedecision <= 3)
			Gdefense = "vest";		// 3/5 of the time G chooses vest
		else
			Gdefense = "shield";	// 2/5 of the time G chooses shield

		if (Hweapon == "pistol" && Gdefense == "vest" && (live <= 5))	//50% chance gorgon dies 
			Galive = false;
		else if (Hweapon == "knife" && Gdefense == "vest" && (live <= 9)) //90% chance gorgon dies
			Galive = false;
		else if (Hweapon == "pistol" && Gdefense == "shield" && (live <= 8)) //80% chance gorgon dies 
			Galive = false;
		else if (Hweapon == "knife" && Gdefense == "shield" && (live <= 2)) //20% chance gorgon dies 
			Galive = false;
		cout << "Battle 1\n";
		cout << "Human weapon: " << Hweapon << endl;
		cout << "Gorgon defense: " << Gdefense << endl << endl;
	}

	else if (battlenumber == 2)
	{
		// G (max) strategy: (S,L) = (1/4, 3/4)
		// H (min) strategy: (H,B) = (1/2, 1/2)
		if (Gweapondecision <= 1)	// 1/4 of the time G chooses sword
			Gweapon = "sword";
		else
			Gweapon = "lance";		// 3/4 of the time G chooses lance
		if (Hdefensedecision <= 1)
			Hdefense = "helmet";		// 1/2 of the time H chooses army helmet
		else
			Hdefense = "breastplate";	// 1/2 of the time H chooses breastplate

		if (Gweapon == "sword" && Hdefense == "helmet" && (live <= 5))	//50% chance human dies 
			Halive = false;
		else if (Gweapon == "lance" && Hdefense == "helmet" && (live <= 3)) //30% chance human dies
			Halive = false;
		else if (Gweapon == "sword" && Hdefense == "breastplate" && (live <= 2)) //20% chance human dies
			Halive = false;
		else if (Gweapon == "lance" && Hdefense == "breastplate" && (live <= 4)) //40% chance human dies
			Halive = false;
		cout << "Battle 2\n";
		cout << "Gorgon weapon: " << Gweapon << endl;
		cout << "Human defense: " << Hdefense << endl << endl;
	}

	if (Halive == false)
		cout << "Human killed in battle\n\n";
	//changes coordinates if gorgon dies
	if (Galive == false)
	{
		cout << "Gorgon killed in battle\n\n";
		G[0] = 999;
		G[1] = 999;
	}
}

void RefreshGrid(char grid[][gridSize], int HG[], int VG[], int H[], int C[])
{
	for (int i = 0; i<gridSize; i++){
		for (int j = 0; j<gridSize; j++){
			grid[i][j] = '-';
		}
	}

	if (HG[0] < gridSize)					//if gorgon is not alive, will not
		grid[HG[0]][HG[1]] = 'G';	//appear on grid
	if (VG[0] < gridSize)
		grid[VG[0]][VG[1]] = 'V';
	grid[H[0]][H[1]] = 'H';
	grid[C[0]][C[1]] = 'C';
}

void PrintCoordinates(char grid[][gridSize], int HG[], int VG[], int H[], int C[])
{
	for (int i = 0; i<gridSize; i++){
		for (int j = 0; j<gridSize; j++){
			cout << grid[i][j] << ' ';
		}
		cout << endl;
	}

	cout << "Horizontal Gorgon: " << HG[0] << ' ' << HG[1] << endl;
	cout << "Vertical Gorgon:" << VG[0] << ' ' << VG[1] << endl;
	cout << "Cyclops:" << C[0] << ' ' << C[1] << endl;
	cout << "Human:" << H[0] << ' ' << H[1] << endl << endl;
}
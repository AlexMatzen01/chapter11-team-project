#include <iostream>
#include <fstream>

using namespace std;

// i didnt test this btw, sorry if it doesnt work.
// its written to the info i have
// its all commented

int main()
{
	// Init variables
	ofstream Output_File("File_Name.txt");
	int Players = 0;
	
	// Get the players in the tournament
	while (Players <= 0)
	{
		cout << "How many players are there in the tournament? :> ";
		cin >> Players;
	}

	// Get the player info, write it to a file
	for (int I = 0; I <= Players; I++)
	{
		// Create the player object and get all info.
		Player My_Player;
		cout << "-- Player " << I + 1 << "--" << endl;
		cout << "Enter player name :> ";
		cin >> My_Player.name;
		cout << "Enter player role :> ";
		cin >> My_Player.role;
		cout << "Enter player kills :> ";
		cin >> My_Player.kills;
		cout << "Enter player deaths :> ";
		cin >> My_Player.deaths;

		cout << endl << endl;

		// Write it to the file
		Output_File << Player.name << "|" << Player.role << "|" << Player.kills << "|" << Player.deaths << endl;
		// Format: Name|Role|Kills|Deaths
	}
}

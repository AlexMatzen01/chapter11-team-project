#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

int main()
{
	ofstream Output_File("File_Name.txt");
	vector<Player>;

	int Players = 0;
	
	if (Players <= 0)
	{
		cout << "How many players are there in the tournament? :> ";
		cin >> Players;
	}

	for (int I = 0; I <= Players; I++)
	{
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

		Output_File << Player.name
	}
}

// Format: Name|Role|Kills|Deaths
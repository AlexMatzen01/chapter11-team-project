#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

/**
 * Player stats for the tournament.
 */
struct Player {
    string name;
    string role;
    int kills;
    int deaths;
};

/**
 * Team information containing a pointer to an array of Players.
 */
struct Team {
    string teamName;
    int wins;
    int losses;
    int playerCount;
    Player* players; // Dynamic array of players
};

/**
 * The high-level Tournament structure.
 * Holds multiple teams.
 */
struct Tournament {
    vector<Team*> teams; // Vector of pointers to Teams
};

// --- Function Prototypes ---
// here are the core I/O and memory management ones.
void loadTournamentData(const string& filename, Tournament& tournament);
void cleanupTournament(Tournament& tournament);
void parsePlayerData(string line, Player& player);
string trim(const string& s);

int main() {
    Tournament myTournament;
    const string DATA_FILE = "tournament_data.txt";

    cout << "--- Grand Tournament Manager Starting ---" << endl;
    cout << "Loading data from " << DATA_FILE << "..." << endl;

    // Load the file and build the nested structures
    loadTournamentData(DATA_FILE, myTournament);

    // Verify loading worked
    if (myTournament.teams.empty()) {
        cout << "Error: No teams loaded. Check if " << DATA_FILE << " exists." << endl;
    } else {
        cout << "Successfully loaded " << myTournament.teams.size() << " teams." << endl;
        
        // Print a quick summary to show it's working
        cout << "\nQuick Team List:" << endl;
        for (const auto& t : myTournament.teams) {
            cout << " - " << t->teamName << " (" << t->wins << "W/" << t->losses << "L)" << endl;
        }
    }

    // Since we're using pointers and dynamic memory, we MUST clean up before exiting
    cout << "\nCleaning up memory..." << endl;
    cleanupTournament(myTournament);

    cout << "Shutdown complete." << endl;
    return 0;
}

/**
 * Reads the tournament data file and parses the custom format.
 */
void loadTournamentData(const string& filename, Tournament& tournament) {
    ifstream inputFile(filename);
    if (!inputFile.is_open()) return;

    string line;
    Team* currentTeam = nullptr;

    while (getline(inputFile, line)) {
        line = trim(line);
        if (line.empty()) continue;

        if (line == "BEGIN_TEAM") {
            currentTeam = new Team();
            currentTeam->players = nullptr;
        } else if (line.find("TEAM_NAME:") == 0) {
            currentTeam->teamName = trim(line.substr(10));
        } else if (line.find("WINS:") == 0) {
            currentTeam->wins = stoi(line.substr(5));
        } else if (line.find("LOSSES:") == 0) {
            currentTeam->losses = stoi(line.substr(7));
        } else if (line.find("PLAYERS:") == 0) {
            currentTeam->playerCount = stoi(line.substr(8));
            currentTeam->players = new Player[currentTeam->playerCount];
        } else if (line == "PLAYER_DATA:") {
            for (int i = 0; i < currentTeam->playerCount; ++i) {
                if (getline(inputFile, line)) {
                    parsePlayerData(line, currentTeam->players[i]);
                }
            }
        } else if (line == "END_TEAM") {
            if (currentTeam) {
                tournament.teams.push_back(currentTeam);
                currentTeam = nullptr;
            }
        }
    }
    inputFile.close();
}

/**
 * Splits the player line (e.g., Alice|Sniper|18|6) into a Player struct.
 * Uses stringstream for clean extraction.
 */
void parsePlayerData(string line, Player& player) {
    stringstream ss(line);
    string segment;
    
    // Format: Name|Role|Kills|Deaths
    getline(ss, player.name, '|');
    getline(ss, player.role, '|');
    
    getline(ss, segment, '|');
    player.kills = segment.empty() ? 0 : stoi(segment);
    
    getline(ss, segment, '|');
    player.deaths = segment.empty() ? 0 : stoi(segment);
}

/**
 * Manual memory cleanup for all dynamic allocations.
 */
void cleanupTournament(Tournament& tournament) {
    for (Team* t : tournament.teams) {
        if (t) {
            // Delete the array of players FIRST
            delete[] t->players; 
            // Then delete the team itself
            delete t; 
        }
    }
    tournament.teams.clear();
}

/**
 * Utility to trim whitespace from strings.
 */
string trim(const string& s) {
    size_t first = s.find_first_not_of(" \t\r\n");
    if (string::npos == first) return "";
    size_t last = s.find_last_not_of(" \t\r\n");
    return s.substr(first, (last - first + 1));
}

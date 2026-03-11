#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <algorithm>

using namespace std;

// --- Data Structures ---

struct Player {
    string name;
    string role;
    int kills;
    int deaths;
};

struct Team {
    string teamName;
    int wins;
    int losses;
    int playerCount;
    Player* players; // Pointer to dynamic array
};

struct Tournament {
    vector<Team*> teams; // Vector of Team pointers
};

// --- Function Prototypes ---

void loadTournamentData(const string& filename, Tournament& tournament);
void saveTournamentData(const string& filename, const Tournament& tournament);
void exportFinalReport(const string& filename, const Tournament& tournament);
void generateTournamentReport(const string& filename, const Tournament& tournament);
void generateTournamentStats(const Tournament& tournament);
void sortTeamsByPerformance(Tournament& tournament);
double calculateWinRate(const Team* t);
void displayTournamentWinner(const Tournament& tournament);
void addManualPlayerData(Tournament& tournament);
void cleanupTournament(Tournament& tournament);
void parsePlayerData(string line, Player& player);
string trim(const string& s);
string toUpper(string s);
bool runSelfTests();

int main() {
    // Run integration tests first
    if (!runSelfTests()) {
        cout << "Self-tests failed! Exiting..." << endl;
        return 1;
    }

    Tournament myTournament;
    const string DATA_FILE = "tournament_data.txt";
    const string SUMMARY_FILE = "tournament_summary.txt";
    const string REPORT_FILE = "OFFICIAL_REPORT.txt";

    cout << "--- Tournament Manager ---" << endl;
    
    // Load initial data
    loadTournamentData(DATA_FILE, myTournament);

    // Optional manual entry
    char choice;
    cout << "\nAdd a new team? (y/n): ";
    cin >> choice;
    if (toupper(choice) == 'Y') {
        addManualPlayerData(myTournament);
    }

    if (!myTournament.teams.empty()) {
        // Sort and rank teams
        sortTeamsByPerformance(myTournament);

        // Display results and stats
        generateTournamentStats(myTournament);
        displayTournamentWinner(myTournament);
        
        // Export formal report
        exportFinalReport(REPORT_FILE, myTournament);

        // Also generate a full detailed tournament report for the assignment
        generateTournamentReport("tournament_report.txt", myTournament);
        
        // Save raw data state
        saveTournamentData(SUMMARY_FILE, myTournament);
    }

    // Free dynamic memory
    cleanupTournament(myTournament);
    return 0;
}

// --- Function Definitions ---

void loadTournamentData(const string& filename, Tournament& tournament) {
    ifstream file(filename);
    if (!file.is_open()) return;

    string line;
    Team* currentTeam = nullptr;

    while (getline(file, line)) {
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
                if (getline(file, line)) {
                    parsePlayerData(line, currentTeam->players[i]);
                }
            }
        } else if (line == "END_TEAM") {
            if (currentTeam) tournament.teams.push_back(currentTeam);
        }
    }
    file.close();
}

void saveTournamentData(const string& filename, const Tournament& tournament) {
    ofstream file(filename);
    for (const auto& t : tournament.teams) {
        file << "BEGIN_TEAM\nTEAM_NAME: " << t->teamName 
             << "\nWINS: " << t->wins << "\nLOSSES: " << t->losses 
             << "\nPLAYERS: " << t->playerCount << "\nPLAYER_DATA:\n";
        for (int i = 0; i < t->playerCount; i++) {
            file << t->players[i].name << "|" << t->players[i].role << "|" 
                 << t->players[i].kills << "|" << t->players[i].deaths << "\n";
        }
        file << "END_TEAM\n\n";
    }
    file.close();
}

void generateTournamentStats(const Tournament& tournament) {
    for (const auto& team : tournament.teams) {
        int tKills = 0, tDeaths = 0;
        double bestKD = -1.0;
        string bestPlayer = "N/A";

        cout << "--------------------------------" << endl << endl;
        cout << "Team: " << team->teamName << endl;
        cout << "Wins: " << team->wins << endl;
        cout << "Losses: " << team->losses << endl << endl;

        cout << "Players" << endl;
        cout << "----------------------------" << endl;

        for (int i = 0; i < team->playerCount; ++i) {
            Player& p = team->players[i];
            tKills += p.kills; 
            tDeaths += p.deaths;
            
            double kd = (p.deaths == 0) ? p.kills : (double)p.kills / p.deaths;
            
            cout << left << setw(11) << p.name 
                 << setw(9) << p.role 
                 << p.kills << " K / " << p.deaths << " D" << endl;

            if (kd > bestKD) { 
                bestKD = kd; 
                bestPlayer = p.name; 
            }
        }
        
        cout << endl;
        cout << "Team Kills: " << tKills << endl;
        cout << "Team Deaths: " << tDeaths << endl;
        cout << "Best Player: " << bestPlayer << endl << endl;
    }
}

void exportFinalReport(const string& filename, const Tournament& tournament) {
    ofstream report(filename);
    report << "--- OFFICIAL TOURNAMENT REPORT ---\n\n";
    if (!tournament.teams.empty()) {
        report << "WINNER: " << toUpper(tournament.teams[0]->teamName) << "\n\n";
    }
    for (const auto& t : tournament.teams) {
        report << "Team: " << t->teamName << " | Win Rate: " << calculateWinRate(t) << "%\n";
    }
    report.close();
}

void generateTournamentReport(const string& filename, const Tournament& tournament) {
    ofstream out(filename);
    out << "=================================\n";
    out << "      TOURNAMENT REPORT\n";
    out << "=================================\n\n";

    int totalPlayers = 0;
    string topPlayerName = "N/A";
    int topPlayerKills = -1;
    string teamMostWins = "N/A";
    int mostWins = -1;

    for (const auto& t : tournament.teams) {
        out << "Team: " << t->teamName << "\n";
        out << "Wins: " << t->wins << "\n";
        out << "Losses: " << t->losses << "\n\n";

        out << "Players\n";
        out << "----------------------------\n";

        int teamKills = 0;
        int teamDeaths = 0;
        string bestPlayer = "N/A";
        double bestKD = -1.0;

        for (int i = 0; i < t->playerCount; ++i) {
            const Player& p = t->players[i];
            out << left << setw(11) << p.name << setw(9) << p.role
                << p.kills << " K / " << p.deaths << " D\n";

            teamKills += p.kills;
            teamDeaths += p.deaths;
            totalPlayers++;

            if (p.kills > topPlayerKills) {
                topPlayerKills = p.kills;
                topPlayerName = p.name;
            }

            double kd = (p.deaths == 0) ? p.kills : (double)p.kills / p.deaths;
            if (kd > bestKD) { bestKD = kd; bestPlayer = p.name; }
        }

        out << "\n";
        out << "Team Kills: " << teamKills << "\n";
        out << "Team Deaths: " << teamDeaths << "\n";
        out << "Best Player: " << bestPlayer << "\n\n";
        out << "--------------------------------\n\n";

        if (t->wins > mostWins) { mostWins = t->wins; teamMostWins = t->teamName; }
    }

    // Global stats
    int totalTeams = (int)tournament.teams.size();
    int avgKills = totalPlayers == 0 ? 0 : (topPlayerKills * totalPlayers) / totalPlayers; // placeholder if needed
    // Compute average kills properly
    int totalKills = 0;
    for (const auto& t : tournament.teams) {
        for (int i = 0; i < t->playerCount; ++i) totalKills += t->players[i].kills;
    }
    avgKills = totalPlayers == 0 ? 0 : totalKills / totalPlayers;

    out << "GLOBAL TOURNAMENT STATS\n\n";
    out << "Total Teams: " << totalTeams << "\n";
    out << "Total Players: " << totalPlayers << "\n";
    out << "Top Player: " << topPlayerName << " (" << topPlayerKills << " kills)\n";
    out << "Team with Most Wins: " << teamMostWins << "\n";
    out << "Average Kills per Player: " << avgKills << "\n";

    out.close();
}

void addManualPlayerData(Tournament& tournament) {
    Team* t = new Team();
    cout << "Team Name: "; cin.ignore(); getline(cin, t->teamName);
    cout << "Wins/Losses: "; cin >> t->wins >> t->losses;
    cout << "Player Count: "; cin >> t->playerCount;
    t->players = new Player[t->playerCount];

    for (int i = 0; i < t->playerCount; i++) {
        cout << "Player " << i+1 << " (Name Role Kills Deaths): ";
        cin >> t->players[i].name >> t->players[i].role >> t->players[i].kills >> t->players[i].deaths;
    }
    tournament.teams.push_back(t);
}

void parsePlayerData(string line, Player& p) {
    stringstream ss(line);
    string part;
    getline(ss, p.name, '|');
    getline(ss, p.role, '|');
    getline(ss, part, '|'); p.kills = part.empty() ? 0 : stoi(part);
    getline(ss, part, '|'); p.deaths = part.empty() ? 0 : stoi(part);
}

double calculateWinRate(const Team* t) {
    if (t->wins + t->losses == 0) return 0;
    return (double)t->wins / (t->wins + t->losses) * 100.0;
}

void sortTeamsByPerformance(Tournament& tournament) {
    sort(tournament.teams.begin(), tournament.teams.end(), [](Team* a, Team* b) {
        return calculateWinRate(a) > calculateWinRate(b);
    });
}

void displayTournamentWinner(const Tournament& tournament) {
    if (tournament.teams.empty()) return;
    cout << "\n>>> CHAMPION: " << toUpper(tournament.teams[0]->teamName) << " <<<\n";
}

void cleanupTournament(Tournament& tournament) {
    for (Team* t : tournament.teams) {
        delete[] t->players;
        delete t;
    }
    tournament.teams.clear();
}

string trim(const string& s) {
    size_t f = s.find_first_not_of(" \t\r\n");
    if (f == string::npos) return "";
    return s.substr(f, s.find_last_not_of(" \t\r\n") - f + 1);
}

string toUpper(string s) {
    for (char &c : s) c = toupper(c);
    return s;
}

bool runSelfTests() {
    if (trim("  hi  ") != "hi") return false;
    if (toUpper("abc") != "ABC") return false;
    // Test parsing player data
    Player testP;
    parsePlayerData(string("Alice|Sniper|18|6"), testP);
    if (testP.name != "Alice" || testP.role != "Sniper" || testP.kills != 18 || testP.deaths != 6) return false;

    // Test win rate calculation
    Team testT;
    testT.wins = 1; testT.losses = 1;
    if (calculateWinRate(&testT) != 50.0) return false;
    return true;
}

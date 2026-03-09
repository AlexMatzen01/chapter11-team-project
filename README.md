# The Grand Tournament Manager
### Chapter 11 Team Programming Challenge

A C++ analysis system designed to rebuild tournament data from raw text files using nested structures, dynamic memory management, and modular design.

## Project Overview
The software reads `tournament_data.txt`, organizes teams and players into hierarchical structures, and prepares for comprehensive statistical analysis. It emphasizes efficient string manipulation and robust memory safety.

## ⚠️⚠️⚠️IMPORTANT NOTE⚠️⚠️⚠️
I, Alex Matzen, will take on the duty of ensuring that all individual modules will work together seamlessly—hopefully.

## Team Roles & Contributions
| Member | Responsibility | Status |
| :--- | :--- | :--- |
| **Alex Matzen** | File input system, Struct design, Memory management | ✅ Complete |
| **Liam Dowell** | Team parsing & Logic | ⏳ Waiting |
| **Gage Boyd** | Player data parsing | ⏳ Waiting |
| **Luke Bishop** | Statistical calculations | ⏳ Waiting |
| **Max Barlow** | Statistical calculations | ⏳ Waiting |
| **Jacob Losey** | String utilities & formatting | ⏳ Waiting |
| **Ethan Todd** | Report generation & Integration testing | ⏳ Waiting |
| **Alex Matzen** | Module Integration (requires all other roles to be complete) | ⏳ Waiting |

## Architecture
- **Nested Structs**: `Tournament` → `Team` → `Player`.
- **Memory Strategy**: Hybrid approach using `std::vector<Team*>` for dynamic growth and pointers to fixed-size arrays for `Player` data to ensure high-performance access.
- **Parsing**: Custom state-machine parser handles the `BEGIN_TEAM` / `END_TEAM` blocks and pipe-delimited player records.

## How to build and run
1. Ensure `tournament_data.txt` is in the root directory.
2. Compile using a C++11 (or newer) compiler:
   ```bash
   g++ -std=c++11 main.cpp -o tournament_manager
   ```
3. Run the executable:
   ```bash
   ./tournament_manager
   ```

## Requirements Checklist
- [x] Nested struct types
- [x] Use of pointers
- [x] Use of vectors/arrays
- [x] File input system
- [x] Memory management (Cleanup logic)
- [ ] 14+ Modular functions
- [ ] Report generation output


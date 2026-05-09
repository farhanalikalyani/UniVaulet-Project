cat > README.md << 'EOF'
# UniVault - University Record & Simulation System

## Programming Fundamentals Project - Spring 2026

### Features
- 3D Campus Grid (Departments → Floors → Rooms)
- Student Enrollment with Department Selection
- Academic Simulation with Random Events
- Live Dashboard Display
- Save/Load Data Persistence

### Departments
| Code | Department |
|------|------------|
| 0 | Cyber Security |
| 1 | Software Engineering |
| 2 | Artificial Intelligence |
| 3 | Data Science |
| 4 | Social Sciences |

### Commands
| Key | Action |
|-----|--------|
| ENTER | Run 1 time step |
| E | Add new student |
| J | Jump to different floor |
| F | Find student by ID |
| S | Save data |
| X | Exit |

### How to Compile and Run
```bash
g++ project.cpp -o univault.exe
./univault.exe

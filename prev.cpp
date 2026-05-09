// ============================================================
// Names   : Ali Raza                , Sara Khan
// Roll No : CS-1001                 , CS-1002
// Section : B                       , B
// Course  : Programming Fundamentals - CS101
// Project : UniVault
// Semester: Spring 2026
// ============================================================

#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <cstring>

using namespace std;

// function declarations
int*** makeCampus(int d, int f, int r);
void deleteCampus(int*** grid, int d, int f);

void addStudent(int*** campus,
                char**& nameArr, int*& idArr, float*& gpaArr, int*& statusArr,
                int& totalStudents,
                int deptCount, int floorCount, int roomCount,
                int chosenDept,
                const char* DEPT_NAMES[]);

void doTimeStep(int*** campus,
                char**& nameArr, int*& idArr, float*& gpaArr, int*& statusArr,
                int& totalStudents,
                int deptCount, int floorCount, int roomCount,
                int& stepNum);

void showDashboard(int*** campus,
                   char** nameArr, int* idArr, float* gpaArr, int* statusArr,
                   int totalStudents,
                   int deptCount, int floorCount, int roomCount,
                   int curDept, int curFloor, int stepNum,
                   const char* DEPT_NAMES[]);

void searchStudent(int* idArr, char** nameArr, float* gpaArr, int* statusArr,
                   int totalStudents,
                   int*** campus, int deptCount, int floorCount, int roomCount,
                   const char* DEPT_NAMES[]);

void moveView(int deptCount, int floorCount, int& curDept, int& curFloor,
              const char* DEPT_NAMES[]);

void saveAll(int*** campus,
             char** nameArr, int* idArr, float* gpaArr, int* statusArr,
             int totalStudents,
             int deptCount, int floorCount, int roomCount);

bool loadAll(int*** campus,
             char**& nameArr, int*& idArr, float*& gpaArr, int*& statusArr,
             int& totalStudents,
             int deptCount, int floorCount, int roomCount);

void showDeptMenu(const char* DEPT_NAMES[], int TOTAL_DEPTS);

// helper functions
char* copyText(const char* source);
const char* getStatusText(int s);

// ============================================================
// HELPER FUNCTIONS
// ============================================================

char* copyText(const char* source)
{
    int len = 0;
    while(source[len] != '\0') len++;
    
    char* dest = new char[len + 1];
    for(int i = 0; i <= len; i++)
        dest[i] = source[i];
    return dest;
}

const char* getStatusText(int s)
{
    if(s == 0) return "STUDYING";
    if(s == 1) return "WARNING";
    return "GRADUATED";
}

void showDeptMenu(const char* DEPT_NAMES[], int TOTAL_DEPTS)
{
    cout << "\n  Select Department:\n";
    for(int i = 0; i < TOTAL_DEPTS; i++)
    {
        cout << "    " << i << ". " << DEPT_NAMES[i] << "\n";
    }
    cout << "  Enter choice (0-" << TOTAL_DEPTS-1 << "): ";
}

// ============================================================
// MAKING THE 3D CAMPUS GRID
// ============================================================

int*** makeCampus(int d, int f, int r)
{
    int*** grid = new int**[d];
    
    for(int i = 0; i < d; i++)
    {
        grid[i] = new int*[f];
        
        for(int j = 0; j < f; j++)
        {
            grid[i][j] = new int[r];
            
            for(int k = 0; k < r; k++)
            {
                grid[i][j][k] = 0;
            }
        }
    }
    return grid;
}

void deleteCampus(int*** grid, int d, int f)
{
    for(int i = 0; i < d; i++)
    {
        for(int j = 0; j < f; j++)
            delete[] grid[i][j];
        delete[] grid[i];
    }
    delete[] grid;
}

// ============================================================
// ADDING A NEW STUDENT
// ============================================================

void addStudent(int*** campus,
                char**& nameArr, int*& idArr, float*& gpaArr, int*& statusArr,
                int& totalStudents,
                int deptCount, int floorCount, int roomCount,
                int chosenDept,
                const char* DEPT_NAMES[])
{
    char tempName[200];
    cout << "\n  Enter student name: ";
    cin.ignore();
    cin.getline(tempName, 200);
    
    cout << "\n  Department: " << DEPT_NAMES[chosenDept] << " (Dept " << chosenDept << ")\n";
    
    int newId = 1001;
    if(totalStudents > 0)
        newId = idArr[totalStudents - 1] + 1;
    
    int oldSize = totalStudents;
    int newSize = oldSize + 1;
    
    // grow name array
    char** newNameArr = new char*[newSize];
    for(int i = 0; i < oldSize; i++) 
        newNameArr[i] = nameArr[i];
    newNameArr[oldSize] = copyText(tempName);
    if(oldSize > 0) 
        delete[] nameArr;
    nameArr = newNameArr;
    
    // grow id array
    int* newIdArr = new int[newSize];
    for(int i = 0; i < oldSize; i++) 
        newIdArr[i] = idArr[i];
    newIdArr[oldSize] = newId;
    if(oldSize > 0) 
        delete[] idArr;
    idArr = newIdArr;
    
    // grow gpa array
    float* newGpaArr = new float[newSize];
    for(int i = 0; i < oldSize; i++) 
        newGpaArr[i] = gpaArr[i];
    newGpaArr[oldSize] = 2.50;
    if(oldSize > 0) 
        delete[] gpaArr;
    gpaArr = newGpaArr;
    
    // grow status array
    int* newStatusArr = new int[newSize];
    for(int i = 0; i < oldSize; i++) 
        newStatusArr[i] = statusArr[i];
    newStatusArr[oldSize] = 0;
    if(oldSize > 0) 
        delete[] statusArr;
    statusArr = newStatusArr;
    
    totalStudents++;
    
    // FIND EMPTY ROOM IN CHOSEN DEPARTMENT
    int dept = chosenDept;
    int floor = -1;
    int room = -1;
    bool found = false;
    
    for(int f = 0; f < floorCount; f++)
    {
        for(int r = 0; r < roomCount; r++)
        {
            if(campus[dept][f][r] == 0)
            {
                floor = f;
                room = r;
                found = true;
                break;
            }
        }
        if(found) break;
    }
    
    if(!found)
    {
        cout << "\n  " << DEPT_NAMES[chosenDept] << " department is FULL!\n";
        delete[] nameArr[totalStudents - 1];
        totalStudents--;
        return;
    }
    
    // ASSIGN THE ROOM
    campus[dept][floor][room] = newId;
    
    cout << "\n  Student added SUCCESSFULLY!"
         << "\n    ID     : " << newId
         << "\n    Name   : " << nameArr[totalStudents - 1]
         << "\n    Dept   : " << DEPT_NAMES[chosenDept]
         << "\n    Floor  : " << floor
         << "\n    Room   : " << room << "\n";
    cout << "\n  [TIP] Press J then enter " << dept << " and " << floor << " to see this student\n";
}

// ============================================================
// TIME STEP - UPDATES GPA AND HANDLES GRADUATION
// ============================================================

void doTimeStep(int*** campus,
                char**& nameArr, int*& idArr, float*& gpaArr, int*& statusArr,
                int& totalStudents,
                int deptCount, int floorCount, int roomCount,
                int& stepNum)
{
    stepNum++;
    cout << "\n  === STEP " << stepNum << " ===\n";
    
    int i = 0;
    while(i < totalStudents)
    {
        int r = (rand() % 10) + 1;
        unsigned char flag = 0;
        
        if(r <= 5)  // progress
        {
            gpaArr[i] = gpaArr[i] + 0.1;
            if(gpaArr[i] > 4.0) 
                gpaArr[i] = 4.0;
            statusArr[i] = 0;
            flag = flag | 1;
            cout << "  [PROGRESS] " << nameArr[i] << " (ID " << idArr[i] << ") GPA now " << gpaArr[i] << "\n";
            i++;
        }
        else if(r <= 8)  // steady
        {
            statusArr[i] = 0;
            cout << "  [STEADY]   " << nameArr[i] << " (ID " << idArr[i] << ") no change\n";
            i++;
        }
        else if(r == 9)  // warning
        {
            statusArr[i] = 1;
            gpaArr[i] = 0.2;
            flag = flag | 2;
            cout << "  [WARNING]  " << nameArr[i] << " (ID " << idArr[i] << ") GPA set to " << gpaArr[i] << "\n";
            i++;
        }
        else  // r == 10 GRADUATION
        {
            statusArr[i] = 2;
            flag = flag | 4;
            
            cout << "  [GRADUATED] " << nameArr[i] << " (ID " << idArr[i] << ") finished!\n";
            
            if(flag & 4)
                cout << "    >> graduated in this step <<\n";
            
            // FIND AND CLEAR ROOM
            int targetId = idArr[i];
            for(int d = 0; d < deptCount; d++)
            {
                for(int f = 0; f < floorCount; f++)
                {
                    for(int rm = 0; rm < roomCount; rm++)
                    {
                        if(campus[d][f][rm] == targetId)
                        {
                            campus[d][f][rm] = 0;
                            cout << "    Cleared room at Dept " << d << ", Floor " << f << ", Room " << rm << "\n";
                        }
                    }
                }
            }
            
            // FREE NAME MEMORY
            delete[] nameArr[i];
            
            // SHIFT ALL ARRAYS LEFT
            for(int j = i; j < totalStudents - 1; j++)
            {
                nameArr[j] = nameArr[j + 1];
                idArr[j] = idArr[j + 1];
                gpaArr[j] = gpaArr[j + 1];
                statusArr[j] = statusArr[j + 1];
            }
            
            // SHRINK ARRAYS
            int newN = totalStudents - 1;
            
            char** newNameArr = new char*[newN];
            for(int j = 0; j < newN; j++) newNameArr[j] = nameArr[j];
            delete[] nameArr;
            nameArr = newNameArr;
            
            int* newIdArr = new int[newN];
            for(int j = 0; j < newN; j++) newIdArr[j] = idArr[j];
            delete[] idArr;
            idArr = newIdArr;
            
            float* newGpaArr = new float[newN];
            for(int j = 0; j < newN; j++) newGpaArr[j] = gpaArr[j];
            delete[] gpaArr;
            gpaArr = newGpaArr;
            
            int* newStatusArr = new int[newN];
            for(int j = 0; j < newN; j++) newStatusArr[j] = statusArr[j];
            delete[] statusArr;
            statusArr = newStatusArr;
            
            totalStudents--;
            
            // DO NOT INCREMENT i - next student shifts into this position
            cout << "    Student removed. " << totalStudents << " students remaining.\n";
        }
    }
}

// ============================================================
// SHOW DASHBOARD
// ============================================================

void showDashboard(int*** campus,
                   char** nameArr, int* idArr, float* gpaArr, int* statusArr,
                   int totalStudents,
                   int deptCount, int floorCount, int roomCount,
                   int curDept, int curFloor, int stepNum,
                   const char* DEPT_NAMES[])
{
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
    
    cout << "==============================================================================\n";
    cout << "                         UNIVAULT - DASHBOARD\n";
    cout << "==============================================================================\n";
    
    if(curDept < 10)
        cout << "[DEPT: 0" << curDept << " - " << DEPT_NAMES[curDept] << "]";
    else
        cout << "[DEPT: " << curDept << " - " << DEPT_NAMES[curDept] << "]";
    
    if(curFloor < 10)
        cout << "  [FLOOR: 0" << curFloor << "]";
    else
        cout << "  [FLOOR: " << curFloor << "]";
    
    cout << "  [ROOMS: " << roomCount << "]";
    cout << "  [STEP: " << stepNum << "]\n";
    
    cout << "------------------------------------------------------------------------------\n";
    cout << left << setw(18) << "ROOM ADDR" << setw(8) << "ID" << setw(22) << "NAME" << setw(8) << "GPA" << "STATUS\n";
    cout << "------------------------------------------------------------------------------\n";
    
    for(int r = 0; r < roomCount; r++)
    {
        int* ptr = &campus[curDept][curFloor][r];
        int occupant = *ptr;
        
        cout << left << setw(18) << (void*)ptr;
        
        if(occupant == 0)
        {
            cout << left << setw(8) << "----"
                 << setw(22) << "------------------"
                 << setw(8) << "---"
                 << "-----\n";
        }
        else
        {
            int idx = -1;
            for(int k = 0; k < totalStudents; k++)
            {
                if(idArr[k] == occupant)
                {
                    idx = k;
                    break;
                }
            }
            
            if(idx == -1)
            {
                cout << left << setw(8) << occupant 
                     << setw(22) << "ERROR" 
                     << setw(8) << "?" 
                     << "???\n";
            }
            else
            {
                cout << left << setw(8) << idArr[idx]
                     << setw(22) << nameArr[idx]
                     << setw(8) << fixed << setprecision(2) << gpaArr[idx];
                
                if(statusArr[idx] == 0)
                    cout << "STUDYING\n";
                else if(statusArr[idx] == 1)
                    cout << "WARNING\n";
                else
                    cout << "GRADUATED\n";
            }
        }
    }
    
    cout << "------------------------------------------------------------------------------\n";
    cout << "[ENTER] Run 1 Step | [E] Add Student | [J] Jump | [F] Find | [S] Save | [X] Exit\n";
    cout << "==============================================================================\n";
    cout << "> ";
}

// ============================================================
// SEARCH FOR STUDENT BY ID
// ============================================================

void searchStudent(int* idArr, char** nameArr, float* gpaArr, int* statusArr,
                   int totalStudents,
                   int*** campus, int deptCount, int floorCount, int roomCount,
                   const char* DEPT_NAMES[])
{
    cout << "\n  Enter ID to find: ";
    int sid;
    cin >> sid;
    
    int pos = -1;
    for(int i = 0; i < totalStudents; i++)
    {
        if(idArr[i] == sid)
        {
            pos = i;
            break;
        }
    }
    
    if(pos == -1)
    {
        cout << "\n  NOT FOUND - No student with ID " << sid << "\n";
        return;
    }
    
    // find room coordinates
    int dept = -1, floor = -1, room = -1;
    for(int d = 0; d < deptCount; d++)
    {
        for(int f = 0; f < floorCount; f++)
        {
            for(int r = 0; r < roomCount; r++)
            {
                if(campus[d][f][r] == sid)
                {
                    dept = d;
                    floor = f;
                    room = r;
                    break;
                }
            }
            if(dept != -1) break;
        }
        if(dept != -1) break;
    }
    
    cout << "\n  ---- STUDENT FOUND ----\n";
    cout << "  ID     : " << idArr[pos] << "\n";
    cout << "  Name   : " << nameArr[pos] << "\n";
    cout << "  GPA    : " << fixed << setprecision(2) << gpaArr[pos] << "\n";
    cout << "  Status : " << getStatusText(statusArr[pos]) << "\n";
    
    if(dept != -1)
    {
        cout << "  Dept   : " << DEPT_NAMES[dept] << "\n";
        cout << "  Floor  : " << floor << "\n";
        cout << "  Room   : " << room << "\n";
        cout << "\n  [TIP] Press J to go to Dept " << dept << ", Floor " << floor << "\n";
    }
    else
    {
        cout << "\n  ERROR: Student ID " << sid << " is in registry but NOT in grid!\n";
        cout << "  This is a LOADING issue. The grid was not restored properly.\n";
    }
    cout << "  -----------------------\n";
}

// ============================================================
// JUMP TO DIFFERENT DEPARTMENT/FLOOR
// ============================================================

void moveView(int deptCount, int floorCount, int& curDept, int& curFloor,
              const char* DEPT_NAMES[])
{
    cout << "\n  Available Departments:\n";
    for(int i = 0; i < deptCount; i++)
    {
        cout << "    " << i << ". " << DEPT_NAMES[i] << "\n";
    }
    
    cout << "\n  Enter Department (0 to " << deptCount - 1 << "): ";
    int d;
    cin >> d;
    
    if(d < 0 || d >= deptCount)
    {
        cout << "  Invalid department!\n";
        return;
    }
    curDept = d;
    cout << "  Selected: " << DEPT_NAMES[curDept] << "\n";
    
    cout << "  Enter Floor (0 to " << floorCount - 1 << "): ";
    int f;
    cin >> f;
    
    if(f < 0 || f >= floorCount)
    {
        cout << "  Invalid floor!\n";
        return;
    }
    curFloor = f;
    
    cout << "  Now showing " << DEPT_NAMES[curDept] << " (Dept " << curDept << "), Floor " << curFloor << "\n";
}

// ============================================================
// SAVE EVERYTHING TO FILE
// ============================================================
// FIX: Each student record is saved as TWO lines:
//   Line 1: id  gpa  status  dept  floor  room
//   Line 2: name (may contain spaces)
// This ensures names with spaces load back correctly.
// ============================================================

void saveAll(int*** campus,
             char** nameArr, int* idArr, float* gpaArr, int* statusArr,
             int totalStudents,
             int deptCount, int floorCount, int roomCount)
{
    ofstream out("database.txt");
    if(!out)
    {
        cout << "\n  Error creating file!\n";
        return;
    }
    
    out << totalStudents << "\n";
    
    for(int i = 0; i < totalStudents; i++)
    {
        // Find where this student is in the grid
        int dept = -1, floor = -1, room = -1;
        for(int d = 0; d < deptCount; d++)
        {
            for(int f = 0; f < floorCount; f++)
            {
                for(int r = 0; r < roomCount; r++)
                {
                    if(campus[d][f][r] == idArr[i])
                    {
                        dept = d;
                        floor = f;
                        room = r;
                        break;
                    }
                }
                if(dept != -1) break;
            }
            if(dept != -1) break;
        }

        // Line 1: all numeric fields
        out << idArr[i] << " " << gpaArr[i] << " " << statusArr[i] << " ";
        out << dept << " " << floor << " " << room << "\n";
        // Line 2: name on its own line (safe for names with spaces)
        out << nameArr[i] << "\n";
    }
    
    out.close();
    cout << "\n  Data saved to database.txt\n";
}

// ============================================================
// LOAD DATA FROM FILE
// ============================================================
// FIX: Reads numeric fields first, then uses getline for the
//      name so that spaces in names are handled correctly.
// ============================================================

bool loadAll(int*** campus,
             char**& nameArr, int*& idArr, float*& gpaArr, int*& statusArr,
             int& totalStudents,
             int deptCount, int floorCount, int roomCount)
{
    ifstream in("database.txt");
    if(!in) 
        return false;
    
    int n;
    in >> n;
    in.ignore(1000, '\n');  // consume newline after the count
    
    nameArr   = new char*[n];
    idArr     = new int[n];
    gpaArr    = new float[n];
    statusArr = new int[n];
    totalStudents = n;
    
    char tempName[200];
    for(int i = 0; i < n; i++)
    {
        int id, st, dd, ff, rr;
        float g;

        // Read Line 1: numeric fields
        in >> id >> g >> st >> dd >> ff >> rr;
        in.ignore(1000, '\n');  // consume newline after numeric fields

        // Read Line 2: full name (handles spaces correctly)
        in.getline(tempName, 200);

        idArr[i]     = id;
        gpaArr[i]    = g;
        statusArr[i] = st;
        nameArr[i]   = copyText(tempName);

        // Validate coordinates before writing to grid
        if(dd >= 0 && dd < deptCount &&
           ff >= 0 && ff < floorCount &&
           rr >= 0 && rr < roomCount)
        {
            campus[dd][ff][rr] = id;
        }
    }
    
    in.close();
    cout << "\n  Loaded " << n << " students from database.txt\n";
    return true;
}

// ============================================================
// MAIN FUNCTION
// ============================================================

int main()
{
    srand(time(0));
    
    // Department names - LOCAL to main, NOT global
    const char* DEPT_NAMES[] = {
        "Cyber Security",
        "Software Engineering", 
        "Artificial Intelligence",
        "Data Science",
        "Social Sciences"
    };
    const int TOTAL_DEPTS = 5;
    
    int deptCount = 0, floorCount = 0, roomCount = 0;
    
    // read config
    ifstream configFile("config.txt");
    if(configFile)
    {
        configFile >> deptCount >> floorCount >> roomCount;
        configFile.close();
        cout << "  Config loaded: " << deptCount << " depts, " << floorCount << " floors, " << roomCount << " rooms\n";
    }
    else
    {
        cout << "  config.txt not found!\n";
        cout << "  Enter number of Departments: ";
        cin >> deptCount;
        cout << "  Enter number of Floors: ";
        cin >> floorCount;
        cout << "  Enter number of Rooms: ";
        cin >> roomCount;
        
        ofstream newConfig("config.txt");
        newConfig << deptCount << "\n" << floorCount << "\n" << roomCount << "\n";
        newConfig.close();
        cout << "  config.txt created\n";
    }
    
    int*** campus = makeCampus(deptCount, floorCount, roomCount);
    
    char** names = nullptr;
    int* ids = nullptr;
    float* gpas = nullptr;
    int* statuses = nullptr;
    int studentCount = 0;
    
    // Try to load existing data
    bool loaded = loadAll(campus, names, ids, gpas, statuses, studentCount, deptCount, floorCount, roomCount);
    if(!loaded)
    {
        cout << "  No existing database found. Starting fresh.\n";
    }
    
    int step = 0;
    int currentDept = 0;
    int currentFloor = 0;
    
    bool keepGoing = true;
    while(keepGoing)
    {
        showDashboard(campus, names, ids, gpas, statuses, studentCount,
                      deptCount, floorCount, roomCount,
                      currentDept, currentFloor, step, DEPT_NAMES);
        
        char choice;
        cin.get(choice);
        
        if(choice == '\n')
        {
            cout << "\n  >>> Running 1 time step... each student gets random event\n";
            doTimeStep(campus, names, ids, gpas, statuses, studentCount,
                       deptCount, floorCount, roomCount, step);
            cout << "\n  Press ENTER to continue...";
            cin.get();
        }
        else if(choice == 'E' || choice == 'e')
        {
            showDeptMenu(DEPT_NAMES, TOTAL_DEPTS);
            int selectedDept;
            cin >> selectedDept;
            
            if(selectedDept < 0 || selectedDept >= deptCount)
            {
                cout << "  Invalid department choice!\n";
                cout << "\n  Press ENTER to continue...";
                cin.ignore(1000, '\n');
                cin.get();
            }
            else
            {
                addStudent(campus, names, ids, gpas, statuses, studentCount,
                           deptCount, floorCount, roomCount, selectedDept, DEPT_NAMES);
                cout << "\n  Press ENTER to continue...";
                cin.ignore(1000, '\n');
                cin.get();
            }
        }
        else if(choice == 'J' || choice == 'j')
        {
            moveView(deptCount, floorCount, currentDept, currentFloor, DEPT_NAMES);
            cout << "\n  Press ENTER to continue...";
            cin.get();
        }
        else if(choice == 'F' || choice == 'f')
        {
            cin.ignore(1000, '\n');
            searchStudent(ids, names, gpas, statuses, studentCount,
                          campus, deptCount, floorCount, roomCount, DEPT_NAMES);
            cout << "\n  Press ENTER to continue...";
            cin.ignore(1000, '\n');
            cin.get();
        }
        else if(choice == 'S' || choice == 's')
        {
            cin.ignore(1000, '\n');
            saveAll(campus, names, ids, gpas, statuses, studentCount,
                    deptCount, floorCount, roomCount);
            cout << "\n  Press ENTER to continue...";
            cin.get();
        }
        else if(choice == 'X' || choice == 'x')
        {
            cout << "\n  Are you sure you want to exit? (y/n): ";
            char confirm;
            cin >> confirm;
            
            if(confirm == 'y' || confirm == 'Y')
            {
                cout << "\n  Saving data before exit...\n";
                saveAll(campus, names, ids, gpas, statuses, studentCount,
                        deptCount, floorCount, roomCount);
                keepGoing = false;
            }
            else
            {
                cout << "\n  Exit cancelled. Returning to dashboard...\n";
                cout << "\n  Press ENTER to continue...";
                cin.ignore(1000, '\n');
                cin.get();
            }
        }
        else
        {
            cout << "\n  Unknown command! Use: ENTER, E, J, F, S, or X\n";
            cout << "\n  Press ENTER to continue...";
            cin.ignore(1000, '\n');
            cin.get();
        }
    }
    
    // cleanup
    for(int i = 0; i < studentCount; i++)
        delete[] names[i];
    delete[] names;
    delete[] ids;
    delete[] gpas;
    delete[] statuses;
    
    deleteCampus(campus, deptCount, floorCount);
    
    cout << "\n  Thank you for using UniVault! Goodbye!\n";
    return 0;
}
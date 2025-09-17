#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

const int MAX_COURSES = 1000;
const int MAX_STUDENTS = 1000;

// Parallel arrays for courses
string courseIds[MAX_COURSES];
string titles[MAX_COURSES];
int creditHours[MAX_COURSES];
int courseCount = 0;

// Parallel arrays for students
string studentIds[MAX_STUDENTS];
string studentNames[MAX_STUDENTS];
string studentPrograms[MAX_STUDENTS];
int studentYears[MAX_STUDENTS];
int studentCount = 0;

// ---------------- File Utilities ----------------
void loadCourses() {
    courseCount = 0;
    ifstream inFile("courses.txt");
    string line;
    while (getline(inFile, line) && courseCount < MAX_COURSES) {
        stringstream ss(line);
        string id, title, credits;
        getline(ss, id, ',');
        getline(ss, title, ',');
        getline(ss, credits, ',');
        if (!id.empty() && !title.empty() && !credits.empty()) {
            courseIds[courseCount] = id;
            titles[courseCount] = title;
            creditHours[courseCount] = stoi(credits);
            courseCount++;
        }
    }
    inFile.close();
}

void saveCourses() {
    ofstream outFile("courses.txt", ios::trunc);
    for (int i = 0; i < courseCount; i++) {
        outFile << courseIds[i] << "," << titles[i] << "," << creditHours[i] << "\n";
    }
    outFile.close();
}

void loadStudents() {
    studentCount = 0;
    ifstream inFile("students.csv");
    string line;
    bool skipHeader = true;
    while (getline(inFile, line) && studentCount < MAX_STUDENTS) {
        if (skipHeader) { // skip the first line (header row)
            skipHeader = false;
            continue;
        }
        stringstream ss(line);
        string id, name, program, yearStr;
        getline(ss, id, ',');
        getline(ss, name, ',');
        getline(ss, program, ',');
        getline(ss, yearStr, ',');
        if (!id.empty() && !name.empty() && !program.empty() && !yearStr.empty()) {
            studentIds[studentCount] = id;
            studentNames[studentCount] = name;
            studentPrograms[studentCount] = program;
            studentYears[studentCount] = stoi(yearStr);
            studentCount++;
        }
    }
    inFile.close();
}

// ---------------- User Functions ----------------
bool loginUser() {
    string username, password;
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;

    ifstream userFile("users.txt");
    string line;
    while (getline(userFile, line)) {
        string fileUser, filePass;
        stringstream ss(line);
        getline(ss, fileUser, ',');
        getline(ss, filePass, ',');
        if (username == fileUser && password == filePass) {
            userFile.close();
            return true;
        }
    }
    userFile.close();
    return false;
}

void registerUser() {
    string username, password;
    cout << "Choose username: ";
    cin >> username;
    cout << "Choose password: ";
    cin >> password;

    ofstream userFile("users.txt", ios::app);
    userFile << username << "," << password << "\n";
    userFile.close();
    cout << "User registered successfully!\n";
}

// ---------------- Course Functions ----------------
void listCourses() {
    if (courseCount == 0) {
        cout << "No courses available.\n";
        return;
    }
    cout << "Course List:\n";
    for (int i = 0; i < courseCount; i++) {
        cout << courseIds[i] << " | " << titles[i] << " | " << creditHours[i] << " credits\n";
    }
}

void addCourse() {
    if (courseCount >= MAX_COURSES) {
        cout << "Course storage full!\n";
        return;
    }
    string id, title;
    int credits;
    cout << "Enter course ID: ";
    cin >> id;
    for (int i = 0; i < courseCount; i++) {
        if (courseIds[i] == id) {
            cout << "Course ID already exists!\n";
            return;
        }
    }
    cin.ignore();
    cout << "Enter course title: ";
    getline(cin, title);
    cout << "Enter credit hours (1-6): ";
    cin >> credits;
    if (credits < 1 || credits > 6) {
        cout << "Invalid credit hours!\n";
        return;
    }
    courseIds[courseCount] = id;
    titles[courseCount] = title;
    creditHours[courseCount] = credits;
    courseCount++;
    saveCourses();
    cout << "Course added successfully!\n";
}

void deleteCourse() {
    string id;
    cout << "Enter course ID to delete: ";
    cin >> id;
    for (int i = 0; i < courseCount; i++) {
        if (courseIds[i] == id) {
            for (int j = i; j < courseCount - 1; j++) {
                courseIds[j] = courseIds[j + 1];
                titles[j] = titles[j + 1];
                creditHours[j] = creditHours[j + 1];
            }
            courseCount--;
            saveCourses();
            cout << "Course deleted successfully!\n";
            return;
        }
    }
    cout << "Course not found.\n";
}

void searchCourse() {
    string keyword;
    cin.ignore();
    cout << "Enter course ID or title keyword: ";
    getline(cin, keyword);
    bool found = false;
    for (int i = 0; i < courseCount; i++) {
        if (courseIds[i] == keyword || titles[i].find(keyword) != string::npos) {
            cout << courseIds[i] << " | " << titles[i] << " | " << creditHours[i] << " credits\n";
            found = true;
        }
    }
    if (!found) cout << "No matching course found.\n";
}

void updateCourse() {
    string id;
    cout << "Enter course ID to update: ";
    cin >> id;
    for (int i = 0; i < courseCount; i++) {
        if (courseIds[i] == id) {
            cin.ignore();
            cout << "Enter new title: ";
            getline(cin, titles[i]);
            cout << "Enter new credit hours (1-6): ";
            cin >> creditHours[i];
            if (creditHours[i] < 1 || creditHours[i] > 6) {
                cout << "Invalid credit hours!\n";
                return;
            }
            saveCourses();
            cout << "Course updated successfully!\n";
            return;
        }
    }
    cout << "Course not found.\n";
}

// ---------------- Student Function ----------------
void listStudents() {
    if (studentCount == 0) {
        cout << "No students found.\n";
        return;
    }
    cout << "Student List:\n";
    for (int i = 0; i < studentCount; i++) {
        cout << studentIds[i] << " | " << studentNames[i]
             << " | " << studentPrograms[i]
             << " | Year " << studentYears[i] << "\n";
    }
}

// ---------------- Main Menu ----------------
void menu() {
    int choice;
    do {
        cout << "\n--- Course Management System ---\n";
        cout << "1. Add Course\n";
        cout << "2. Delete Course\n";
        cout << "3. Search Course\n";
        cout << "4. Update Course\n";
        cout << "5. List All Courses\n";
        cout << "6. List All Students\n";
        cout << "7. Logout/Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1: addCourse(); break;
            case 2: deleteCourse(); break;
            case 3: searchCourse(); break;
            case 4: updateCourse(); break;
            case 5: listCourses(); break;
            case 6: listStudents(); break;
            case 7: cout << "Logging out...\n"; break;
            default: cout << "Invalid choice.\n";
        }
    } while (choice != 7);
}

int main() {
    loadCourses();
    loadStudents();

    int choice;
    cout << "--- Welcome to CMS ---\n";
    cout << "1. Login\n2. Register\nChoice: ";
    cin >> choice;
    bool loggedIn = false;

    if (choice == 1) {
        if (loginUser()) {
            cout << "Login successful!\n";
            loggedIn = true;
        } else {
            cout << "Login failed.\n";
            return 0;
        }
    } else if (choice == 2) {
        registerUser();
        cout << "Now login to continue.\n";
        if (loginUser()) {
            cout << "Login successful!\n";
            loggedIn = true;
        } else {
            cout << "Login failed.\n";
            return 0;
        }
    } else {
        cout << "Invalid choice.\n";
        return 0;
    }

    if (loggedIn) menu();
    return 0;
}

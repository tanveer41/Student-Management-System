#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <algorithm>
#ifdef _WIN32
#include <windows.h> // For Windows
#else
#include <unistd.h> // For macOS/Linux
#endif
using namespace std;

// Function to clear the screen
void clearScreen() {
#ifdef _WIN32
    system("cls"); // Clear screen on Windows
#else
    system("clear"); // Clear screen on macOS/Linux
#endif
}

class Student {
private:
    string password = "admin123"; // Default password

    struct StudentData {
        int roll;
        string name;
        vector<int> marks; // Marks for 5 subjects
        vector<int> attendance; // 1 for present, 0 for absent
    };

    vector<StudentData> students; // Store all students in memory

public:
    // Function to display the main menu
    void menu() {
        loadStudents(); // Load students from file at the start
        int choice;
        while (true) {
            clearScreen(); // Clear the screen
            cout << "************************************\n";
            cout << "Welcome to Student Management System\n";
            cout << "************************************\n";
            cout << "Press 1 To Add Student\n";
            cout << "Press 2 To View All Students\n";
            cout << "Press 3 To Search Student\n";
            cout << "Press 4 To Update Student\n";
            cout << "Press 5 To Delete Student\n";
            cout << "Press 6 To Manage Attendance\n";
            cout << "Press 7 To Manage Marks\n";
            cout << "Press 8 To Generate Reports\n";
            cout << "Press 9 To Exit\n";
            cout << "Enter your choice: ";
            cin >> choice;
            cin.ignore();

            switch (choice) {
                case 1:
                    addStudent();
                    break;
                case 2:
                    viewAllStudents();
                    break;
                case 3:
                    searchStudent();
                    break;
                case 4:
                    updateStudent();
                    break;
                case 5:
                    deleteStudent();
                    break;
                case 6:
                    manageAttendance();
                    break;
                case 7:
                    manageMarks();
                    break;
                case 8:
                    generateReports();
                    break;
                case 9:
                    saveStudents(); // Save students to file before exiting
                    cout << "Exiting the program. Goodbye!\n";
                    return;
                default:
                    cout << "Invalid choice! Please try again.\n";
                    break;
            }
            cout << "\nPress Enter to continue...";
            cin.ignore(); // Wait for user input before clearing the screen
        }
    }

    // Function to add a new student
    void addStudent() {
        clearScreen(); // Clear the screen
        StudentData student;
        cout << "Enter Student Details:\n";
        cout << "Roll Number: ";
        cin >> student.roll;
        cin.ignore();
        cout << "Name: ";
        getline(cin, student.name);

        // Initialize marks and attendance
        student.marks.resize(5, 0); // 5 subjects
        student.attendance.resize(30, 0); // 30 days attendance

        students.push_back(student);
        cout << "Student added successfully!\n";
    }

    // Function to view all students
    void viewAllStudents() {
        clearScreen(); // Clear the screen
        if (students.empty()) {
            cout << "No student records found!\n";
            return;
        }

        cout << "Roll No\tName\t\tMarks (%)\tAttendance (%)\n";
        for (const auto& student : students) {
            cout << student.roll << "\t" << student.name << "\t\t";
            cout << fixed << setprecision(2) << calculateMarksPercentage(student.marks) << "%\t\t";
            cout << fixed << setprecision(2) << calculateAttendancePercentage(student.attendance) << "%\n";
        }
    }

    // Function to search for a student
    void searchStudent() {
        clearScreen(); // Clear the screen
        int searchRoll;
        cout << "Enter Roll Number to search: ";
        cin >> searchRoll;

        bool found = false;
        for (const auto& student : students) {
            if (student.roll == searchRoll) {
                cout << "Student Found:\n";
                cout << "Roll No: " << student.roll << "\n";
                cout << "Name: " << student.name << "\n";
                cout << "Marks (%): " << fixed << setprecision(2) << calculateMarksPercentage(student.marks) << "%\n";
                cout << "Attendance (%): " << fixed << setprecision(2) << calculateAttendancePercentage(student.attendance) << "%\n";
                found = true;
                break;
            }
        }
        if (!found) cout << "Student not found!\n";
    }

    // Function to update student details
    void updateStudent() {
        clearScreen(); // Clear the screen
        int updateRoll;
        cout << "Enter Roll Number to update: ";
        cin >> updateRoll;

        for (auto& student : students) {
            if (student.roll == updateRoll) {
                cout << "Enter new name: ";
                cin.ignore();
                getline(cin, student.name);
                cout << "Student details updated successfully!\n";
                return;
            }
        }
        cout << "Student not found!\n";
    }

    // Function to delete a student
    void deleteStudent() {
        clearScreen(); // Clear the screen
        int deleteRoll;
        cout << "Enter Roll Number to delete: ";
        cin >> deleteRoll;

        auto it = remove_if(students.begin(), students.end(), [deleteRoll](const StudentData& student) {
            return student.roll == deleteRoll;
        });

        if (it != students.end()) {
            students.erase(it, students.end());
            cout << "Student deleted successfully!\n";
        } else {
            cout << "Student not found!\n";
        }
    }

    // Function to manage attendance
    void manageAttendance() {
        clearScreen(); // Clear the screen
        int roll;
        cout << "Enter Roll Number to manage attendance: ";
        cin >> roll;

        for (auto& student : students) {
            if (student.roll == roll) {
                cout << "Enter attendance for 30 days (1 for present, 0 for absent):\n";
                for (int i = 0; i < 30; i++) {
                    cout << "Day " << i + 1 << ": ";
                    cin >> student.attendance[i];
                }
                cout << "Attendance updated successfully!\n";
                cout << "New Attendance (%): " << fixed << setprecision(2) << calculateAttendancePercentage(student.attendance) << "%\n";
                return;
            }
        }
        cout << "Student not found!\n";
    }

    // Function to manage marks
    void manageMarks() {
        clearScreen(); // Clear the screen
        int roll;
        cout << "Enter Roll Number to manage marks: ";
        cin >> roll;

        for (auto& student : students) {
            if (student.roll == roll) {
                cout << "Enter marks for 5 subjects (out of 100):\n";
                for (int i = 0; i < 5; i++) {
                    cout << "Subject " << i + 1 << ": ";
                    cin >> student.marks[i];
                }
                cout << "Marks updated successfully!\n";
                cout << "New Marks (%): " << fixed << setprecision(2) << calculateMarksPercentage(student.marks) << "%\n";
                return;
            }
        }
        cout << "Student not found!\n";
    }

    // Function to generate reports
   // Function to generate reports
void generateReports() {
    clearScreen(); // Clear the screen before showing the menu
    int choice;
    cout << "************************************\n";
    cout << "Generate Reports\n";
    cout << "************************************\n";
    cout << "1. List of all students\n";
    cout << "2. Toppers (Top 3 students by marks)\n";
    cout << "3. Students with low attendance (< 75%)\n";
    cout << "Enter your choice: ";
    cin >> choice;

    switch (choice) {
        case 1:
            viewAllStudents();
            break;
        case 2:
            generateToppersReport();
            break;
        case 3:
            generateLowAttendanceReport();
            break;
        default:
            cout << "Invalid choice!\n";
            break;
    }
}

    // Function to generate toppers report
void generateToppersReport() {
    if (students.empty()) {
        cout << "No student records found!\n";
        return;
    }

    // Create a vector of pairs: (Roll, Marks %)
    vector<pair<int, double>> toppers;
    for (const auto& student : students) {
        toppers.push_back({student.roll, calculateMarksPercentage(student.marks)});
    }

    // Sort toppers by marks in descending order
    sort(toppers.begin(), toppers.end(), [](const pair<int, double>& a, const pair<int, double>& b) {
        return a.second > b.second;
    });

    // Display top 3 toppers
    cout << "Top 3 Toppers:\n";
    cout << "Roll No\tMarks (%)\n";
    for (int i = 0; i < min(3, (int)toppers.size()); i++) {
        cout << toppers[i].first << "\t" << fixed << setprecision(2) << toppers[i].second << "%\n";
    }
}

// Function to generate low attendance report
void generateLowAttendanceReport() {
    if (students.empty()) {
        cout << "No student records found!\n";
        return;
    }

    cout << "Students with Low Attendance (< 75%):\n";
    cout << "Roll No\tName\t\tAttendance (%)\n";
    for (const auto& student : students) {
        double attendancePercentage = calculateAttendancePercentage(student.attendance);
        if (attendancePercentage < 75) {
            cout << student.roll << "\t" << student.name << "\t\t" << fixed << setprecision(2) << attendancePercentage << "%\n";
        }
    }
}
    // Function to calculate attendance percentage
    double calculateAttendancePercentage(const vector<int>& attendance) {
        int presentDays = 0;
        for (int att : attendance) presentDays += att;
        return (presentDays / 30.0) * 100;
    }

    // Function to calculate marks percentage
    double calculateMarksPercentage(const vector<int>& marks) {
        double total = 0;
        for (int mark : marks) total += mark;
        return (total / 500.0) * 100; // 5 subjects, each out of 100
    }

    // Function to authenticate user
    bool authenticate() {
        clearScreen(); // Clear the screen
        string inputPassword;
        cout << "Enter password: ";
        cin >> inputPassword;
        return inputPassword == password;
    }

    // Function to load students from file
    void loadStudents() {
        ifstream file("students.txt");
        if (!file.is_open()) return;

        students.clear();
        string line;
        while (getline(file, line)) {
            StudentData student;
            size_t pos = 0;
            int count = 0;
            while ((pos = line.find(',')) != string::npos) {
                string token = line.substr(0, pos);
                if (count == 0) student.roll = stoi(token);
                else if (count == 1) student.name = token;
                else if (count < 7) student.marks.push_back(stoi(token));
                else student.attendance.push_back(stoi(token));
                line.erase(0, pos + 1);
                count++;
            }
            students.push_back(student);
        }
        file.close();
    }

    // Function to save students to file
    void saveStudents() {
        ofstream file("students.txt");
        if (!file.is_open()) {
            cout << "Error saving student data!\n";
            return;
        }

        for (const auto& student : students) {
            file << student.roll << "," << student.name << ",";
            for (int mark : student.marks) file << mark << ",";
            for (int att : student.attendance) file << att << ",";
            file << "\n";
        }
        file.close();
    }
};

int main() {
    Student system;
    if (system.authenticate()) {
        system.menu();
    } else {
        cout << "Access denied! Incorrect password.\n";
    }
    return 0;
}
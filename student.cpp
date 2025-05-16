#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <algorithm>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif
using namespace std;

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

class Student {
private:
    string password = "admin123";

    struct StudentData {
        int roll;
        string name;
        vector<int> marks;
        vector<int> attendance;
    };

    vector<StudentData> students;

public:
    void menu() {
        if (!authenticate()) {
            cout << "Access Denied! Exiting...\n";
            return;
        }
        loadStudents();
        int choice;
        while (true) {
            clearScreen();
            cout << "************************************\n";
            cout << "Welcome to Student Management System\n";
            cout << "************************************\n";
            cout << "1. Add Student\n";
            cout << "2. View All Students\n";
            cout << "3. Search Student\n";
            cout << "4. Update Student\n";
            cout << "5. Delete Student\n";
            cout << "6. Manage Attendance\n";
            cout << "7. Manage Marks\n";
            cout << "8. Generate Reports\n";
            cout << "9. Exit\n";
            cout << "Enter your choice: ";
            cin >> choice;
            cin.ignore();

            switch (choice) {
                case 1: addStudent(); break;
                case 2: viewAllStudents(); break;
                case 3: searchStudent(); break;
                case 4: updateStudent(); break;
                case 5: deleteStudent(); break;
                case 6: manageAttendance(); break;
                case 7: manageMarks(); break;
                case 8: generateReports(); break;
                case 9:
                    saveStudents();
                    cout << "Exiting the program. Goodbye!\n";
                    return;
                default: cout << "Invalid choice!\n"; break;
            }
            cout << "\nPress Enter to continue...";
            cin.ignore();
        }
    }

    void addStudent() {
        clearScreen();
        StudentData student;
        cout << "Enter Student Details:\n";
        cout << "Roll Number: ";
        cin >> student.roll;
        cin.ignore();
        cout << "Name: ";
        getline(cin, student.name);

        student.marks.resize(5, 0);
        student.attendance.resize(30, 0);

        students.push_back(student);
        cout << "Student added successfully!\n";
    }

    void viewAllStudents() {
        clearScreen();
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

    void searchStudent() {
        clearScreen();
        int searchRoll;
        cout << "Enter Roll Number to search: ";
        cin >> searchRoll;

        auto it = find_if(students.begin(), students.end(), [searchRoll](const StudentData& s) { return s.roll == searchRoll; });
        if (it != students.end()) {
            cout << "Student Found:\n";
            cout << "Roll No: " << it->roll << "\n";
            cout << "Name: " << it->name << "\n";
            cout << "Marks (%): " << fixed << setprecision(2) << calculateMarksPercentage(it->marks) << "%\n";
            cout << "Attendance (%): " << fixed << setprecision(2) << calculateAttendancePercentage(it->attendance) << "%\n";
        } else {
            cout << "Student not found!\n";
        }
    }

    void updateStudent() {
        clearScreen();
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

    void deleteStudent() {
        clearScreen();
        int deleteRoll;
        cout << "Enter Roll Number to delete: ";
        cin >> deleteRoll;

        auto it = remove_if(students.begin(), students.end(), [deleteRoll](const StudentData& s) { return s.roll == deleteRoll; });
        if (it != students.end()) {
            students.erase(it, students.end());
            cout << "Student deleted successfully!\n";
        } else {
            cout << "Student not found!\n";
        }
    }

    void manageAttendance() {
        clearScreen();
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
                return;
            }
        }
        cout << "Student not found!\n";
    }

    void manageMarks() {
        clearScreen();
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
                return;
            }
        }
        cout << "Student not found!\n";
    }

    void generateReports() {
        clearScreen();
        int choice;
        cout << "********** Generate Reports **********\n";
        cout << "1. List of all students\n";
        cout << "2. Toppers (Top 3 students by marks)\n";
        cout << "3. Students with low attendance (< 75%)\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: viewAllStudents(); break;
            case 2: generateToppersReport(); break;
            case 3: generateLowAttendanceReport(); break;
            default: cout << "Invalid choice!\n"; break;
        }
    }

    void generateToppersReport() {
        if (students.empty()) {
            cout << "No student records found!\n";
            return;
        }

        vector<pair<int, double>> toppers;
        for (const auto& student : students) {
            toppers.push_back({student.roll, calculateMarksPercentage(student.marks)});
        }

        sort(toppers.begin(), toppers.end(), [](const pair<int, double>& a, const pair<int, double>& b) {
            return a.second > b.second;
        });

        cout << "Top 3 Toppers:\n";
        cout << "Roll No\tMarks (%)\n";
        for (int i = 0; i < min(3, (int)toppers.size()); i++) {
            cout << toppers[i].first << "\t" << fixed << setprecision(2) << toppers[i].second << "%\n";
        }
    }

    void generateLowAttendanceReport() {
        if (students.empty()) {
            cout << "No student records found!\n";
            return;
        }

        cout << "Students with Low Attendance (< 75%):\n";
        cout << "Roll No\tName\t\tAttendance (%)\n";
        for (const auto& student : students) {
            double perc = calculateAttendancePercentage(student.attendance);
            if (perc < 75.0) {
                cout << student.roll << "\t" << student.name << "\t\t" << fixed << setprecision(2) << perc << "%\n";
            }
        }
    }

    double calculateAttendancePercentage(const vector<int>& attendance) {
        int presentDays = count(attendance.begin(), attendance.end(), 1);
        return (presentDays / 30.0) * 100.0;
    }

    double calculateMarksPercentage(const vector<int>& marks) {
        double total = accumulate(marks.begin(), marks.end(), 0);
        return (total / 500.0) * 100.0;
    }

    bool authenticate() {
        clearScreen();
        string inputPassword;
        cout << "Enter password: ";
        cin >> inputPassword;
        return inputPassword == password;
    }

    void loadStudents() {
        ifstream file("students.txt");
        if (!file) return;
        students.clear();

        while (!file.eof()) {
            StudentData student;
            file >> student.roll;
            file.ignore();
            getline(file, student.name, ',');
            student.marks.resize(5);
            for (int i = 0; i < 5; ++i) file >> student.marks[i];
            student.attendance.resize(30);
            for (int i = 0; i < 30; ++i) file >> student.attendance[i];
            if (file) students.push_back(student);
        }
        file.close();
    }

    void saveStudents() {
        ofstream file("students.txt");
        if (!file) {
            cout << "Error saving student data!\n";
            return;
        }

        for (const auto& student : students) {
            file << student.roll << "," << student.name << ",";
            for (int mark : student.marks) file << mark << " ";
            for (int att : student.attendance) file << att << " ";
            file << "\n";
        }
        file.close();
    }
};

int main() {
    Student s;
    s.menu();
    return 0;
}

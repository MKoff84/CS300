#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <cctype>
#include <limits>
#include <utility>

using namespace std;

// -------------------------------
// Data model
// -------------------------------
struct Course {
    string number;                 // e.g., "CSCI300"
    string title;                  // e.g., "Introduction to Algorithms"
    vector<string> prerequisites;  // e.g., ["CSCI200", "MATH201"]
};

// -------------------------------
// Utility helpers
// -------------------------------

// Trim leading/trailing whitespace
static string trim(const string& s) {
    size_t start = 0;
    while (start < s.size() && isspace(static_cast<unsigned char>(s[start]))) start++;

    size_t end = s.size();
    while (end > start && isspace(static_cast<unsigned char>(s[end - 1]))) end--;

    return s.substr(start, end - start);
}

// Convert to uppercase (course numbers should be case-insensitive for user input)
static string toUpper(string s) {
    for (char& c : s) c = static_cast<char>(toupper(static_cast<unsigned char>(c)));
    return s;
}

// Split a CSV line by commas (simple CSV: no quoted commas assumed per assignment format)
static vector<string> splitCSV(const string& line) {
    vector<string> tokens;
    string token;
    stringstream ss(line);

    while (getline(ss, token, ',')) {
        tokens.push_back(trim(token));
    }
    return tokens;
}

// -------------------------------
// Core functionality
// -------------------------------

/*
Loads courses from file into hash table.
Validates:
  - each non-empty line has at least 2 tokens (courseNumber, title)
  - every prerequisite listed exists as a courseNumber somewhere in the file

Returns true if load succeeds, false otherwise.
*/
static bool loadCoursesFromFile(
    const string& fileName,
    unordered_map<string, Course>& courses
) {
    ifstream file(fileName);
    if (!file.is_open()) {
        cout << "Error: Could not open file '" << fileName << "'." << endl;
        return false;
    }

    courses.clear();

    // Keep track of prerequisites we saw so we can validate after reading all courses
    vector<pair<string, string>> prereqEdges;
    // pair<courseNumber, prereqNumber>

    string line;
    unsigned int lineNumber = 0;

    while (getline(file, line)) {
        lineNumber++;

        line = trim(line);
        if (line.empty()) {
            continue; // ignore blank lines
        }

        vector<string> parts = splitCSV(line);

        // Rule: At least courseNumber and title must exist
        if (parts.size() < 2 || parts[0].empty() || parts[1].empty()) {
            cout << "Error: Invalid line format at line " << lineNumber
                 << ". Each line must have at least CourseNumber and Title." << endl;
            return false;
        }

        Course c;
        c.number = toUpper(parts[0]);
        c.title  = parts[1];

        // Remaining tokens are prerequisites (0..N)
        for (size_t i = 2; i < parts.size(); i++) {
            if (!parts[i].empty()) {
                string prereq = toUpper(parts[i]);
                c.prerequisites.push_back(prereq);
                prereqEdges.push_back({c.number, prereq});
            }
        }

        // Insert into hash table (if duplicates appear, last one wins)
        if (courses.find(c.number) != courses.end()) {
            cout << "Error: Duplicate course number '" << c.number
                 << "' found at line " << lineNumber << "." << endl;
            return false;
        }
        courses[c.number] = c;

    } // end while (getline)

       file.close();

       // Validate: any prerequisite must exist as a course in the file
       for (const auto& edge : prereqEdges) {
           const string& courseNum = edge.first;
           const string& prereqNum = edge.second;

           if (courses.find(prereqNum) == courses.end()) {
               cout << "Error: Course '" << courseNum << "' lists prerequisite '"
                    << prereqNum << "' but '" << prereqNum << "' is not found in the file." << endl;
               return false;
           }
       }

       cout << "Successfully loaded " << courses.size() << " courses." << endl;
       return true;
   } // end loadCoursesFromFile
// Print list of courses in alphanumeric order (course number only, or number + title?both are acceptable;
// including title is usually better for advisors).
static void printCourseList(const unordered_map<string, Course>& courses) {
    if (courses.empty()) {
        cout << "No courses loaded. Please choose option 1 to load data first." << endl;
        return;
    }

    vector<string> keys;
    keys.reserve(courses.size());
    for (const auto& kv : courses) keys.push_back(kv.first);

    sort(keys.begin(), keys.end());

    cout << "\nCourse List (Alphanumeric Order)\n";
    cout << "--------------------------------\n";
    for (const string& courseNum : keys) {
        const Course& c = courses.at(courseNum);
        cout << c.number << ", " << c.title << endl;
    }
    cout << endl;
}

// Print one course with prerequisites (numbers + titles)
static void printCourseInfo(const unordered_map<string, Course>& courses, const string& userCourseNumber) {
    if (courses.empty()) {
        cout << "No courses loaded. Please choose option 1 to load data first." << endl;
        return;
    }

    string courseNumber = toUpper(trim(userCourseNumber));
    auto it = courses.find(courseNumber);

    if (it == courses.end()) {
        cout << "Course '" << courseNumber << "' not found." << endl;
        return;
    }

    const Course& c = it->second;
    cout << "\n" << c.number << ", " << c.title << endl;

    if (c.prerequisites.empty()) {
        cout << "Prerequisites: None\n" << endl;
        return;
    }

    cout << "Prerequisites:\n";
    for (const string& prereqNum : c.prerequisites) {
        auto pit = courses.find(prereqNum);
        // Should exist due to validation, but keep it safe:
        if (pit != courses.end()) {
            cout << "  - " << pit->second.number << ", " << pit->second.title << endl;
        } else {
            cout << "  - " << prereqNum << " (title unavailable)\n";
        }
    }
    cout << endl;
}

static void printMenu() {
    cout << "Menu:\n";
    cout << "  1. Load Data Structure\n";
    cout << "  2. Print Course List\n";
    cout << "  3. Print Course\n";
    cout << "  9. Exit\n";
    cout << "Enter choice: ";
}

int main() {
    unordered_map<string, Course> courses; // Hash table (preferred)
    string fileName;

    int choice = 0;
    while (choice != 9) {
        printMenu();

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Error: Please enter a valid menu number.\n" << endl;
            continue;
        }

        switch (choice) {
            case 1: {
                cout << "Enter the file name: ";
                cin >> fileName;

                bool ok = loadCoursesFromFile(fileName, courses);

                if (ok) {
                    cout << "Courses loaded successfully.\n" << endl;
                } else {
                    cout << "Error: Could not load file. Please try again.\n" << endl;
                }
                break;
            }

            case 2:
                if (courses.empty()) {
                    cout << "Error: Please load the data file first (Option 1).\n" << endl;
                    break;
                }
                printCourseList(courses);
                break;

            case 3: {
                if (courses.empty()) {
                    cout << "Error: Please load the data file first (Option 1).\n" << endl;
                    break;
                }

                string courseNumber;
                cout << "Enter course number: ";
                cin >> courseNumber;

                printCourseInfo(courses, courseNumber);
                break;
            }

            case 9:
                cout << "Goodbye.\n";
                break;

            default:
                cout << "Error: Invalid choice. Please select 1, 2, 3, or 9.\n" << endl;
                break;
        }
    }

    return 0;
}



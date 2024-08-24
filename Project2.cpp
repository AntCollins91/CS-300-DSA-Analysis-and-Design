#include <iostream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <climits>
#include <string>

// Structure to hold course information
struct Course {
    std::string courseNumber;   // unique identifier
    std::string courseTitle;
    std::vector<std::string> prerequisites;

    Course() = default;

    Course(std::string number, std::string title, std::vector<std::string> prereqs) {
        courseNumber = number;
        courseTitle = title;
        prerequisites = prereqs;
    }
};

// Hash Table class definition
class HashTable {
public:
    struct Node {
        Course course;
        unsigned int key;
        Node* next;

        Node() {
            key = UINT_MAX;
            next = nullptr;
        }

        Node(Course aCourse) : Node() {
            course = aCourse;
        }

        Node(Course aCourse, unsigned int aKey) : Node(aCourse) {
            key = aKey;
        }
    };

    std::vector<Node> nodes;
    unsigned int tableSize;

    HashTable(unsigned int size = 179) {
        this->tableSize = size;
        nodes.resize(size);
    }

    ~HashTable() {
        for (unsigned int i = 0; i < tableSize; ++i) {
            Node* node = &nodes[i];
            while (node != nullptr) {
                Node* temp = node;
                node = node->next;
                delete temp;
            }
        }
    }

    unsigned int hash(int key) {
        return key % tableSize;
    }

    void Insert(Course course) {
        unsigned int key = hash(std::stoi(course.courseNumber.substr(4)));  // Use the last 3 digits of course number as key
        Node* prev = nullptr;
        Node* node = &nodes[key];

        while (node != nullptr && node->key != UINT_MAX) {
            prev = node;
            node = node->next;
        }

        if (node == nullptr) {
            node = new Node(course, key);
            if (prev != nullptr) {
                prev->next = node;
            }
            else {
                nodes[key] = *node;
            }
        }
        else {
            node->course = course;
            node->key = key;
        }
    }

    Course Search(std::string courseNumber) {
        unsigned int key = hash(std::stoi(courseNumber.substr(4)));
        Node* node = &nodes[key];

        while (node != nullptr && node->key != UINT_MAX && node->course.courseNumber != courseNumber) {
            node = node->next;
        }

        if (node != nullptr && node->key != UINT_MAX) {
            return node->course;
        }
        else {
            return Course(); // Return an empty Course object if not found
        }
    }

    void PrintAll() {
        std::cout << "Printing all courses:" << std::endl;
        for (unsigned int i = 0; i < tableSize; ++i) {
            Node* node = &nodes[i];
            while (node != nullptr && node->key != UINT_MAX) {
                std::cout << node->course.courseNumber << ", " << node->course.courseTitle << std::endl;
                node = node->next;
            }
        }
    }
};

// Function prototypes for menu and data handling
void loadData(HashTable& courseTable);
void printCourseList(HashTable& courseTable);
void searchCourse(HashTable& courseTable);
void displayMenu();

int main() {
    HashTable courseTable;

    // Automatically load data at the start of the program
    loadData(courseTable);

    int choice = 0;
    while (choice != 9) {
        displayMenu();
        std::cin >> choice;
        switch (choice) {
        case 1:
            std::cout << "Data has already been loaded automatically." << std::endl;
            break;
        case 2:
            printCourseList(courseTable);
            break;
        case 3:
            searchCourse(courseTable);
            break;
        case 9:
            std::cout << "Thank you for using the course planner!" << std::endl;
            break;
        default:
            std::cout << choice << " is not a valid option." << std::endl;
            break;
        }
    }
    return 0;
}

void loadData(HashTable& courseTable) {
    std::vector<std::string> courseData = {
        "CSCI100,Introduction to Computer Science",
        "CSCI101,Introduction to Programming in C++,CSCI100",
        "CSCI200,Data Structures,CSCI101",
        "MATH201,Discrete Mathematics",
        "CSCI300,Introduction to Algorithms,CSCI200,MATH201",
        "CSCI301,Advanced Programming in C++,CSCI101",
        "CSCI350,Operating Systems,CSCI300",
        "CSCI400,Large Software Development,CSCI301,CSCI350"
    };

    for (const auto& line : courseData) {
        std::istringstream stream(line);
        std::string courseNumber, courseTitle, prereq;
        std::getline(stream, courseNumber, ',');
        std::getline(stream, courseTitle, ',');

        std::vector<std::string> prerequisites;
        while (std::getline(stream, prereq, ',')) {
            prerequisites.push_back(prereq);
        }

        Course course(courseNumber, courseTitle, prerequisites);
        courseTable.Insert(course);
    }

    std::cout << "Data loaded successfully." << std::endl;
}

void printCourseList(HashTable& courseTable) {
    std::vector<Course> courseList;

    for (unsigned int i = 0; i < courseTable.tableSize; ++i) {
        HashTable::Node* node = &courseTable.nodes[i];
        while (node != nullptr && node->key != UINT_MAX) {
            courseList.push_back(node->course);
            node = node->next;
        }
    }

    std::sort(courseList.begin(), courseList.end(), [](const Course& a, const Course& b) {
        return a.courseNumber < b.courseNumber;
        });

    std::cout << "Here is a sample schedule:" << std::endl;
    for (const auto& course : courseList) {
        std::cout << course.courseNumber << ", " << course.courseTitle << std::endl;
    }
}

void searchCourse(HashTable& courseTable) {
    std::string courseNumber;
    std::cout << "What course do you want to know about? ";
    std::cin >> courseNumber;

    std::transform(courseNumber.begin(), courseNumber.end(), courseNumber.begin(), ::toupper);

    Course course = courseTable.Search(courseNumber);
    if (!course.courseNumber.empty()) {
        std::cout << course.courseNumber << ", " << course.courseTitle << std::endl;
        if (!course.prerequisites.empty()) {
            std::cout << "Prerequisites: ";
            for (size_t i = 0; i < course.prerequisites.size(); ++i) {
                std::cout << course.prerequisites[i];
                if (i < course.prerequisites.size() - 1) {
                    std::cout << ", ";
                }
            }
            std::cout << std::endl;
        }
        else {
            std::cout << "Prerequisites: None" << std::endl;
        }
    }
    else {
        std::cout << "Course not found." << std::endl;
    }
}

void displayMenu() {
    std::cout << "\n1. Load Data Structure." << std::endl;
    std::cout << "2. Print Course List." << std::endl;
    std::cout << "3. Print Course." << std::endl;
    std::cout << "9. Exit" << std::endl;
    std::cout << "\nWhat would you like to do? ";
}

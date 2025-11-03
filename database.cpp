#include "database.hpp"
#include <iostream>
#include <fstream>
#include <algorithm>


Student::Student(const std::string &line) {
    std::vector<std::string> string_splitted;
    std::stringstream str_stream(line);
    std::string word;
    while (getline(str_stream, word, ',')) {
        string_splitted.push_back(word);
    }
    if (string_splitted.size() != 9) {
        throw std::invalid_argument("Invalid input line: " + line);
    }
    this->m_name = string_splitted[0];
    this->m_surname = string_splitted[1];
    this->m_email = string_splitted[2];
    this->m_birth_year = std::stoi(string_splitted[3]);
    this->m_birth_month = std::stoi(string_splitted[4]);
    this->m_birth_day = std::stoi(string_splitted[5]);
    this->m_group = string_splitted[6];
    this->m_rating = std::stof(string_splitted[7]);
    this->m_phone_number = string_splitted[8];
}

bool Student::operator<(const Student& other) const {
    if (m_group != other.m_group)
        return m_group < other.m_group;
    return m_email < other.m_email;
}

void Student::toString() const {
    std::cout << this->m_name << std::endl;
    std::cout << this->m_surname << std::endl;
    std::cout << this->m_email << std::endl;
    std::cout << this->m_birth_year << std::endl;
    std::cout << this->m_birth_month << std::endl;
    std::cout << this->m_birth_day << std::endl;
    std::cout << this->m_group << std::endl;
    std::cout << this->m_rating << std::endl;
    std::cout << this->m_phone_number << std::endl;
}


Database::Database(const std::string &filename) {
    std::fstream inputFile(filename);
    if (!inputFile.is_open()) {
        throw std::runtime_error("Cannot open input file: " + filename);
    }
    std::string line;
    std::getline(inputFile, line);
    while (std::getline(inputFile, line)) {
        if (line.empty()) continue;
        students.emplace_back(line);

        emails.push_back(students.back().m_email);
    }
}

SolutionMap::SolutionMap(Database &database) {
    for (const auto &student : database.students) {
        students.emplace(student.m_email, student);
        groupCounts[student.m_group]++;
        groupRatingsSum[student.m_group] += student.m_rating;
    }
}

std::string SolutionMap::maxGroup() {
    if (groupCounts.empty()) return {};

    int bestCount = -1;
    std::string bestGroup;


    for (const auto &p : groupCounts) {
        const std::string &group = p.first;
        int cnt = p.second;
        if (cnt > bestCount || (cnt == bestCount && group < bestGroup)) {
            bestCount = cnt;
            bestGroup = group;
        }
    }
    return bestGroup;
}

std::string SolutionMap::maxGradeGroup() {
    if (groupCounts.empty()) return {};

    double bestAvg = -1.0;
    std::string bestGroup;

    for (const auto &p : groupCounts) {
        const std::string &group = p.first;
        int cnt = p.second;

        if (cnt == 0) continue;
        double sum = groupRatingsSum[group];
        double avg = sum / cnt;

        if (avg > bestAvg || (avg == bestAvg && group < bestGroup)) {
            bestAvg = avg;
            bestGroup = group;
        }
    }
    return bestGroup;
}

void SolutionMap::changeGroupByEmail(const std::string &email, const std::string &newGroup) {
    auto it = students.find(email);
    if (it == students.end()) {
        return;
    }

    Student& student = it->second;

    std::string oldGroup = student.m_group;

    if (oldGroup == newGroup) {
        return;
    }

    float rating = student.m_rating;

    groupCounts[oldGroup]--;
    groupRatingsSum[oldGroup] -= rating;

    groupCounts[newGroup]++;
    groupRatingsSum[newGroup] += rating;

    student.m_group = newGroup;
}

SolutionVector::SolutionVector(Database &database) {
    this->students = database.students;
}

std::string SolutionVector::maxGroup() {
    if (students.empty()) return {};
    std::unordered_map<std::string, int> groups;
    for (const auto &s : students) {
        groups[s.m_group]++;
    }
    int bestCount = -1;
    std::string bestGroup;
    for (const auto &p : groups) {
        const std::string &group = p.first;
        int cnt = p.second;
        if (cnt > bestCount || (cnt == bestCount && group < bestGroup)) {
            bestCount = cnt;
            bestGroup = group;
        }
    }
    return bestGroup;
}

void SolutionVector::changeGroupByEmail(const std::string &email, const std::string &group) {
    for (auto &student : students) {
        if (student.m_email == email) {
            student.m_group = group;
            return;
        }
    }
    throw std::invalid_argument("No student with such email");
}

std::string SolutionVector::maxGradeGroup() {
    if (students.empty()) return {};
    std::unordered_map<std::string, std::pair<double,int>> groups;
    for (const auto &s : students) {
        groups[s.m_group].first += s.m_rating;
        groups[s.m_group].second += 1;
    }
    double bestAvg = -1.0;
    std::string bestGroup;
    for (const auto &p : groups) {
        const std::string &group = p.first;
        double sum = p.second.first;
        int cnt = p.second.second;
        if (cnt == 0) continue;
        double avg = sum / cnt;
        if (avg > bestAvg || (avg == bestAvg && group < bestGroup)) {
            bestAvg = avg;
            bestGroup = group;
        }
    }
    return bestGroup;
}


SolutionSet::SolutionSet(Database &database) {
    students = std::set<Student>(database.students.begin(), database.students.end());
}

std::string SolutionSet::maxGroup() {
    std::string bestGroup, currentGroup;
    int bestCount = 0, currentCount = 0;
    if (students.empty()) return {};
    for (auto it = students.begin(); it != students.end(); ++it) {
        if (it == students.begin() || it->m_group != currentGroup) {
            if (currentCount > 0 &&
                (currentCount > bestCount ||
                 (currentCount == bestCount && currentGroup < bestGroup))) {
                bestCount = currentCount;
                bestGroup = currentGroup;
            }
            currentGroup = it->m_group;
            currentCount = 0;
        }
        currentCount++;
    }
    if (currentCount > 0 &&
        (currentCount > bestCount ||
         (currentCount == bestCount && currentGroup < bestGroup))) {
        bestCount = currentCount;
        bestGroup = currentGroup;
    }
    return bestGroup;
}

void SolutionSet::changeGroupByEmail(const std::string &email, const std::string &newGroup) {
    auto it = std::find_if(students.begin(), students.end(),
                          [&](const Student &s){ return s.m_email == email; });
    if (it == students.end()) return;
    Student s = *it;
    students.erase(it);
    s.m_group = newGroup;
    students.insert(s);
}

std::string SolutionSet::maxGradeGroup() {
    std::string bestGroup, currentGroup;
    double bestAvg = -1, sum = 0;
    int count = 0;
    if (students.empty()) return {};
    for (auto it = students.begin(); it != students.end(); ++it) {
        if (it == students.begin() || it->m_group != currentGroup) {
            if (count > 0) {
                double avg = sum / count;
                if (avg > bestAvg ||
                    (avg == bestAvg && currentGroup < bestGroup)) {
                    bestAvg = avg;
                    bestGroup = currentGroup;
                }
            }
            currentGroup = it->m_group;
            sum = 0;
            count = 0;
        }
        sum += it->m_rating;
        count++;
    }
    if (count > 0) {
        double avg = sum / count;
        if (avg > bestAvg || (avg == bestAvg && currentGroup < bestGroup)) {
            bestAvg = avg;
            bestGroup = currentGroup;
        }
    }
    return bestGroup;
}


BSTNode::BSTNode(const Student &s) : data(s), left(nullptr), right(nullptr) {}

void Task3::sort_built_in(Database &database) {
    std::sort(database.students.begin(), database.students.end(),
              [](const Student &a, const Student &b) {
                  if (a.m_surname == b.m_surname)
                      return a.m_name < b.m_name;
                  return a.m_surname < b.m_surname;
              });
}

void Task3::insertBST(BSTNode *&root, const Student &s) {
    if (!root) {
        root = new BSTNode(s);
        return;
    }
    if (s.m_surname < root->data.m_surname ||
       (s.m_surname == root->data.m_surname && s.m_name < root->data.m_name)) {
        insertBST(root->left, s);
    } else {
        insertBST(root->right, s);
    }
}

void Task3::inorderTraversal(BSTNode *root, std::vector<Student> &sorted) {
    if (!root) return;
    inorderTraversal(root->left, sorted);
    sorted.push_back(root->data);
    inorderTraversal(root->right, sorted);
}

void Task3::deleteBST(BSTNode *root) {
    if (!root) return;
    deleteBST(root->left);
    deleteBST(root->right);
    delete root;
}

void Task3::bstSort(Database &database) {
    BSTNode *root = nullptr;
    for (const auto &s : database.students)
        insertBST(root, s);

    std::vector<Student> sorted;
    inorderTraversal(root, sorted);
    database.students = sorted;

    deleteBST(root);
}

void Task3::saveCSV(Database &database, const std::string &outputFile) {
    std::ofstream out(outputFile);
    out << "name,surname,email,birth_year,birth_month,birth_day,group,rating,phone_number\n";
    for (const auto &s : database.students) {
        out << s.m_name << ","
            << s.m_surname << ","
            << s.m_email << ","
            << s.m_birth_year << ","
            << s.m_birth_month << ","
            << s.m_birth_day << ","
            << s.m_group << ","
            << s.m_rating << ","
            << s.m_phone_number << "\n";
    }
}

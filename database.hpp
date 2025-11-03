#ifndef HOMEWORK1_DATABASE_HPP
#define HOMEWORK1_DATABASE_HPP

#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <unordered_map>
#include <set>
struct Student {
    std::string m_name;
    std::string m_surname;
    std::string m_email;
    int m_birth_year;
    int m_birth_month;
    int m_birth_day;
    std::string m_group;
    float m_rating;
    std::string m_phone_number;

    Student(const std::string &line);

    bool operator<(const Student& other) const;
    void toString() const;
    };

struct Database {
    std::vector<Student> students;
    std::vector<std::string> emails;

    explicit Database(const std::string &filename = "../students.csv");
};

class SolutionMap {
    std::unordered_map<std::string, Student> students;
    std::unordered_map<std::string, int> groupCounts;
    std::unordered_map<std::string, double> groupRatingsSum;

public:
    explicit SolutionMap(Database &database);

    std::string maxGroup();

    std::string maxGradeGroup();

    void changeGroupByEmail(const std::string &email, const std::string &newGroup);
};
class SolutionVector {
    std::vector<Student> students;
public:
    explicit SolutionVector(Database &database);
    std::string maxGroup();
    void changeGroupByEmail(const std::string &email, const std::string &group);
    std::string maxGradeGroup();
};

class SolutionSet {
    std::set<Student> students;

public:
    explicit SolutionSet(Database &database);
    std::string maxGroup();
    void changeGroupByEmail(const std::string &email, const std::string &newGroup);
    std::string maxGradeGroup();
};


struct BSTNode {
    Student data;
    BSTNode *left;
    BSTNode *right;

    explicit BSTNode(const Student &s);
};

class Task3 {
public:
    static void sort_built_in(Database &database);
    static void insertBST(BSTNode *&root, const Student &s);
    static void inorderTraversal(BSTNode *root, std::vector<Student> &sorted);
    static void deleteBST(BSTNode *root);
    static void bstSort(Database &database);
    static void saveCSV(Database &database, const std::string &outputFile = "output.csv");
};
#endif //HOMEWORK1_DATABASE_HPP
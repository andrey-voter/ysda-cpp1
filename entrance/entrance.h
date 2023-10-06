#pragma once

#include <stdexcept>
#include <map>
#include <string>
#include <vector>
#include <tuple>
#include <algorithm>

struct StudentName {
    std::string name, surname;
};

struct Date {
    int year, month, day;
};

struct Student {
    StudentName full_name;
    Date date_of_birth;
    int score;
    std::vector<std::string> universities;
};

bool CompareStudents(const Student& st1, const Student& st2) {
    if (st1.score == st2.score) {
        if (st1.date_of_birth.year == st2.date_of_birth.year) {
            if (st1.date_of_birth.month == st2.date_of_birth.month) {
                if (st1.date_of_birth.day == st2.date_of_birth.day) {
                    if (st1.full_name.surname == st2.full_name.surname) {
                        return st1.full_name.name < st2.full_name.surname;
                    } else {
                        return st1.full_name.surname < st2.full_name.surname;
                    }
                } else {
                    return st1.date_of_birth.day < st2.date_of_birth.day;
                }
            } else {
                return st1.date_of_birth.month < st2.date_of_birth.month;
            }
        } else {
            return st1.date_of_birth.year < st2.date_of_birth.year;
        }
    } else {
        return st1.score > st2.score;
    }
}

bool CompareNames(const StudentName& name1, const StudentName& name2) {
    if (name1.surname == name2.surname) {
        return name1.name < name2.name;
    } else {
        return name1.surname < name2.surname;
    }
}

std::map<std::string, std::vector<StudentName>> GetStudents(
    const std::vector<std::pair<std::string, int>>& universities_info,
    const std::vector<std::tuple<StudentName, Date, int, std::vector<std::string>>>&
        students_info) {

    std::vector<Student> students;
    for (const auto& info : students_info) {
        const auto& [full_name, date_of_birth, score, universities] = info;
        Student student{full_name, date_of_birth, score, universities};
        students.push_back(student);
    }

    std::map<std::string, int> universities;
    for (const auto& pair : universities_info) {
        universities[pair.first] = pair.second;
    }

    std::sort(students.begin(), students.end(), CompareStudents);

    std::map<std::string, std::vector<StudentName>> ans;

    for (const auto& student : students) {
        auto& cur_unis = student.universities;
        for (auto& uni : cur_unis) {
            if (universities[uni] > 0) {
                universities[uni]--;
                ans[uni].push_back(student.full_name);
                break;
            }
        }
    }

    for (auto& pair : ans) {
        std::vector<StudentName>& cur_uni_students = pair.second;
        std::sort(cur_uni_students.begin(), cur_uni_students.end(), CompareNames);
    }
    return ans;
}

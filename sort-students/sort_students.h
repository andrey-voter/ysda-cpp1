#pragma once

#include <vector>
#include <string>
#include <stdexcept>

struct Student {
    std::string name, surname;
    int day, month, year;
};

enum class SortType { kByName, kByDate };

bool SortByName(const Student &student1, const Student &student2) {
    if (student1.surname == student2.surname) {
        if (student1.name == student2.name) {
            if (student1.year == student2.year) {
                if (student1.month == student2.month) {
                    return student1.day < student2.day;
                } else {
                    return student1.month < student2.month;
                }
            } else {
                return student1.year < student2.year;
            }
        } else {
            return student1.name < student2.name;
        }
    } else {
        return student1.surname < student2.surname;
    }
}

bool SortByDate(const Student &student1, const Student &student2) {
    if (student1.year == student2.year) {
        if (student1.month == student2.month) {
            if (student1.day == student2.day) {
                if (student1.surname == student2.surname) {
                    return student1.name < student2.name;
                } else {
                    return student1.surname < student2.surname;
                }
            } else {
                return student1.day < student2.day;
            }
        } else {
            return student1.month < student2.month;
        }
    } else {
        return student1.year < student2.year;
    }
}

void SortStudents(std::vector<Student> *students, SortType sort_type) {
    if (sort_type == SortType::kByName) {
        std::sort(students->begin(), students->end(), SortByName);
    } else {
        std::sort(students->begin(), students->end(), SortByDate);
    }
}

#pragma once

#include <string>
#include <stdexcept>
#include <vector>
#include <stack>
#include <set>

std::vector<std::string> Split(const std::string& string, const std::string& delimiter = " ") {
    if (string.empty()) {
        return std::vector<std::string>();
    }
    std::vector<std::string> res;
    auto delimiter_pos = string.find(delimiter);
    size_t prev_pos = 0;
    while (std::string::npos != delimiter_pos) {
        if (string.substr(prev_pos, delimiter.size()) == delimiter) {
            res.push_back("");
            prev_pos = delimiter_pos + delimiter.size();
            delimiter_pos = string.find(delimiter, delimiter_pos + delimiter.size());
            continue;
        }
        res.push_back(string.substr(prev_pos, delimiter_pos - prev_pos));
        prev_pos = delimiter_pos + delimiter.size();
        delimiter_pos = string.find(delimiter, delimiter_pos + delimiter.size());
    }
    if (prev_pos < string.size()) {
        res.push_back(string.substr(prev_pos));
    } else {
        if (prev_pos == string.size()) {
            res.push_back("");
        }
    }
    return res;
}

int EvaluateExpression(const std::string& expression) {
    auto tokens = Split(expression);
    std::stack<std::string> stack;
    std::set<std::string> ops{"+", "-", "*"};
    for (std::string str : tokens) {
        if (ops.count(str)) {
            int op1 = std::stoi(stack.top());
            stack.pop();
            int op2 = std::stoi(stack.top());
            stack.pop();
            std::string res;
            if (str == "+") {
                res = std::to_string(op2 + op1);
            } else {
                if (str == "*") {
                    res = std::to_string(op2 * op1);
                } else {
                    res = std::to_string(op2 - op1);
                }
            }
            stack.push(res);
        } else {
            stack.push(str);
        }
    }
    return std::stoi(stack.top());
}

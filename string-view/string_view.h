#pragma once

#include <string>
#include <cstring>

class StringView {
public:
    StringView(const std::string& string, size_t pos = 0, size_t len = std::string::npos) {
        data_ = string.c_str() + pos;
        size_ = std::min(len, string.length() - pos);
    }

    StringView(const char* c_str) {
        data_ = c_str;
        size_ = strlen(c_str);
    }

    StringView(const char* c_str, size_t len) {
        data_ = c_str;
        size_ = len;
    }

    char operator[](int i) const {
        return *(data_ + i * sizeof(char));
    }

    const char* Data() const {
        return data_;
    }

    size_t Size() const {
        return size_;
    }

private:
    const char* data_;
    size_t size_;
};

#pragma once

#include <cstddef>
#include <string>
#include <vector>
#include <string_view>
#include <iostream>
#include <deque>

class Editor {
public:
    class TextView {
    public:
        TextView(const std::string& str, size_t pos, size_t n)
            : str_(str.substr(pos, n)) {}

        auto begin() const {
            return str_.begin();
        }

        auto end() const {
            return str_.end();
        }

    private:
        std::string str_;
    };


    void Type(char symbol, bool new_action = true) {
        before_cursor_.push_back(symbol);
        ++cursor_position_;
        std::string func_name = __func__;
        func_name += symbol;
        done_commands_.push_back(func_name);
        if (new_action) {
            cancelled_commands_.clear();
        }
    }

    void ShiftLeft(bool new_action = true) {
        if (cursor_position_) {
            --cursor_position_;
            char c = before_cursor_.back();
            before_cursor_.pop_back();
            after_cursor_.push_back(c);
            after_cursor_helper_.push_front(c);
            done_commands_.push_back(__func__);
            if (new_action) {
                cancelled_commands_.clear();
            }
        }
    }

    void ShiftRight(bool new_action = true) {
        if (cursor_position_ < Size()) {
            ++cursor_position_;
            char c = after_cursor_.back();
            after_cursor_.pop_back();
            after_cursor_helper_.pop_front();
            before_cursor_.push_back(c);
            done_commands_.push_back(__func__);
            if (new_action) {
                cancelled_commands_.clear();
            }
        }
    }

    void Backspace(bool new_action = true) {
        if (!before_cursor_.empty()) {
            --cursor_position_;
            char c = before_cursor_.back();
            before_cursor_.pop_back();
            std::string func_name = __func__;
            func_name += c;
            done_commands_.push_back(func_name);
            if (new_action) {
                cancelled_commands_.clear();
            }
        }
    }

    void Undo() {
        if (!done_commands_.empty()) {
            std::string last_command = done_commands_.back();
            done_commands_.pop_back();

            if (last_command == "ShiftLeft") {
                ++cursor_position_;
                char c = after_cursor_.back();
                after_cursor_.pop_back();
                after_cursor_helper_.pop_front();
                before_cursor_.push_back(c);
            } else if (last_command == "ShiftRight") {
                --cursor_position_;
                char c = before_cursor_.back();
                before_cursor_.pop_back();
                after_cursor_.push_back(c);
                after_cursor_helper_.push_front(c);
            } else if (last_command.find("Type") != std::string::npos) {
                --cursor_position_;
                before_cursor_.pop_back();
            } else {
                ++cursor_position_;
                before_cursor_.push_back(last_command.back());
            }
            cancelled_commands_.push_back(last_command);
        }
    }

    void Redo() {
        if (!cancelled_commands_.empty()) {
            std::string cancelled_command = cancelled_commands_.back();
            cancelled_commands_.pop_back();

            if (cancelled_command == "ShiftLeft") {
                ShiftLeft(false);
            } else if (cancelled_command == "ShiftRight") {
                ShiftRight(false);
            } else if (cancelled_command.find("Type") != std::string::npos) {
                Type(cancelled_command.back(), false);
            } else {
                Backspace(false);
            }
        }
    }

    size_t Size() const {
        return before_cursor_.size() + after_cursor_.size();
    }

    TextView GetText(size_t pos, size_t count) const {
        std::string str(after_cursor_helper_.begin(), after_cursor_helper_.end());
        return {before_cursor_ + str, pos, count};
    }

    size_t GetPosition() const {
        return cursor_position_;
    }

private:

    std::string before_cursor_;
    std::string after_cursor_;
    std::deque<char> after_cursor_helper_;
    size_t cursor_position_ = 0;

    std::vector<std::string> done_commands_;
    std::vector<std::string> cancelled_commands_;
};

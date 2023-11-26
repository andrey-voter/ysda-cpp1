#pragma once

#include <cstddef>
#include <string>
#include <vector>
#include <string_view>
#include <deque>

class Editor {
public:
    class TextView {
    public:
        class Iterator {
        public:
            Iterator(std::string_view before, std::string_view after, size_t pos)
                : before_(before), after_(after), cur_pos_(pos) {
            }

            Iterator& operator++() {
                ++cur_pos_;
                return *this;
            }

            char operator*() const {
                if (cur_pos_ < before_.size()) {
                    return before_[cur_pos_];
                }
                return after_[after_.size() + before_.size() - cur_pos_ - 1];
            }

            bool operator==(Iterator rhs) {
                return cur_pos_ == rhs.cur_pos_;
            }

            bool operator!=(Iterator rhs) {
                return cur_pos_ != rhs.cur_pos_;
            }

        private:
            std::string_view before_;
            std::string_view after_;
            size_t cur_pos_;
        };

        TextView(const std::string_view before, const std::string_view after, size_t pos,
                 size_t count)
            : before_(before), after_(after), pos_(pos), count_(count) {
        }

        Iterator begin() {
            return Iterator(before_, after_, pos_);
        }

        Iterator end() {
            return Iterator(before_, after_, pos_ + count_);
        }

    private:
        std::string_view before_;
        std::string_view after_;
        size_t pos_;
        size_t count_;
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
            cancelled_commands_.push_back(last_command);

            if (last_command == "ShiftLeft") {
                ++cursor_position_;
                char c = after_cursor_.back();
                after_cursor_.pop_back();
                before_cursor_.push_back(c);
            } else if (last_command == "ShiftRight") {
                --cursor_position_;
                char c = before_cursor_.back();
                before_cursor_.pop_back();
                after_cursor_.push_back(c);
            } else if (last_command.find("Type") != std::string::npos) {
                --cursor_position_;
                before_cursor_.pop_back();
            } else {
                ++cursor_position_;
                before_cursor_.push_back(last_command.back());
            }
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
        std::string_view first = before_cursor_;
        std::string_view second = after_cursor_;
        return {first, second, pos, count};
    }

    size_t GetPosition() const {
        return cursor_position_;
    }

private:
    std::string before_cursor_;
    std::string after_cursor_;
    size_t cursor_position_ = 0;

    std::vector<std::string> done_commands_;
    std::vector<std::string> cancelled_commands_;
};

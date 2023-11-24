#pragma once

#include <cstddef>
#include <string>
#include <vector>
#include <string_view>

class Editor {
public:
    class TextView {
    public:
        TextView(const std::string& str, size_t pos, size_t n, bool is_reversed = false) :
              str_(str),
              pos_(pos),
              size_(std::min(n, str_.size() - pos)),
              isReversed_(is_reversed) {}

        // Итераторы для поддержки range-based for
        auto begin() const {
            return isReversed_ ? std::make_reverse_iterator(str_.begin() + pos_ + size_) : str_.begin() + pos_;
        }

        auto end() const {
            return isReversed_ ? std::make_reverse_iterator(str_.begin() + pos_) : str_.begin() + pos_ + size_;
        }

    private:
        const std::string& str_;
        size_t pos_;
        size_t size_;
        bool isReversed_;
    };
    void Type(char symbol) {
        before_cursor_.push_back(symbol);
        ++cursor_position_;
        done_commands_.push_back(__func__ + symbol);
        cancelled_commands_.clear();
    }

    void ShiftLeft() {
        if (cursor_position_) {
            --cursor_position_;
            char c = before_cursor_.back();
            before_cursor_.pop_back();
            after_cursor_.push_back(c);
            done_commands_.push_back(__func__);
            cancelled_commands_.clear();
        }
    }

    void ShiftRight() {
        if (cursor_position_ < Size()) {
            ++cursor_position_;
            char c = after_cursor_.back();
            after_cursor_.pop_back();
            before_cursor_.push_back(c);
            done_commands_.push_back(__func__);
            cancelled_commands_.clear();
        }
    }

    void Backspace() {
        if (!before_cursor_.empty()) {
            --cursor_position_;
            char c = before_cursor_.back();
            before_cursor_.pop_back();
            done_commands_.push_back(__func__ + c);
            cancelled_commands_.clear();
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
            cancelled_commands_.push_back(last_command);
        }
    }

    void Redo() {
        if (!cancelled_commands_.empty()) {
            std::string cancelled_command = cancelled_commands_.back();
            cancelled_commands_.pop_back();

            if (cancelled_command == "ShiftLeft") {
                ShiftLeft();
            } else if (cancelled_command == "ShiftRight") {
                ShiftRight();
            } else if (cancelled_command.find("Type") != std::string::npos) {
                Type(cancelled_command.back());
            } else {
                Backspace();
            }
        }
    }

    size_t Size() const {
        return before_cursor_.size() + after_cursor_.size();
    }

    TextView GetText(size_t pos, size_t count) const {
        // only before cursor
        if (pos < cursor_position_ and (count + pos < before_cursor_.size())) {
            return {before_cursor_, pos, count};
        }
        // only after cursor
        if (pos > cursor_position_ and (pos - cursor_position_ + count < after_cursor_.size())) {
            return {after_cursor_, pos, count, true};
        }
        // view includes both
        return {before_cursor_, pos - cursor_position_, count};
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

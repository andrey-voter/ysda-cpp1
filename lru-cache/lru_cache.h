#pragma once

#include <list>
#include <string>
#include <unordered_map>

class LruCache {
public:
    explicit LruCache(size_t max_size) {
        max_size_ = max_size;
    }

    LruCache(const LruCache&) = delete;
    LruCache& operator=(const LruCache&) = delete;

    void Set(const std::string& key, const std::string& value) {
        if (size_ < max_size_) {
            list_.push_back({key, value});
            map_[key] = --list_.end();
            ++size_;
        } else {
            map_.erase(list_.front().first);
            list_.pop_front();
            list_.push_back({key, value});
            map_[key] = --list_.end();
        }
    }

    bool Get(const std::string& key, std::string* value) {
        auto it = map_.find(key);
        if (it == map_.end()) {
            return false;
        }
        *value = (map_[key])->second;
        auto tmp_pair = *map_[key];
        auto to_delete = map_[key];
        list_.erase(to_delete);
        list_.push_back(tmp_pair);
        map_[key] = --list_.end();
        return true;
    }

private:
    std::unordered_map<std::string, std::list<std::pair<std::string, std::string>>::iterator> map_;
    std::list<std::pair<std::string, std::string>> list_;
    size_t size_ = 0;
    size_t max_size_;
};

#pragma once

#include "dungeon.h"

#include <stdexcept>
#include <unordered_set>
#include <queue>

void GetKeys(const Room* room, std::vector<std::string>& my_keys) {
    for (size_t i = 0; i < room->NumKeys(); ++i) {
        my_keys.push_back(room->GetKey(i));
    }
}

const Room* FindFinalRoom(const Room* starting_room) {
    if (starting_room->IsFinal()) {
        return starting_room;
    }

    std::queue<const Room*> queue;
    std::unordered_set<const Room*> visited;

    queue.push(starting_room);
    visited.insert(starting_room);

    std::vector<std::string> my_keys;

    while (!queue.empty()) {
        const Room* current_room = queue.front();
        queue.pop();

        if (current_room->IsFinal()) {
            return current_room;
        }

        GetKeys(current_room, my_keys);

        for (auto& prev_room : visited) {
            for (size_t i = 0; i < prev_room->NumDoors(); ++i) {
                bool flag = false;
                Door* door = prev_room->GetDoor(i);
                if (!door->IsOpen()) {
                    for (auto& key : my_keys) {
                        if (door->TryOpen(key)) {
                            flag = true;
                            break;
                        }
                    }
                }
                if (flag) {
                    queue.push(door->GoThrough());
                }
            }
        }

        for (size_t i = 0; i < current_room->NumDoors(); ++i) {
            Door* door = current_room->GetDoor(i);

            if (!door->IsOpen()) {
                for (auto& key : my_keys) {
                    if (door->TryOpen(key)) {
                        break;
                    }
                }
            }

            if (door->IsOpen() && door->GoThrough() != nullptr &&
                visited.find(door->GoThrough()) == visited.end()) {
                queue.push(door->GoThrough());
                visited.insert(door->GoThrough());
            }
        }
    }
    return nullptr;
}

#pragma once

#include <string>
#include <unordered_map>
#include <filesystem>
#include <cmath>
#include <random>
#include <fstream>
#include <algorithm>
#include <iostream>

using Map = std::unordered_map<std::string, double>;
using Key = std::unordered_map<char, char>;

constexpr size_t kIters = 3'000;

Map ReadNgrams(const std::filesystem::path& path) {
    std::ifstream in{path};

    if (!in) {
        throw std::runtime_error("Can't open file");
    }

    std::string ngram;
    int count;
    int64_t total_count = 0;
    Map prob;

    while (in >> ngram >> count) {
        prob[ngram] = count;
        total_count += count;
    }
    for (auto& [key, val] : prob) {
        val = std::log(val / total_count);
    }
    return prob;
}

std::string Decode(const std::string& text, const Key& key) {
    std::string decoded;
    for (const char c : text) {
        if (std::isalpha(c)) {
            if (std::isupper(c)) {
                decoded.push_back(key.at(c));
            }
            if (std::islower(c)) {
                decoded.push_back(std::tolower(key.at(std::toupper(c))));
            }
        } else {
            decoded.push_back(c);
        }
    }
    return decoded;
}

double Fitness(const std::string& text, const Map& dict) {
    double fitness = 0.0;
    if (text.size() < 4) {
        return fitness;
    }

    for (size_t pos = 0; pos < (text.size() - 4); ++pos) {
        std::string quadgram = text.substr(pos, 4);
        std::transform(quadgram.begin(), quadgram.end(), quadgram.begin(),
                       [](unsigned char c) { return std::toupper(c); });

        if (dict.contains(quadgram)) {
            fitness += dict.at(quadgram);
        }
    }
    return fitness;
}

Map GetProbs(const std::string& text) {
    Map prob;
    for (char c = 'A'; c <= 'Z'; ++c) {
        prob[std::string(1, c)] = 0;
    }
    int total_count = 0;
    for (char c : text) {
        if (std::isalpha(c)) {
            ++prob[std::string(1, std::toupper(c))];
            ++total_count;
        }
    }
    for (auto& [key, val] : prob) {
        val = std::log(val / total_count);
    }
    return prob;
}

Key Transpose(const Key& key, std::mt19937& gen) {
    Key new_key = key;
    std::uniform_int_distribution<int> dist(0, key.size() - 1);
    std::swap(new_key['A' + dist(gen)], new_key['A' + dist(gen)]);
    return new_key;
}

Key GetKey(const std::string& text, const std::filesystem::path& path) {
    Map lang_letter_probs = ReadNgrams(path);
    Map text_letter_probs = GetProbs(text);
    std::vector<char> lang_ord_by_freq;
    std::vector<char> text_ord_by_freq;
    for (char c = 'A'; c <= 'Z'; ++c) {
        lang_ord_by_freq.push_back(c);
        text_ord_by_freq.push_back(c);
    }

    std::sort(lang_ord_by_freq.begin(), lang_ord_by_freq.end(),
              [&lang_letter_probs](char left, char right) {
                  return (lang_letter_probs[std::string(1, left)] <
                          lang_letter_probs[std::string(1, right)]);
              });
    std::sort(text_ord_by_freq.begin(), text_ord_by_freq.end(),
              [&text_letter_probs](char left, char right) {
                  return (text_letter_probs[std::string(1, left)] <
                          text_letter_probs[std::string(1, right)]);
              });

    Key key;
    for (size_t i = 0; i < 26; ++i) {
        key[text_ord_by_freq[i]] = lang_ord_by_freq[i];
    }
    return key;
}

std::string Decode(const std::string& text, const Map& dict, const std::filesystem::path& path) {

    std::mt19937 gen(123456789);

    Key key = GetKey(text, path);
    std::string decoded_text = Decode(text, key);
    double fitness = Fitness(decoded_text, dict);

    for (size_t i = 0; i < kIters; ++i) {
        Key trial_key = Transpose(key, gen);
        std::string trial_decoding = Decode(text, trial_key);
        double trial_fitness = Fitness(trial_decoding, dict);

        if (trial_fitness > fitness) {
            std::swap(decoded_text, trial_decoding);
            std::swap(key, trial_key);
            std::swap(fitness, trial_fitness);
        }
    }

    return decoded_text;
}

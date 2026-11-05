//
// Created by Kristina Despotuli  on 11/4/26.
//

#include "MovieData.h"
#include <fstream>
#include <string>
#include <unordered_set>

// returns a vector with each element being a line of the csv
// splits title, release data, popularity, rating and id
static std::vector<std::string> parseCSVLine(const std::string& line) {
    std::vector<std::string> fields;
    std::string field;
    bool inQuotes = false;

    for (int i = 0; i < line.size(); i++) {
        char c = line[i];
        if (inQuotes) {
            if (c == '"' && i + 1 < line.size() && line[i + 1] == '"') {
                field += '"';
                i++;
            }
            else if (c == '"') {
                inQuotes = false;
            }
            else {
                field += c;
            }
        }
        else if (c == '"') {
            inQuotes = true;
        }
        else if (c == ',') {
            fields.push_back(field);
            field.clear();
        }
        else if (c != '\r') {
            field += c;
        }
    }
    fields.push_back(field);
    return fields;
}

// stod help function that catches errors
static double toDouble(const std::string& s) {
    try { return s.empty() ? 0.0 : std::stod(s); } catch (...) { return 0.0; }
}

std::vector<Movie> loadMovies(const std::string& filepath, int& duplicateTitles) {
    std::vector<Movie> movies;
    std::unordered_set<std::string> seenTitles;
    duplicateTitles = 0;

    std::ifstream file(filepath);
    if (!file.is_open()) {
        return movies;
    }

    std::string line;
    std::getline(file, line);
    while (std::getline(file, line)) {
        std::vector<std::string> f = parseCSVLine(line);
        if (f.size() < 5 || f[0].empty() || f[1].empty()) continue;

        Movie m;
        try { m.id = std::stoi(f[0]); } catch (...) { continue; }
        m.title = f[1];
        m.releaseDate = f[2];
        m.rating = toDouble(f[3]);
        m.popularity = toDouble(f[4]);

        if (!seenTitles.insert(m.title).second) duplicateTitles++; // title already seen
        movies.push_back(m);
    }
    return movies;
}
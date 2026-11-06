#pragma once
#include <string>

struct Movie {             // Stores one movie record from dataset
    int id = 0;
    std::string title;
    std::string releaseDate;
    double rating = 0.0;
    double popularity = 0.0;

};

//sorts movies by name, if they have the same name they get sorted by popularity, if they have the same popularity they get sorted by ID
inline bool comesBefore(const Movie& a, const Movie& b) {
    if (a.title != b.title) return a.title < b.title;
    if (a.popularity != b.popularity) return a.popularity > b.popularity;
    return a.id < b.id;
}
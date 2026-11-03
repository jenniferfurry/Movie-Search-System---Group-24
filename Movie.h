#pragma once
#include <string>

struct Movie {             // Stores one movie record from dataset
    int id = 0;
    std::string title;
    std::string releaseDate;
    double rating = 0.0;
    double popularity = 0.0;
};

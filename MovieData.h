//
// Created by Kristina Despotuli  on 11/4/26.
//

#pragma once
#include <string>
#include <vector>
#include "Movie.h"

#ifndef MOVIE_SEARCH_SYSTEM_GROUP_24_MOVIEDATA_H
#define MOVIE_SEARCH_SYSTEM_GROUP_24_MOVIEDATA_H


std::vector<Movie> loadMovies(const std::string& fileName, int& duplicatesSkipped);


#endif //MOVIE_SEARCH_SYSTEM_GROUP_24_MOVIEDATA_H

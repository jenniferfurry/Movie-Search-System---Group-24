//
// Created by Kristina Despotuli  on 11/5/26.
//
#pragma once
#include <memory>
#include <random>
#include <string>
#include <vector>

#include "Movie.h"
#include "RedBlackTree.h"
#include "SplayTree.h"

#ifndef MOVIE_SEARCH_SYSTEM_GROUP_24_MOVIEAPP_H
#define MOVIE_SEARCH_SYSTEM_GROUP_24_MOVIEAPP_H


struct ImFont;

// Holds the movie data, both trees, and all screen state.
// draw() is called once per frame and draws whichever page is active.
class MovieApp {
public:
    void setFonts(ImFont* body, ImFont* heading);
    void load(const std::string& path);
    void draw();

private:
    enum class Page { Home, Search, Compare };
    Page page = Page::Home;

    // data
    std::vector<Movie> movies;
    std::unique_ptr<RedBlackTree> rbt;
    std::unique_ptr<SplayTree> splay;
    std::string loadMessage;
    std::string duplicateNote; // disclosure shown on the home page
    bool loaded = false;
    double rbInsertMs = 0, splayInsertMs = 0;
    std::mt19937 rng{42}; // fixed seed so every run uses the same test data

    ImFont* bodyFont = nullptr;
    ImFont* headingFont = nullptr;

    // header search box (top right on every page)
    char headerSearch[256] = "";

    // Search page
    char searchTitle[256] = "";
    int structure = 0; // 0 = Red-Black, 1 = Splay
    struct SearchResult {
        bool ran = false;
        bool found = false;
        Movie movie;
        double micros = 0;
        std::string structureName;
    } searchResult;

    // Compare page: one title
    char compareTitle[256] = "";
    struct CompareResult {
        bool ran = false;
        bool found = false;
        double rbFirstUs = 0, splayFirstUs = 0;     // one search
        double rbRepeatMs = 0, splayRepeatMs = 0;   // same title searched many times
    } compareResult;

    // Compare page: full benchmarks
    struct BenchResult {
        bool ran = false;
        std::string name;
        int searches = 0;
        double rbMs = 0, splayMs = 0;
    } benchResult;

    void drawHeader(const char* pageName);
    bool drawBackButton();
    void drawHome();
    void drawSearch();
    void drawCompare();

    void runSearch();
    void runCompare();
    void runBenchmark(bool repeated);
};


#endif //MOVIE_SEARCH_SYSTEM_GROUP_24_MOVIEAPP_H

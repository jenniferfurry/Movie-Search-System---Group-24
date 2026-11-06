//
// Created by Kristina Despotuli  on 11/5/26.
//

#include "MovieApp.h"

#include <algorithm>
#include <chrono>
#include <cstdio>
#include <cstring>

#include "imgui.h"
#include "MovieData.h"

using Clock = std::chrono::steady_clock;

static const int REPEAT_COUNT = 1000;      // searches of one title on the Compare page
static const int BENCHMARK_COUNT = 100000; // searches in the full benchmarks
static const char* MICRO = "\xC2\xB5s";    // "µs"

static double msSince(Clock::time_point start) {
    return std::chrono::duration<double, std::milli>(Clock::now() - start).count();
}

// Puts the next item in the horizontal center of the window
static void centerNext(float width) {
    float avail = ImGui::GetContentRegionAvail().x;
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + std::max(0.0f, (avail - width) * 0.5f));
}

static std::string yearOf(const Movie& m) {
    return m.releaseDate.size() >= 4 ? m.releaseDate.substr(0, 4) : "n/a";
}

void MovieApp::setFonts(ImFont* body, ImFont* heading) {
    bodyFont = body;
    headingFont = heading;
}

// loading data

void MovieApp::load(const std::string& path) {
    int duplicates = 0;
    movies = loadMovies(path, duplicates);
    if (movies.empty()) {
        loaded = false;
        loadMessage = "Could not load " + path + ". Put tmdb_movies_100000.csv next to the program.";
        return;
    }

    // shuffles data so that csv order does not affect how trees are built
    std::shuffle(movies.begin(), movies.end(), rng);

    rbt = std::make_unique<RedBlackTree>();
    splay = std::make_unique<SplayTree>();

    auto start = Clock::now();
    for (const Movie& m : movies) rbt->insert(m);
    rbInsertMs = msSince(start);

    start = Clock::now();
    for (const Movie& m : movies) splay->insert(m);
    splayInsertMs = msSince(start);

    loaded = true;
    char buf[256];
    std::snprintf(buf, sizeof(buf),
                  "Loaded %zu movies. Insert time: Red-Black %.1f ms, Splay %.1f ms",
                  movies.size(), rbInsertMs, splayInsertMs);
    loadMessage = buf;
    std::snprintf(buf, sizeof(buf),
                  "Note: %d movies share a title with another movie. Searching that title shows the most popular one.",
                  duplicates);
    duplicateNote = buf;
}

void MovieApp::runSearch() {
    if (!loaded) return;
    searchResult = SearchResult();
    searchResult.ran = true;
    searchResult.structureName = structure == 0 ? "Red-Black Tree" : "Splay Tree";
    std::string title = searchTitle;

    auto start = Clock::now();
    Movie* m = structure == 0 ? rbt->search(title) : splay->search(title);
    searchResult.micros = msSince(start) * 1000.0;

    if (m != nullptr) {
        searchResult.found = true;
        searchResult.movie = *m;
    }
}

void MovieApp::runCompare() {
    if (!loaded) return;
    compareResult = CompareResult();
    compareResult.ran = true;
    std::string title = compareTitle;

    // one search in each tree
    auto start = Clock::now();
    Movie* rbMovie = rbt->search(title);
    compareResult.rbFirstUs = msSince(start) * 1000.0;

    start = Clock::now();
    Movie* splayMovie = splay->search(title);
    compareResult.splayFirstUs = msSince(start) * 1000.0;

    compareResult.found = rbMovie != nullptr && splayMovie != nullptr;
    if (!compareResult.found) return;

    // same title searched repeatedly
    start = Clock::now();
    for (int i = 0; i < REPEAT_COUNT; i++) rbt->search(title);
    compareResult.rbRepeatMs = msSince(start);

    start = Clock::now();
    for (int i = 0; i < REPEAT_COUNT; i++) splay->search(title);
    compareResult.splayRepeatMs = msSince(start);
}

void MovieApp::runBenchmark(bool repeated) {
    if (!loaded) return;
    std::uniform_int_distribution<size_t> pickAll(0, movies.size() - 1);
    std::vector<size_t> queries(BENCHMARK_COUNT);

    if (!repeated) {
        for (size_t& q : queries) q = pickAll(rng);
    } else {
        // 90% of searches go to a small "popular" set of 100 movies, 10% anywhere
        size_t hotSize = std::min<size_t>(100, movies.size());
        std::vector<size_t> hot(hotSize);
        for (size_t& h : hot) h = pickAll(rng);
        std::uniform_int_distribution<size_t> pickHot(0, hotSize - 1);
        std::uniform_int_distribution<int> percent(1, 100);
        for (size_t& q : queries) q = percent(rng) <= 90 ? hot[pickHot(rng)] : pickAll(rng);
    }

    // both trees get the exact same list of searches
    auto start = Clock::now();
    for (size_t q : queries) rbt->search(movies[q].title);
    double rbMs = msSince(start);

    start = Clock::now();
    for (size_t q : queries) splay->search(movies[q].title);
    double splayMs = msSince(start);

    benchResult.ran = true;
    benchResult.name = repeated ? "Repeated searches (90% go to 100 popular movies)" : "Random searches";
    benchResult.searches = BENCHMARK_COUNT;
    benchResult.rbMs = rbMs;
    benchResult.splayMs = splayMs;
}

// drawing app

void MovieApp::draw() {
    ImGuiViewport* vp = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(vp->WorkPos);
    ImGui::SetNextWindowSize(vp->WorkSize);
    ImGui::Begin("Movie Search System", nullptr,
                 ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove |
                 ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings);

    switch (page) {
        case Page::Home:    drawHome(); break;
        case Page::Search:  drawSearch(); break;
        case Page::Compare: drawCompare(); break;
    }

    ImGui::End();
}

// title on the left search box on the right
void MovieApp::drawHeader(const char* pageName) {
    ImGui::TextDisabled("%s", pageName);

    if (headingFont) ImGui::PushFont(headingFont);
    ImGui::Text("Movie Search System");
    if (headingFont) ImGui::PopFont();

    float boxWidth = 260.0f;
    ImGui::SameLine(ImGui::GetWindowWidth() - boxWidth - ImGui::GetStyle().WindowPadding.x);
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 6.0f);
    ImGui::SetNextItemWidth(boxWidth);
    if (ImGui::InputTextWithHint("##headerSearch", "Search Title", headerSearch,
                                 sizeof(headerSearch), ImGuiInputTextFlags_EnterReturnsTrue)) {
        // jump to the search page and run the search there
        std::strncpy(searchTitle, headerSearch, sizeof(searchTitle) - 1);
        headerSearch[0] = '\0';
        page = Page::Search;
        runSearch();
    }
    ImGui::Separator();
    ImGui::Spacing();
}

bool MovieApp::drawBackButton() {
    if (ImGui::Button("Back to Main Menu")) {
        page = Page::Home;
        return true;
    }
    ImGui::Spacing();
    return false;
}

void MovieApp::drawHome() {
    drawHeader("Home Page");

    ImGui::Dummy(ImVec2(0, 50));
    const char* welcome = "Welcome to the Movie Search System!";
    if (headingFont) ImGui::PushFont(headingFont);
    centerNext(ImGui::CalcTextSize(welcome).x);
    ImGui::Text("%s", welcome);
    if (headingFont) ImGui::PopFont();

    ImGui::Dummy(ImVec2(0, 60));
    ImVec2 buttonSize(260, 48);
    float gap = 40.0f;
    centerNext(buttonSize.x * 2 + gap);
    ImGui::BeginDisabled(!loaded);
    if (ImGui::Button("Search for a Movie", buttonSize)) page = Page::Search;
    ImGui::SameLine(0, gap);
    if (ImGui::Button("Compare Search Performance", buttonSize)) page = Page::Compare;
    ImGui::EndDisabled();

    ImGui::Dummy(ImVec2(0, 50));
    centerNext(ImGui::CalcTextSize(loadMessage.c_str()).x);
    if (loaded) {
        ImGui::TextDisabled("%s", loadMessage.c_str());
        centerNext(ImGui::CalcTextSize(duplicateNote.c_str()).x);
        ImGui::TextDisabled("%s", duplicateNote.c_str());
    } else {
        ImGui::TextColored(ImVec4(0.8f, 0.1f, 0.1f, 1.0f), "%s", loadMessage.c_str());
    }
}

void MovieApp::drawSearch() {
    drawHeader("Search for a Movie");
    if (drawBackButton()) return;

    ImGui::Indent(40);
    ImGui::AlignTextToFramePadding();
    ImGui::Text("Movie Title:");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(360);
    bool enter = ImGui::InputTextWithHint("##searchTitle", "Search Title", searchTitle,
                                          sizeof(searchTitle), ImGuiInputTextFlags_EnterReturnsTrue);

    ImGui::Spacing();
    ImGui::Text("Data Structure");
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4)); // smaller radio circles
    ImGui::RadioButton("Red-Black Tree", &structure, 0);
    ImGui::RadioButton("Splay Tree", &structure, 1);
    ImGui::PopStyleVar();
    ImGui::SameLine(360);
    if (ImGui::Button("Run", ImVec2(80, 0)) || enter) runSearch();

    ImGui::Dummy(ImVec2(0, 30));
    if (searchResult.ran) {
        if (searchResult.found) {
            const Movie& m = searchResult.movie;
            if (headingFont) ImGui::PushFont(headingFont);
            ImGui::Text("%s (%s)", m.title.c_str(), yearOf(m).c_str());
            if (headingFont) ImGui::PopFont();
            ImGui::Text("Rating: %.1f / 10", m.rating);
            ImGui::Text("Released: %s", m.releaseDate.empty() ? "n/a" : m.releaseDate.c_str());
            ImGui::Text("Popularity: %.3f", m.popularity);
            ImGui::Text("Movie ID: %d", m.id);
        } else {
            ImGui::TextColored(ImVec4(0.8f, 0.1f, 0.1f, 1.0f), "No movie found with that exact title.");
        }
        ImGui::Spacing();
        ImGui::Text("Search Time: %.2f %s (%s)", searchResult.micros, MICRO, searchResult.structureName.c_str());
    }
    ImGui::Unindent(40);
}

void MovieApp::drawCompare() {
    drawHeader("Compare Search Performance");
    if (drawBackButton()) return;

    ImGui::Indent(40);
    ImGui::AlignTextToFramePadding();
    ImGui::Text("Movie Title:");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(360);
    bool enter = ImGui::InputTextWithHint("##compareTitle", "Search Title", compareTitle,
                                          sizeof(compareTitle), ImGuiInputTextFlags_EnterReturnsTrue);
    ImGui::SameLine();
    if (ImGui::Button("Run Test") || enter) runCompare();

    ImGui::Spacing();
    if (compareResult.ran && !compareResult.found) {
        ImGui::TextColored(ImVec4(0.8f, 0.1f, 0.1f, 1.0f), "No movie found with that exact title.");
    } else if (compareResult.ran) {
        char repeatHeader[64];
        std::snprintf(repeatHeader, sizeof(repeatHeader), "Same Title x%d (ms)", REPEAT_COUNT);
        char firstHeader[64];
        std::snprintf(firstHeader, sizeof(firstHeader), "First Search (%s)", MICRO);

        if (ImGui::BeginTable("compareTable", 3, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg,
                              ImVec2(640, 0))) {
            ImGui::TableSetupColumn("Data Structure");
            ImGui::TableSetupColumn(firstHeader);
            ImGui::TableSetupColumn(repeatHeader);
            ImGui::TableHeadersRow();

            ImGui::TableNextRow();
            ImGui::TableNextColumn(); ImGui::Text("Red-Black Tree");
            ImGui::TableNextColumn(); ImGui::Text("%.2f", compareResult.rbFirstUs);
            ImGui::TableNextColumn(); ImGui::Text("%.3f", compareResult.rbRepeatMs);

            ImGui::TableNextRow();
            ImGui::TableNextColumn(); ImGui::Text("Splay Tree");
            ImGui::TableNextColumn(); ImGui::Text("%.2f", compareResult.splayFirstUs);
            ImGui::TableNextColumn(); ImGui::Text("%.3f", compareResult.splayRepeatMs);
            ImGui::EndTable();
        }
    }

    ImGui::Dummy(ImVec2(0, 20));
    ImGui::Unindent(40);
    ImGui::Separator();
    ImGui::Spacing();
    ImGui::Indent(40);

    ImGui::Text("Full benchmark: %d searches, same search list for both trees", BENCHMARK_COUNT);
    if (ImGui::Button("Random Searches")) runBenchmark(false);
    ImGui::SameLine();
    if (ImGui::Button("Repeated Searches (popular movies)")) runBenchmark(true);

    if (benchResult.ran) {
        ImGui::Spacing();
        ImGui::Text("%s", benchResult.name.c_str());
        if (ImGui::BeginTable("benchTable", 3, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg,
                              ImVec2(640, 0))) {
            ImGui::TableSetupColumn("Data Structure");
            ImGui::TableSetupColumn("Total Time (ms)");
            ImGui::TableSetupColumn("Average per Search (ns)");
            ImGui::TableHeadersRow();

            double n = benchResult.searches;
            ImGui::TableNextRow();
            ImGui::TableNextColumn(); ImGui::Text("Red-Black Tree");
            ImGui::TableNextColumn(); ImGui::Text("%.2f", benchResult.rbMs);
            ImGui::TableNextColumn(); ImGui::Text("%.1f", benchResult.rbMs * 1e6 / n);

            ImGui::TableNextRow();
            ImGui::TableNextColumn(); ImGui::Text("Splay Tree");
            ImGui::TableNextColumn(); ImGui::Text("%.2f", benchResult.splayMs);
            ImGui::TableNextColumn(); ImGui::Text("%.1f", benchResult.splayMs * 1e6 / n);
            ImGui::EndTable();
        }
        bool rbFaster = benchResult.rbMs < benchResult.splayMs;
        double ratio = std::max(benchResult.rbMs, benchResult.splayMs) /
                       std::max(1e-9, std::min(benchResult.rbMs, benchResult.splayMs));
        ImGui::Text("%s was faster by %.2fx", rbFaster ? "Red-Black Tree" : "Splay Tree", ratio);
    }
    ImGui::Unindent(40);
}

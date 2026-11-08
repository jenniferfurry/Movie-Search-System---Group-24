# Movie Search System

## Overview
The Movie Search System is a C++ desktop application that searches a movie dataset using two different self-balancing tree data structures: a **Red-Black Tree** and a **Splay Tree**. It also includes benchmarking tools that let users compare the search performance of the two structures.

The application has a graphical user interface built with Dear ImGui, GLFW, and OpenGL.


## Features
- Load movie records from a CSV dataset.
- Search for a movie by its exact title using either the Red-Black Tree or the Splay Tree.
- View a movie's title, release year, rating, release date, popularity, and ID.
- See the time taken by an individual search.
- Compare the first search and repeated searches for the same title in both trees.
- Run larger benchmarks using random searches or repeated searches that favor a smaller group of popular titles.
- Display how many movie records were loaded and how many titles are shared by multiple records.

## Data Structures
The project implements two tree-based data structures:

### Red-Black Tree
A self-balancing binary search tree that uses node colors and rotations to maintain balance after insertion. Searching follows the title ordering used by the project.

### Splay Tree
A self-adjusting binary search tree. After a successful search or insertion, rotations move the accessed or inserted node toward the root. This can make later searches for recently accessed titles faster.

### Movie Ordering
Movies are ordered alphabetically by title. When titles are the same, records are ordered by popularity (highest first), then by movie ID. If multiple records share a title, searching for that title returns the most popular matching movie.

## Dataset
The application uses `tmdb_movies_100000.csv`, a dataset containing approximately 100,000 movie records.

Each movie contains the following information:
- `id` - Movie identifier
- `title` - Movie title
- `release_date` - Movie release date
- `vote_average` - Movie rating
- `popularity` - Popularity value

The program loads the CSV when it starts and builds both trees from the loaded records. The dataset file must remain available; the CMake build copies it next to the executable.

## Technologies Used
- C++
- CMake
- Dear ImGui
- GLFW
- OpenGL
- Git and GitHub

## Installation and Execution

### Requirements
- A C++17-compatible compiler
- CMake 3.16 or later
- Git
- OpenGL development libraries
- A supported desktop environment for GLFW


### 1. Clone the Repository
```bash
git clone [https://github.com/jenniferfurry/Movie-Search-System---Group-24.git](https://github.com/jenniferfurry/Movie-Search-System---Group-24.git)
cd Movie-Search-System---Group-24
```

### 2. Configure the Project
```bash
cmake -S . -B build
```

### 3. Build the Application
```bash
cmake --build build --config Release
```

### 4. Run the Application

macOS / Linux:
```bash
./build/MovieSearchSystem
```

Windows:
```powershell
.\build\Release\MovieSearchSystem.exe
```


## How to Use the Application

### Search for a Movie

1. Open Search for a Movie from the home screen.
2. Enter the movie's exact title in the search box.
3. Select either the Red-Black Tree or Splay Tree.
4. Select Run or press Enter.
5. If a match is found, the application displays the movie information and search time. If no exact title match is found, a not-found message is displayed.

### Compare Search Performance

1. Select Compare Search Performance from the home screen.
2. Enter a movie title and select Run Test to compare one search and 1,000 repeated searches for that title in both trees.
3. Select Random Searches to benchmark 100,000 searches using randomly selected movie titles.
4. Select Repeated Searches (popular movies) to benchmark 100,000 searches where 90% of searches target a smaller group of up to 100 selected movies.
5. Compare the elapsed times displayed for the Red-Black Tree and Splay Tree.

Both trees receive the same search inputs during each full benchmark. Results may vary depending on the computer and system conditions.

## Project Structure

| File | Description |
|---|---|
| `main.cpp` | Initializes GLFW, OpenGL, and Dear ImGui, then runs the application loop. |
| `MovieApp.h` / `MovieApp.cpp` | Manages application screens, user interaction, searches, and benchmarks. |
| `Movie.h` | Defines the Movie record and the ordering function used by the trees. |
| `MovieData.h` / `MovieData.cpp` | Reads and parses movie records from the CSV file. |
| `RedBlackTree.h` / `RedBlackTree.cpp` | Implements the Red-Black Tree. |
| `SplayTree.h` / `SplayTree.cpp` | Implements the Splay Tree. |
| `CMakeLists.txt` | Defines the build configuration and external dependencies. |
| `tmdb_movies_100000.csv` | Movie dataset loaded by the application. |

## Contributors
Jennifer Furry (jenniferfurry), Kristina Despotuli (kristina-despotuli), & Maya Crecos (mayacrecos)
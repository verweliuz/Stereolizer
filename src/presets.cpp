#include <bits/stdc++.h>
#include "util.cpp"

using namespace std;

void tetrahedron(map<char, Dot> &dots) {
    dots['A'] = {'A', {1e-20, 1 / sqrt(3), 2 / sqrt(3)}, {'B', 'C', 'D'}};
    dots['B'] = {'B', {1, 1 / sqrt(3), -1 / sqrt(3)}, {'A', 'C', 'D'}};
    dots['C'] = {'C', {-1, 1 / sqrt(3), -1 / sqrt(3)}, {'A', 'B', 'D'}};
    dots['D'] = {'D', {1e-20, -2 / sqrt(3), 1e-20}, {'A', 'B', 'C'}};
}

void cube(map<char, Dot> &dots) {
    dots['A'] = {'A', {-1, 1, -1}, {'B', 'D', 'P'}};
    dots['B'] = {'B', {-1, 1, 1}, {'A', 'C', 'R'}};
    dots['C'] = {'C', {1, 1, 1}, {'B', 'D', 'S'}};
    dots['D'] = {'D', {1, 1, -1}, {'A', 'C', 'T'}};

    dots['P'] = {'P', {-1, -1, -1}, {'R', 'T', 'A'}};
    dots['R'] = {'R', {-1, -1, 1}, {'P', 'S', 'B'}};
    dots['S'] = {'S', {1, -1, 1}, {'R', 'T', 'C'}};
    dots['T'] = {'T', {1, -1, -1}, {'S', 'P', 'D'}};
}

void cuboid(map<char, Dot> &dots) {
    dots['A'] = {'A', {-1.5, 1, -1}, {'B', 'D', 'P'}};
    dots['B'] = {'B', {-1.5, 1, 1}, {'A', 'C', 'R'}};
    dots['C'] = {'C', {1.5, 1, 1}, {'B', 'D', 'S'}};
    dots['D'] = {'D', {1.5, 1, -1}, {'A', 'C', 'T'}};

    dots['P'] = {'P', {-1.5, -1, -1}, {'R', 'T', 'A'}};
    dots['R'] = {'R', {-1.5, -1, 1}, {'P', 'S', 'B'}};
    dots['S'] = {'S', {1.5, -1, 1}, {'R', 'T', 'C'}};
    dots['T'] = {'T', {1.5, -1, -1}, {'S', 'P', 'D'}};
}

void parallelepiped(map<char, Dot> &dots) {
    dots['A'] = {'A', {-1.7, 1, -1}, {'B', 'D', 'P'}};
    dots['B'] = {'B', {-1.7, 1, 1}, {'A', 'C', 'R'}};
    dots['C'] = {'C', {0.8, 1, 1}, {'B', 'D', 'S'}};
    dots['D'] = {'D', {0.8, 1, -1}, {'A', 'C', 'T'}};

    dots['P'] = {'P', {-0.8, -1, -1}, {'R', 'T', 'A'}};
    dots['R'] = {'R', {-0.8, -1, 1}, {'P', 'S', 'B'}};
    dots['S'] = {'S', {1.7, -1, 1}, {'R', 'T', 'C'}};
    dots['T'] = {'T', {1.7, -1, -1}, {'S', 'P', 'D'}};
}
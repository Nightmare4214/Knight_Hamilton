#pragma once

#include <iostream>
#include <vector>
#include <fstream>
#include <set>
using namespace std;
class Knight {
private:
    //(后继，前驱)(用i*col+j来表示)
    typedef pair<size_t, size_t> Point;
    //高，宽
    size_t row, col;
    bool ok;
    //棋盘，每个点存后继和前驱
    vector<vector<Point> > chess_link;
    size_t pos(const size_t& x, const size_t& y, const size_t& width)const;
    const static pair<size_t, size_t> id2chess[9];
    static size_t chess2id(const size_t& x, const size_t& y);
    //马踏棋盘在特定的棋盘上的顺序
    vector<Point> init_chessboard[9];
    bool load_path();
    void build(const size_t& height, const size_t& width, const size_t& offset_x, const size_t& offset_y);
    bool divide_and_conquer(size_t height, size_t width, size_t offset_x, size_t offset_y);
    bool ans_check(vector<Point>& result);
public:
    Knight(const size_t& row, const size_t& col);
    void solution();
};
#include "Knight.h"
//�������̵ĸ߿�
const pair<size_t, size_t> Knight::id2chess[9] = { {6,6},{6,8},{8,6},{8,8},{8,10},{10,8},{10,10},{10,12},{12,10} };
/**
 * ����λ��ת����
 * @param x x
 * @param y y
 * @param width �п�
 * @return ����
 */
size_t Knight::pos(const size_t& x, const size_t& y, const size_t& width) const {
    return x * width + y;
}

/**
 * �������̵�id
 * @param x ��
 * @param y ��
 * @return id
 */
size_t Knight::chess2id(const size_t& x, const size_t& y) {
    if (x == 6 && y == 6) {
        return 0;
    }
    else if (x == 6 && y == 8) {
        return 1;
    }
    else if (x == 8 && y == 6) {
        return 2;
    }
    else if (x == 8 && y == 8) {
        return 3;
    }
    else if (x == 8 && y == 10) {
        return 4;
    }
    else if (x == 10 && y == 8) {
        return 5;
    }
    else if (x == 10 && y == 10) {
        return 6;
    }
    else if (x == 10 && y == 12) {
        return 7;
    }
    else if (x == 12 && y == 10) {
        return 8;
    }
    return -1;
}


Knight::Knight(const size_t& row, const size_t& col) :row(row), col(col), ok(false) {
    if (0 != (row & 1u) || 0 != (col & 1u) || row<6 || col<6 || row>col + 2 || col>row + 2) {
        cout << "Ŀǰ��֧����������" << endl;
    }
    else {
        chess_link.resize(row, vector<Point>(col));
        ok = load_path();
        if (!ok) {
            cout << "������ļ����ˣ�" << endl;
        }
    }
}

bool Knight::load_path() {
    ifstream fin("board_path.txt");
    if (!fin) {
        return false;
    }
    for (size_t k = 0; k < 9; ++k) {
        size_t x = id2chess[k].first;
        size_t y = id2chess[k].second;
        vector<Point>& cur = init_chessboard[k];
        cur.resize(x * y);
        for (size_t i = 0; i < x; ++i) {
            for (size_t j = 0; j < y; ++j) {
                size_t temp;
                fin >> temp;
                cur[temp - 1] = { i,j };
            }
        }
    }
    return true;
}
/**
 * ����������
 * @param height ��
 * @param width ��
 * @param offset_x x��ƫ����
 * @param offset_y y��ƫ����
 */
void Knight::build(const size_t& height, const size_t& width, const size_t& offset_x, const size_t& offset_y) {
    size_t size = height * width;
    vector<Point>& cur = init_chessboard[chess2id(height, width)];
    for (size_t i = 0; i < size; ++i) {
        size_t x1 = offset_x + cur[i].first;
        size_t y1 = offset_y + cur[i].second;

        size_t x2 = offset_x + cur[(i + 1) % size].first;
        size_t y2 = offset_y + cur[(i + 1) % size].second;

        chess_link[x1][y1].first = pos(x2, y2, this->col);
        chess_link[x2][y2].second = pos(x1, y1, this->col);
    }
}
/**
 * ����
 * @param height ��
 * @param width ��
 * @param offset_x x��ƫ����
 * @param offset_y y��ƫ����
 * @return �Ƿ�ɹ�
 */
bool Knight::divide_and_conquer(size_t height, size_t width, size_t offset_x, size_t offset_y) {
    if (0 != (height & 1u) || 0 != (width & 1u) || height<6 || width<6 || height>width + 2 || width>height + 2) {
        return false;
    }
    if (height < 12 || width < 12) {
        build(height, width, offset_x, offset_y);
        return true;
    }
    //�ĵȷ�
    size_t row1 = height >> 1u;
    if (0 != height % 4) {
        --row1;
    }
    size_t row2 = height - row1;

    size_t col1 = width >> 1u;
    if (0 != width % 4) {
        --row1;
    }
    size_t col2 = width - col1;
    //��
    //����
    divide_and_conquer(row1, col1, offset_x, offset_y);
    //����
    divide_and_conquer(row1, col2, offset_x, offset_y + col1);
    //����
    divide_and_conquer(row2, col1, offset_x + row1, offset_y);
    //����
    divide_and_conquer(row2, col2, offset_x + row1, offset_y + col1);

    //ɾ��������
    size_t x[8] = { offset_x + row1 - 1 };
    size_t y[8] = { offset_y + col1 - 3 };

    x[1] = x[0] - 1, y[1] = y[0] + 2;
    x[2] = x[1] - 1, y[2] = y[1] + 2;
    x[3] = x[2] + 2, y[3] = y[2] - 1;
    x[4] = x[3] + 1, y[4] = y[3] + 2;
    x[5] = x[4] + 1, y[5] = y[4] - 2;
    x[6] = x[5] + 1, y[6] = y[5] - 2;
    x[7] = x[6] - 2, y[7] = y[6] + 1;

    size_t id[8];
    for (size_t i = 0; i < 8; ++i) {
        id[i] = pos(x[i], y[i], this->col);
    }

    //ɾ���ɵıߣ������µı�
    for (size_t i = 1; i < 8; i += 2) {
        size_t j1 = (i + 1) % 8;
        size_t j2 = (i + 2) % 8;

        if (chess_link[x[i]][y[i]].first == id[i - 1]) {
            chess_link[x[i]][y[i]].first = id[j1];
        }
        else {
            chess_link[x[i]][y[i]].second = id[j1];
        }

        if (chess_link[x[j1]][y[j1]].first == id[j2]) {
            chess_link[x[j1]][y[j1]].first = id[i];
        }
        else {
            chess_link[x[j1]][y[j1]].second = id[i];
        }
    }
    return true;
}
/**
 * ���
 */
void Knight::solution() {
    if (!ok || !divide_and_conquer(this->row, this->col, 0, 0)) {
        cout << "ľ������飬�Ͳ�Ҫ������" << endl;
        return;
    }
    vector<vector<size_t> > chessboard(this->row);
    for (auto& cur : chessboard) {
        cur.resize(this->col, 0);
    }
    vector<pair<size_t, size_t> > ans;

    ans.emplace_back(0, 0);
    size_t i = 0;
    size_t j = 0;
    chessboard[0][0] = 1;
    size_t k = 2;
    size_t size = this->row * this->col;
    for (size_t p = 1; p < size; ++p) {
        size_t x = chess_link[i][j].first;
        size_t y = chess_link[i][j].second;
        i = x / this->col;
        j = x % this->col;
        if (chessboard[i][j] > 0) {
            i = y / this->col;
            j = y % this->col;
        }
        ans.emplace_back(i, j);
        chessboard[i][j] = k;
        ++k;
    }
    if (!ans_check(ans)) {
        cout << "faQ" << endl;
        return;
    }
    for (size_t p = 0; p < size; ++p) {
        cout << '(' << ans[p].first << ',' << ans[p].second << ')' << '\t';
        if ((p + 1) % this->col == 0) {
            cout << endl;
        }
    }
}
/**
 * �����Ƿ���ȷ
 * @param result ���
 * @return �Ƿ���ȷ
 */
bool Knight::ans_check(vector<Point>& result) {
    //����Ƿ���������
    if (result.size() != this->row * this->col) {
        return false;
    }
    //����ƶ�����
    int moves[8][2] = { {-1,2},{1,2},{-2,1},{2,1},
                        {-1,-2},{1,-2},{-2,-1},{2,-1} };
    //����
    set<size_t> check;
    for (size_t i = 0; i < result.size(); ++i) {
        //Խ����
        if (result[i].first < 0 || result[i].first >= this->row ||
            result[i].second < 0 || result[i].second >= this->col) {
            return false;
        }
        //��ǰλ��
        int x = static_cast<int>(result[i].first);
        int y = static_cast<int>(result[i].second);
        if (0 == i) {
            check.insert(pos(x, y, this->col));
        }
        else {
            //��һ��
            int pre_x = static_cast<int>(result[i - 1].first);
            int pre_y = static_cast<int>(result[i - 1].second);
            //�ƶ�����
            int vertical = x - pre_x;
            int horizontal = y - pre_y;
            //����ƶ��Ƿ�Ϸ�
            bool flag = false;
            for (size_t j = 0; j < 8; ++j) {
                if (moves[j][0] == vertical && moves[j][1] == horizontal) {
                    flag = true;
                    break;
                }
            }
            if (!flag) {
                return false;
            }
            check.insert(pos(x, y, this->col));
            //����Ƿ��ظ�
            if (check.size() != i + 1) {
                return false;
            }
        }
    }
    //���һ��
    int pre_x = static_cast<int>(result.back().first);
    int pre_y = static_cast<int>(result.back().second);
    //���
    int x = static_cast<int>(result[0].first);
    int y = static_cast<int>(result[0].second);
    int vertical = x - pre_x;
    int horizontal = y - pre_y;
    //����Ƿ��ܻص����
    bool flag = false;
    for (size_t j = 0; j < 8; ++j) {
        if (moves[j][0] == vertical && moves[j][1] == horizontal) {
            flag = true;
            break;
        }
    }
    return flag;
}

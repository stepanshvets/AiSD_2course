#include <iostream>
#include <stdlib.h>
#include <list>
#include "Shape.h"
#include "Screen.h"
#include "Line.h"
#include "Rectangle.h"

class myshape : public rectangle { // ��� ������ ��������
    int w, h;                                             //���������������
    line l_eye; // ����� ���� � ��� ������ �������� �����
    line r_eye; // ������ ����
    line mouth; // ���
public:
    myshape(point, point);
    void draw();
    void move(int, int);
    void resize(int) { }
};

myshape::myshape(point a, point b)
    : rectangle(a, b), //������������� �������� ������
    w(neast().x - swest().x + 1), //������������� ������
    h(neast().y - swest().y + 1), // - ������ � ������� ����������!
    l_eye(point(swest().x + 2, swest().y + h * 3 / 4), 2),
    r_eye(point(swest().x + w - 4, swest().y + h * 3 / 4), 2),
    mouth(point(swest().x + 2, swest().y + h / 4), w - 4)
{ }

void myshape::draw()
{
    rectangle::draw(); //������ ���� (����� � ��� �������� ����!)
    int a = (swest().x + neast().x) / 2;
    int b = (swest().y + neast().y) / 2;
    put_point(point(a, b)); // ��� � ���������� ������ �� �������!
}

void myshape::move(int a, int b)
{
    rectangle::move(a, b);
    l_eye.move(a, b);  r_eye.move(a, b);
    mouth.move(a, b);
}

/*------------------NEW-CODE-------------------*/

class triangle : public rectangle {
    int w, h;
    int top_x = (ne.x + nwest().x) / 2, top_y = ne.y;
    enum class rotate { n, w, s, e };
    rotate state = rotate::n;
public:
    triangle(point, point);
    point top() const { return point(top_x, top_y); }
    void draw();
    void move(int a, int b);
    void rotate_right();
    void rotate_left();
    void resize(double d);
    void flip_horisontally();
    void flip_vertically();
};

triangle::triangle(point a, point b)
    :rectangle(a, b),
    w(a.x > b.x ? a.x - b.x : b.x - a.x),
    h(a.y > b.y ? a.y - b.y : b.y - a.y) {};

void triangle::draw() {
    switch (state) {
    case(rotate::n):
        put_line(top(), seast());
        put_line(seast(), sw);   put_line(sw, top());
        break;
    case(rotate::e):
        put_line(top(), nwest());
        put_line(nwest(), sw);   put_line(sw, top());
        break;
    case(rotate::s):
        put_line(top(), ne);
        put_line(ne, nwest());   put_line(nwest(), top());
        break;
    case(rotate::w):
        put_line(top(), ne);
        put_line(ne, seast());   put_line(seast(), top());
        break;
    }
    put_line(ne, sw);
    put_line(seast(), nwest());
}


void triangle::rotate_right()
{
    ne.x = sw.x + h * 2; ne.y = sw.y + w / 2;
    w = ne.x - sw.x; h = ne.y - sw.y;
    switch (state) {
    case(rotate::n):
        top_x = ne.x; top_y = (sw.y + ne.y) / 2;
        state = rotate::e;
        break;
    case(rotate::e):
        top_x = (sw.x + seast().x) / 2; top_y = sw.y;
        state = rotate::s;
        break;
    case(rotate::s):
        top_x = sw.x; top_y = (sw.y + ne.y) / 2;
        state = rotate::w;
        break;
    case(rotate::w):
        top_x = (ne.x + nwest().x) / 2; top_y = ne.y;
        state = rotate::n;
        break;
    }
}

void triangle::rotate_left()
{
    ne.x = sw.x + h * 2; ne.y = sw.y + w / 2;
    w = ne.x - sw.x; h = ne.y - sw.y;
    switch (state) {
    case(rotate::s):
        top_x = ne.x; top_y = (sw.y + ne.y) / 2;
        state = rotate::e;
        break;
    case(rotate::w):
        top_x = (sw.x + seast().x) / 2; top_y = sw.y;
        state = rotate::s;
        break;
    case(rotate::n):
        top_x = sw.x; top_y = (sw.y + ne.y) / 2;
        state = rotate::w;
        break;
    case(rotate::e):
        top_x = (ne.x + nwest().x) / 2; top_y = ne.y;
        state = rotate::n;
        break;
    }
}

void triangle::flip_vertically() {
    if (state == rotate::n || state == rotate::s) {
        rotate_left();
        rotate_left();
    }
}

void triangle::flip_horisontally() {
    if (state == rotate::e || state == rotate::w) {
        rotate_left();
        rotate_left();
    }
}

void triangle::move(int a, int b) {
    sw.x += a; sw.y += b; ne.x += a; ne.y += b;
    top_x += a; top_y += b;
}

void triangle::resize(double d)
{
    ne.x = sw.x + (ne.x - sw.x) * d; ne.y = sw.y + (ne.y - sw.y) * d;
    w = ne.x - sw.x; h = ne.y - sw.y;
    switch (state) {
    case(rotate::w):
        top_x = ne.x; top_y = (sw.y + ne.y) / 2;
        break;
    case(rotate::s):
        top_x = (sw.x + seast().x) / 2; top_y = sw.y;
        break;
    case(rotate::e):
        top_x = sw.x; top_y = (sw.y + ne.y) / 2;
        break;
    case(rotate::n):
        top_x = (ne.x + nwest().x) / 2; top_y = ne.y;
        break;
    }
}

void up_left(shape& p, const shape& q) {
    point nw = q.nwest();
    point se = p.seast();
    p.move(nw.x - se.x, nw.y - se.y + 1);
}

void up_right(shape& p, const shape& q) {
    point ne = q.neast();
    point sw = p.swest();
    p.move(ne.x - sw.x, ne.y - sw.y + 1);
}

int main() {
    screen_init();
    rectangle hat(point(0, 0), point(14, 5));
    line brim(point(0, 15), 17);
    myshape face(point(15, 10), point(27, 18));
    hat.rotate_right();
    brim.resize(2.0);
    face.resize(2.0);
    face.move(0, -10);
    up(brim, face);
    up(hat, brim);
    hat.move(0, 15);
    brim.move(0, 15);
    face.move(0, 15);

    /*------------------NEW-CODE-------------------*/

    triangle tie(point(0, 0), point(18, 11));
    triangle left_feather(point(40, 14), point(46, 17));
    triangle right_feather(point(50, 14), point(56, 17));
    shape_refresh();
    std::cout << "Screen 1\n";
    std::cin.get();

    left_feather.resize(1.3);
    right_feather.resize(1.3);
    left_feather.flip_vertically();
    right_feather.flip_vertically();
    tie.resize(0.7);

    shape_refresh();
    std::cout << "Screen 2\n";
    std::cin.get();

    down(tie, face);
    up_left(left_feather, hat);
    up_right(right_feather, hat);
    shape_refresh();

    screen_destroy();
    return 0;
}
#pragma once
#include "Screen.h"
#include <list>
#include <iostream>

char screen[YMAX][XMAX];   //����� ��� �����������
enum color { black = '*', white = '.' };
void screen_init()        //������� ������ (�������������)
{
    for (int y = 0; y < YMAX; y++)
        for (auto& x : screen[y])  x = white;
}
void screen_destroy()
{
    for (auto y = 0; y < YMAX; ++y)
        for (auto& x : screen[y])  x = black;
}
void screen_clear() { screen_init(); } //������� ������
void screen_refresh() // ���������� ������
{
    for (int y = YMAX - 1; 0 <= y; --y) { // � ������� ������ �� ������
        for (auto x : screen[y])    // �� ������ ������� �� �������
            std::cout << x;
        std::cout << '\n';
    }
}


bool on_screen(int a, int b)     // �������� ���������
{
    return 0 <= a && a < XMAX && 0 <= b && b < YMAX;
}


void put_point(int a, int b)               // ��������� �����
{
    if (on_screen(a, b)) screen[b][a] = black;
}


void put_line(int x0, int y0, int x1, int y1) // ��������� �����
{
    int dx = 1, a = x1 - x0;
    if (a < 0) { dx = -1;  a = -a; }
    int dy = 1, b = y1 - y0;
    if (b < 0) { dy = -1; b = -b; }
    int twoA{ a * 2 }, twoB{ b * 2 }, xCrit{ -b + twoA }, eps{ 0 };
    for (;;)
    {
        put_point(x0, y0);
        if (x0 == x1 && y0 == y1)  break;
        if (eps <= xCrit) { x0 += dx; eps += twoB; }
        if (eps >= a || a < b) { y0 += dy; eps -= twoA; }
    }
}

struct shape { // ����������� ������� ����� �������
    static std::list<shape*> shapes; // ������ ����� (���� �� ��� ������!)
    shape() { shapes.push_back(this); }
    //����������� ������������ ������ � ������ ��� ���������
    virtual point north() const = 0;      //����� ��� ��������
    virtual point south() const = 0;
    virtual point east() const = 0;
    virtual point west() const = 0;
    virtual point neast() const = 0;
    virtual point seast() const = 0;
    virtual point nwest() const = 0;
    virtual point swest() const = 0;
    virtual void draw() = 0;              //���������
    virtual void move(int, int) = 0;          //�����������
    virtual void resize(double) = 0;          //��������� �������
    virtual ~shape() { shapes.remove(this); }
};

std::list<shape*> shape::shapes; //���������� ������ �����

void shape_refresh() //����������� ���� ����� �� ������
{
    screen_clear();
    for (auto p : shape::shapes) p->draw(); //������������ ����������!!!
    screen_refresh();
}
//������������� ������ ������������ ����������

class rotatable : virtual public shape { //������, ��������� � ��������
public:
    virtual void rotate_left() = 0; //��������� �����
    virtual void rotate_right() = 0; //��������� ������
};

class reflectable : virtual public shape {
public:
    virtual void flip_horisontally() = 0;
    virtual void flip_vertically() = 0;
};


void up(shape& p, const shape& q) // ��������� p ��� q
{     // ��� ������� �������, �� ���� ������!
  // ������������ ����������!!
    point n = q.north();
    point s = p.south();
    p.move(n.x - s.x, n.y - s.y + 1);
}

// ������ ������� : �������������� ������� ��������������

void down(shape& p, const shape& q) //��������� p ��� q
{
    point n = q.south();
    point s = p.north();
    p.move(n.x - s.x, n.y - s.y - 1);
}
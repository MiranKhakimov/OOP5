#include <iostream>
#include <string>
#include <cstdio>
#include <windows.h>

using namespace std;

class Point {
protected:
    double x;
    double y;

public:
    Point() {
		x = 0;
		y = 0;
		printf("Point with default x:%f y:%f\n", x, y);
	}

	Point(double x, double y) {
		this->x = x;
		this->y = y;
		printf("Point created with x:%f y:%f\n", x, y);
	}

    Point(const Point& other) {
		printf("Point copied\n");
		x = other.x;
		y = other.y;
	}

    virtual ~Point() {
        printf("Point(x:%f y:%f) destructed\n", x, y);
    }

    void setPosition(double x, double y) {
        this->x = x;
        this->y = y;
    }

    double getX() { 
        return x; 
    }
    double getY() { 
        return y; 
    }

    void reset() {
        x = 0;
        y = 0;
        printf("Point reset\n");
    }

    virtual void characteristic() {
        printf("Point coords: x=%f y=%f)\n", x, y);
    }

    virtual string classname() {
        return "Point";
    }

    virtual bool isA(string classname) {
        return classname == "Point";
    }
};

class Circle : public Point {
protected:
    double radius;

public:
	Circle() : Point(){
		radius = 1.;
		printf("Circle created with default radius:%f\n", radius);
	}

	Circle(double x, double y, double radius) : Point(x, y) {
		this->radius = radius;
		printf("Circle created with x:%f y:%f radius:%f\n", x, y, radius);
	}


    Circle(const Circle &other) : Point(other) {
        radius = other.radius;
        printf("Circle copied\n");
    }

    ~Circle() override {
        printf("Circle(x:%f y:%f radius:%f) destructed\n", x, y, radius);
    }

    double getRadius() {
        return radius; 
    }
    void setRadius(double radius) { 
        this->radius = radius; 
    }

    void reset() {
        Point::reset();
        radius = 0.0;
        printf("Circle reset\n");
    }

    void somefunc(){
        printf("unique circle func\n");
    }

    void characteristic() override {
        printf("Circle coords: x=%f y=%f, radius=%f\n", x, y, radius);
    }

    string classname() override {
        return "Circle";
    }

    bool isA(string classname) override {
        return classname == "Circle" || Point::isA(classname);
    }
};

// ==================== ПЕРЕДАЧА ОБЪЕКТОВ В ФУНКЦИИ ====================

// Передача объекта копией
void func1(Point obj) {
    // > func1(Point obj) - получена Копия
    printf("> func1(Point obj) - COPY received\n");
    obj.characteristic();
    // < func1 завершается, копия будет уничтожена
    printf("< func1 finishing, copy will be destroyed\n");
}

// Передача указателя на объект
void func2(Point *obj) {
    // > func2(Point *obj) - получен Указатель (копии нет)
    printf("> func2(Point *obj) - POINTER received (no copy)\n");
    obj->characteristic();
    // < func2 завершается, объект не уничтожается
    printf("< func2 finishing, object NOT destroyed\n");
}

// Передача ссылки на объект
void func3(Point &obj) {
    // > func3(Point &obj) - получена Ссылка (копии нет)
    printf("> func3(Point &obj) - REFERENCE received (no copy)\n");
    obj.characteristic();
    // < func3 завершается, объект НЕ уничтожается
    printf("< func3 finishing, object NOT destroyed\n");
}

// ==================== ВОЗВРАТ ОБЪЕКТОВ ИЗ ФУНКЦИЙ ====================

// Возврат статического локального объекта (КОПИЯ)
Point func_ret1() {
    static Point p(1.0, 1.5);
    // func_ret1: возвращаем статический объект (копию)
    printf("func_ret1: returning static object (copy)\n");
    return p;
}

// Возврат указателя на статический объект
Point* func_ret2() {
    static Point p(2.0, 2.5);
    // func_ret2: возвращаем указатель на статический объект
    printf("func_ret2: returning pointer to static object\n");
    return &p;
}

// Возврат ссылки на статический объект
Point& func_ret3() {
    static Point p(3.0, 3.5);
    // func_ret3: возвращаем ссылку на статический объект
    printf("func_ret3: returning reference to static object\n");
    return p;
}

// Возврат динамического объекта через value (УТЕЧКА!)
Point func_ret4() {
    Point *p = new Point(4.0, 4.5);
    // func_ret4: возвращаем *p (копию динамического объекта) - УТЕЧКА
    printf("func_ret4: returning *p (copy of dynamic object) - LEAK\n");
    return *p; // p не удаляется - утечка
}

// Возврат динамического объекта через указатель
Point* func_ret5() {
    Point *p = new Point(5.0, 5.5);
    // func_ret5: возвращаем указатель на динамический объект
    printf("func_ret5: returning pointer to dynamic object\n");
    return p;
}

// Возврат динамического объекта через ссылку (утечка)
Point& func_ret6() {
    Point *p = new Point(6.0, 6.5);
    // func_ret6: возвращаем ссылку на динамический объект - УТЕЧКА
    printf("func_ret6: returning reference to dynamic object - LEAK\n");
    return *p; // p не удаляется - утечка
}

// ==================== ДЕМОНСТРАЦИЯ ====================

int main() {
    SetConsoleOutputCP(CP_UTF8);

    // ЛР5. Часть 2: Передача и возврат объектов
    printf("LR5. Part 2: Passing and returning objects\n\n");

    // ===== Передача объектов в функции =====
    printf("=== Passing objects to functions ===\n");
    Point p(1.0, 2.0);
    Circle c(3.0, 4.0, 5.0);

    printf("\n> Call func1(p) - pass by value:\n");
    func1(p);

    printf("\n> Call func2(&p) - pass by pointer:\n");
    func2(&p);

    printf("\n> Call func3(p) - pass by reference:\n");
    func3(p);

    // Срезка: Circle превращается в Point
    printf("\n> Call func1(c) - pass derived by value (SLICING):\n");
    func1(c);

    // вызовется Circle::characteristic()
    printf("\n> Call func2(&c) - pass derived by pointer (polymorphism):\n");
    func2(&c);

    // вызовется Circle::characteristic()
    printf("\n> Call func3(c) - pass derived by reference (polymorphism):\n");
    func3(c);

    // ===== Возврат объектов из функций =====
    printf("\n\n=== Returning objects from functions ===\n");

    printf("\n> func_ret1() - return static object by value:\n");
    Point r1 = func_ret1();
    r1.characteristic();

    printf("\n> func_ret2() - return pointer to static object:\n");
    Point *r2 = func_ret2();
    r2->characteristic();

    printf("\n> func_ret3() - return reference to static object:\n");
    Point &r3 = func_ret3();
    r3.characteristic();

    printf("\n> func_ret4() - return dynamic object by value (LEAK):\n");
    Point r4 = func_ret4();
    r4.characteristic();

    printf("\n> func_ret5() - return pointer to dynamic object:\n");
    Point *r5 = func_ret5();
    r5->characteristic();
    delete r5; // Правильно удаляем!

    printf("\n> func_ret6() - return reference to dynamic object (LEAK):\n");
    Point &r6 = func_ret6();
    r6.characteristic();

    printf("\nEnd of code\n");
    return 0;
}
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

int main() {
    // 1. Перекрытие методов
    printf("1.\n");
    Point *p_circle = new Circle(3.0, 4.0, 5.0);
    Point *p_point  = new Point(1.0, 2.0);

    // Вызов characteristic() через Point*:
    p_circle->characteristic();
    p_point->characteristic();

    // Вызов characteristic() через Point:: (явно базовый):
    p_circle->Point::characteristic();

    // 2. Проверка на принадлежность классу
    printf("\n2.\n");
    printf("classname() for p_circle: %s\n", p_circle->classname().c_str());
    printf("classname() for p_point:  %s\n", p_point->classname().c_str());
    printf("isA(\"Point\")  for p_circle: %s\n", p_circle->isA("Point")  ? "true" : "false");
    printf("isA(\"Circle\") for p_circle: %s\n", p_circle->isA("Circle") ? "true" : "false");
    printf("isA(\"Circle\") for p_point:  %s\n", p_point->isA("Circle")  ? "true" : "false");

    // 3. Опасное приведение типов
    printf("\n3. static_cast\n");
    Circle *dangerous = static_cast<Circle*>(p_point); // p_point - это Point
    // Привели Point* к Circle*, у Point нет radius, соответственно вызов somefunc() - это неопределённое поведение
    // dangerous->somefunc(); // Нерабочая строка


    // 4. Безопасное приведение через dynamic_cast
    printf("\n4. dynamic_cast\n");
    Circle *safe = dynamic_cast<Circle*>(p_circle);
    if (safe) {
        printf("dynamic_cast<Circle*>(p_circle) success\n");
        safe->somefunc();
    } else {
        printf("safe = nullptr\n");
    }

    Circle *safe2 = dynamic_cast<Circle*>(p_point);
    if (safe2) {
        printf("dynamic_cast<Circle*>(p_point) succeeded\n");
    } else {
        // dynamic_cast<Circle*>(p_point) вернул nullptr, т.к. это Point, не Circle
        printf("dynamic_cast<Circle*>(p_point) = nullptr\n");
    }

    // 5. Безопасное приведение вручную через isA
    printf("\n5. isA\n");
    if (p_circle->isA("Circle")) {
        Circle *manual = static_cast<Circle*>(p_circle);
        printf("Manual cast p_circle -> Circle* succeeded\n");
        manual->somefunc();
    }

    if (p_point->isA("Circle")) {
        // p_point isA Circle - этого не должно быть
        printf("no chance\n");
    } else {
        // p_point не является Circle - приведение отменено
        printf("p_point not Circle - cast cancel\n");
    }

    // 6. Виртуальный деструктор
    printf("\n6. \n");
    delete p_circle; //p_circle - указатель на базовый класс
    delete p_point;
    return 0;
}
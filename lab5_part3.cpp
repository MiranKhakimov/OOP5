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

// ==================== ФУНКЦИИ С УМНЫМИ УКАЗАТЕЛЯМИ ====================

// Принимает unique_ptr по значению (перемещение владения)
void take_unique(unique_ptr<Point> p) {
    printf(">>> take_unique: got unique_ptr\n");
    p->characteristic();
    printf("<<< take_unique: unique_ptr will be destroyed, object deleted\n");
}

// Принимает shared_ptr по значению (увеличивает счётчик)
void take_shared(shared_ptr<Point> p) {
    printf(">>> take_shared: got shared_ptr, use_count = %ld\n", p.use_count());
    p->characteristic();
    printf("<<< take_shared: shared_ptr destroyed, use_count will decrease\n");
}

// Возвращает unique_ptr
unique_ptr<Point> make_unique_point() {
    printf("make_unique_point: creating object via make_unique\n");
    return make_unique<Point>(50.0, 60.0);
}

// Возвращает shared_ptr
shared_ptr<Point> make_shared_point() {
    printf("make_shared_point: creating object via make_shared\n");
    return make_shared<Point>(51.0, 61.0);
}

// ==================== ДЕМОНСТРАЦИЯ ====================

int main() {
    SetConsoleOutputCP(CP_UTF8);

    printf("LR5. Part 3: Smart pointers\n\n");

    // unique_ptr
    printf("=== unique_ptr ===\n");
    {
        unique_ptr<Point> up1 = make_unique<Point>(60.0, 60.0);
        up1->characteristic();

        // Передача в функцию (перемещение владения)
        printf("\nPassing up1 to take_unique (ownership is moved):\n");
        take_unique(move(up1));
        // up1 теперь пуст!
        printf("up1 after transfer: %s\n", up1 ? "not empty" : "empty (nullptr)");
    }
    // Блок с up1 завершён, объект удалён

    // unique_ptr с полиморфным объектом (Circle через Point)
    printf("\n=== unique_ptr with polymorphic object (Circle) ===\n");
    {
        unique_ptr<Point> up2 = make_unique<Circle>(10.0, 20.0, 5.0);
        up2->characteristic(); // вызовется Circle::characteristic
        printf("classname: %s\n", up2->classname().c_str());
        printf("isA(\"Point\"): %s\n", up2->isA("Point") ? "true" : "false");
    }

    // shared_ptr
    printf("\n=== shared_ptr ===\n");
    {
        shared_ptr<Point> sp1 = make_shared<Point>(70.0, 70.0);
        printf("sp1 use_count = %ld\n", sp1.use_count());

        shared_ptr<Point> sp2 = sp1; // Копирование - счётчик растёт
        printf("After sp2 = sp1: use_count = %ld\n", sp1.use_count());

        printf("\nPassing sp1 to take_shared:\n");
        take_shared(sp1);
        printf("After take_shared: use_count = %ld\n", sp1.use_count());

        printf("\nResetting sp2:\n");
        sp2.reset();
        printf("After sp2.reset(): use_count = %ld\n", sp1.use_count());
    }
    // Блок с sp1 завершён, объект удалён

    // shared_ptr с полиморфным объектом
    printf("\n=== shared_ptr with polymorphic object (Circle) ===\n");
    {
        shared_ptr<Point> sp3 = make_shared<Circle>(1.0, 2.0, 3.0);
        sp3->characteristic();
        shared_ptr<Point> sp4 = sp3;
        printf("sp3/sp4 use_count = %ld\n", sp3.use_count());
    }

    // Сравнение с обычным указателем
    printf("\n=== Comparison: raw pointer vs unique_ptr ===\n");
    {
        Point *raw = new Point(80.0, 80.0);
        // Обычный указатель создан. Если забыть delete - УТЕЧКА
        printf("Raw pointer created. If forget delete - MEMORY LEAK\n");
        delete raw; // ручное удаление
        printf("Raw pointer deleted manually\n");
    }

    // unique_ptr с массивом
    printf("\n=== unique_ptr with array ===\n");
    {
        unique_ptr<Point[]> arr = make_unique<Point[]>(3);
        printf("Array of 3 Point created via unique_ptr\n");
        for (int i = 0; i < 3; i++) {
            arr[i].characteristic();
        }
    }
    // Массив удалён автоматически

    // Возврат из функций
    printf("\n=== Returning smart pointers from functions ===\n");
    {
        unique_ptr<Point> up_ret = make_unique_point();
        up_ret->characteristic();

        shared_ptr<Point> sp_ret = make_shared_point();
        sp_ret->characteristic();
        printf("sp_ret use_count = %ld\n", sp_ret.use_count());
    }
    // Умные указатели из функций уничтожены

    printf("\nEnd of code\n");
    return 0;
}
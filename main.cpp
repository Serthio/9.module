#include <iostream>
#include <memory>
#include <cmath>
#include <cassert>

class Vector3D {
private:
    std::unique_ptr<double[]> coords; // Умный указатель вместо сырого

public:
    // Конструктор по умолчанию
    Vector3D() : coords(std::make_unique<double[]>(3)) {
        coords[0] = 0.0;
        coords[1] = 0.0;
        coords[2] = 0.0;
        std::cout << "Vector3D default constructor" << std::endl;
    }

    // Параметрический конструктор
    Vector3D(double x, double y, double z) : coords(std::make_unique<double[]>(3)) {
        coords[0] = x;
        coords[1] = y;
        coords[2] = z;
        std::cout << "Vector3D parameterized constructor (" << x << ", " << y << ", " << z << ")" << std::endl;
    }

    // Конструктор копирования
    Vector3D(const Vector3D& other) : coords(std::make_unique<double[]>(3)) {
        coords[0] = other.coords[0];
        coords[1] = other.coords[1];
        coords[2] = other.coords[2];
        std::cout << "Vector3D copy constructor" << std::endl;
    }

    // Оператор присваивания копированием
    Vector3D& operator=(const Vector3D& other) {
        if (this != &other) {
            coords[0] = other.coords[0];
            coords[1] = other.coords[1];
            coords[2] = other.coords[2];
        }
        std::cout << "Vector3D copy assignment operator" << std::endl;
        return *this;
    }

    // Конструктор перемещения (noexcept для оптимизации)
    Vector3D(Vector3D&& other) noexcept : coords(std::move(other.coords)) {
        // После перемещения other.coords становится nullptr
        std::cout << "Vector3D move constructor" << std::endl;
    }

    // Оператор присваивания перемещением (noexcept для оптимизации)
    Vector3D& operator=(Vector3D&& other) noexcept {
        if (this != &other) {
            coords = std::move(other.coords);
        }
        std::cout << "Vector3D move assignment operator" << std::endl;
        return *this;
    }

    // Деструктор (не нужен явно благодаря unique_ptr, но оставим для демонстрации)
    ~Vector3D() {
        std::cout << "Vector3D destructor";
        if (coords) {
            std::cout << " - coordinates: (" << coords[0] << ", " << coords[1] << ", " << coords[2] << ")";
        }
        std::cout << std::endl;
    }

    // Методы доступа к координатам
    double x() const { return coords[0]; }
    double y() const { return coords[1]; }
    double z() const { return coords[2]; }

    void setX(double x) { coords[0] = x; }
    void setY(double y) { coords[1] = y; }
    void setZ(double z) { coords[2] = z; }

    // Вычисление длины вектора
    double length() const {
        return std::sqrt(coords[0] * coords[0] + 
                        coords[1] * coords[1] + 
                        coords[2] * coords[2]);
    }

    // Вывод вектора
    void print() const {
        std::cout << "(" << coords[0] << ", " << coords[1] << ", " << coords[2] << ")";
    }
};

// Функция для нормализации вектора (принимает shared_ptr)
void normalizeVector(std::shared_ptr<Vector3D> vec) {
    if (!vec) {
        std::cout << "Null pointer passed to normalizeVector" << std::endl;
        return;
    }

    double len = vec->length();
    if (len > 0) {
        vec->setX(vec->x() / len);
        vec->setY(vec->y() / len);
        vec->setZ(vec->z() / len);
        std::cout << "Normalized vector: ";
        vec->print();
        std::cout << std::endl;
    }
}

// Функция-фабрика для создания unique_ptr
std::unique_ptr<Vector3D> createVector(double x, double y, double z) {
    return std::make_unique<Vector3D>(x, y, z);
}

// Функция, демонстрирующая передачу владения
std::unique_ptr<Vector3D> transferOwnership(std::unique_ptr<Vector3D> vec) {
    if (vec) {
        vec->setX(vec->x() * 2); // Удваиваем x для демонстрации
    }
    return vec; // Передача владения обратно
}

int main() {
    std::cout << "=== ДDemonstration of working with Vector3D ===" << std::endl << std::endl;

    // Тест 1: Конструкторы и деструкторы
    std::cout << "1. Creating objects:" << std::endl;
    {
        Vector3D v1;
        Vector3D v2(1.0, 2.0, 3.0);
    }
    std::cout << std::endl;

    // Тест 2: Копирование
    std::cout << "2. Copy testing:" << std::endl;
    {
        Vector3D original(4.0, 5.0, 6.0);
        Vector3D copy = original; // Конструктор копирования
        Vector3D anotherCopy;
        anotherCopy = original; // Оператор присваивания копированием
    }
    std::cout << std::endl;

    // Тест 3: Перемещение
    std::cout << "3. Movement testing:" << std::endl;
    {
        Vector3D source(7.0, 8.0, 9.0);
        Vector3D moved = std::move(source); // Конструктор перемещения
        
        Vector3D target;
        target = Vector3D(10.0, 11.0, 12.0); // Оператор присваивания перемещением
    }
    std::cout << std::endl;

    // Тест 4: Умные указатели - unique_ptr
    std::cout << "4. Working with the unique_ptr:" << std::endl;
    {
        auto uniqueVec = std::make_unique<Vector3D>(13.0, 14.0, 15.0);
        std::cout << "Original vector: ";
        uniqueVec->print();
        std::cout << std::endl;

        // Передача владения
        auto newOwner = transferOwnership(std::move(uniqueVec));
        std::cout << "After ownership transfer: ";
        if (newOwner) {
            newOwner->print();
            std::cout << std::endl;
        }

        // uniqueVec теперь nullptr - проверяем
        if (!uniqueVec) {
            std::cout << "Original pointer is now null (ownership transferred)" << std::endl;
        }
    }
    std::cout << std::endl;

    // Тест 5: Умные указатели - shared_ptr
    std::cout << "5. Working with shared_ptr:" << std::endl;
    {
        auto sharedVec = std::make_shared<Vector3D>(3.0, 4.0, 0.0);
        std::cout << "Original vector: ";
        sharedVec->print();
        std::cout << ", length: " << sharedVec->length() << std::endl;

        // Создаем несколько владельцев
        auto sharedVec2 = sharedVec;
        std::cout << "Use count after copying: " << sharedVec.use_count() << std::endl;

        // Нормализация через shared_ptr
        normalizeVector(sharedVec);
        
        std::cout << "Use count before scope end: " << sharedVec.use_count() << std::endl;
    }
    std::cout << std::endl;

    // Тест 6: Фабричный метод с unique_ptr
    std::cout << "6. Factory method:" << std::endl;
    {
        auto factoryVec = createVector(20.0, 21.0, 22.0);
        factoryVec->print();
        std::cout << std::endl;
    }
    std::cout << std::endl;

    std::cout << "=== All tests are completed ===" << std::endl;
    return 0;
}
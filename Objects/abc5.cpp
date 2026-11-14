#include <iostream>
#include <sstream>
#include <iomanip>
#include <cstdint>

#include "s_A.h"
#include "s_B.h"

// class s_A {
//     public:

//         s_A() : a(50), b(100) {
//             std::cout << "Constructor A called, initializing resources..." << std::endl;
//             a = 50;
//             b = 100;
//         } // public constructor

//         ~s_A() {
//             std::cout << "Destructor A called, cleaning up resources..." << std::endl;
//         } // public destructor


//     public:
//         int a;
//         int b;

//         void set_a(int value);
//         void set_b(int value);
//         int get_a();
//         int get_b();
//         int Sum();

//     private:
//         int pSum();
// }; // class s_A;

// // Сеттеры и геттеры для работы с переменными структуры s_A
// void s_A::set_a(int value) {
//     std::cout << "Setting A.a to absolute value of " << value << std::endl;
//     a = abs(value);
// }
// void s_A::set_b(int value) {
//     std::cout << "Setting A.b to absolute value of " << value << std::endl;
//     b = abs(value);
// }
// int s_A::get_a() {
//     std::cout << "Getting A.a: " << a << std::endl;
//     return a;
// }
// int s_A::get_b() {
//     std::cout << "Getting A.b: " << b << std::endl;
//     return b;
// }

// // Сумма структуры s_A
// int s_A::Sum(){
//     std::cout << "Calculating sum of A: " << a << " + " << b << std::endl;
//     return pSum();
// }

// int s_A::pSum(){
//     std::cout << "Calculating private sum of A: " << a << " + " << b << std::endl;
//     return a + b;
// }
// ---------------------------


// class s_B {
//     public:
//         s_B() : a(50), b(100) {
//             std::cout << "Constructor B called, initializing resources..." << std::endl;
//             a = -50;
//             b = -100;
//         } // public constructor

//         ~s_B() {
//             std::cout << "Destructor B called, cleaning up resources..." << std::endl;
//         } // public destructor

//     public:
//         int a;
//         int b;
//         void set_a(int value);
//         void set_b(int value);
//         int get_a();
//         int get_b();
//         int Sum();
//     private:
//         int pSum();

// }; // class s_B;

// // Сеттеры и геттеры для работы с переменными структуры s_B
// void s_B::set_a(int value) {
//     std::cout << "Setting B.a to negative absolute value of " << value << std::endl;
//     a = -abs(value);
// }
// void s_B::set_b(int value) {
//     std::cout << "Setting B.b to negative absolute value of " << value << std::endl;
//     b = -abs(value);
// }
// int s_B::get_a() {
//     std::cout << "Getting B.a: " << a << std::endl;
//     return a;
// };
// int s_B::get_b() {
//     std::cout << "Getting B.b: " << b << std::endl;
//     return b;
// };

// // Сумма структуры s_B
// int s_B::Sum(){
//     std::cout << "Calculating sum of B: " << a << " + " << b << std::endl;
//     return this->pSum();
// }

// int s_B::pSum(){
//     std::cout << "Calculating private sum of B: " << a << " + " << b << std::endl;
//     return this->a + this->b;
// }


// ---------------------------

int main() {
    s_A objA;
    s_B objB;
    s_A objC; // Создаем объект класса A -- для того, чтобы показать,
              // что объектов одного класса может быть несколько,
              // и каждый объект имеет свои собственные данные.

    std::cout << "objA.a: " << objA.get_a() << ", objA.b: " << objA.get_b() << '\n';
    std::cout << "a + b = " << objA.Sum() << std::endl;

    objA.set_a(5);
    objA.set_b(10);

    std::cout << "objA.a: " << objA.get_a() << ", objA.b: " << objA.get_b() << '\n';
    std::cout << "a + b = " << objA.Sum() << std::endl;

    std::cout << "objB.a: " << objB.get_a() << ", objB.b: " << objB.get_b() << '\n';
    std::cout << "a + b = " << objB.Sum() << std::endl;

    std::cout << "objC.a: " << objC.get_a() << ", objC.b: " << objC.get_b() << '\n';
    std::cout << "a + b = " << objC.Sum() << std::endl;

    std::cout << "Hello, World!" << std::endl;
    return 0;
}

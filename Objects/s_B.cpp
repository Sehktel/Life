#include <iostream>
#include "s_B.h"

// Сеттеры и геттеры для работы с переменными структуры s_B
void s_B::set_a(int value) {
    std::cout << "Setting B.a to negative absolute value of " << value << std::endl;
    a = -abs(value);
}

void s_B::set_b(int value) {
    std::cout << "Setting B.b to negative absolute value of " << value << std::endl;
    b = -abs(value);
}

int s_B::get_a() {
    std::cout << "Getting B.a: " << a << std::endl;
    return a;
};

int s_B::get_b() {
    std::cout << "Getting B.b: " << b << std::endl;
    return b;
};

// Сумма структуры s_B
int s_B::Sum(){
    std::cout << "Calculating sum of B: " << a << " + " << b << std::endl;
    return this->pSum();
}

int s_B::pSum(){
    std::cout << "Calculating private sum of B: " << a << " + " << b << std::endl;
    return this->a + this->b;
}
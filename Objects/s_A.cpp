#include "s_A.h"

// Сеттеры и геттеры для работы с переменными структуры s_A
void s_A::set_a(int value) {
    std::cout << "Setting A.a to absolute value of " << value << std::endl;
    a = abs(value);
}

void s_A::set_b(int value) {
    std::cout << "Setting A.b to absolute value of " << value << std::endl;
    b = abs(value);
}

int s_A::get_a() {
    std::cout << "Getting A.a: " << a << std::endl;
    return a;
}

int s_A::get_b() {
    std::cout << "Getting A.b: " << b << std::endl;
    return b;
}

// Сумма структуры s_A
int s_A::Sum(){
    std::cout << "Calculating sum of A: " << a << " + " << b << std::endl;
    return pSum();
}

int s_A::pSum(){
    std::cout << "Calculating private sum of A: " << a << " + " << b << std::endl;
    return a + b;
}

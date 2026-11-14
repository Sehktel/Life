
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cstdint>

// заголовки функций для работы с переменными структуры s_A
void set_a(int value);
void set_b(int value);
int get_a();
int get_b();

struct s_A {
    int a;
    int b;
};

s_A varA;

// Сеттеры и геттеры для работы с переменными структуры s_A
void set_a(int value) {
    varA.a = value;
};
void set_b(int value) {
    varA.b = value;
};
int get_a() {
    return varA.a;
};
int get_b() {
    return varA.b;
};

// Сумма структуры s_A
int Sum_s_A(){
    return get_a() + get_b();
}

// ---------------------------

int main() {
    // varA.a = 5;
    // varA.b = 10;
    set_a(5);
    set_b(10);

    std::cout << "\nHex значение структуры в памяти: 0x"
         << std::hex
         << std::uppercase << std::setfill('0') << std::setw(16)
         << *reinterpret_cast<uint64_t*>(&varA)
         << std::dec
         << std::endl << std::endl;

    std::cout << "varA.a: " << get_a() << ", varA.b: " << get_b() << '\n';
    std::cout << "Size of s_A: " << sizeof(varA) << " bytes" << '\n';
    std::cout << "a + b = " << Sum_s_A() << std::endl;

    std::cout << "Hello, World!" << std::endl;
    return 0;
}

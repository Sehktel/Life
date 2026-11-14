
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cstdint>

struct s_A {
    int a;
    int b;
};

int main() {
    s_A varA;

    std::stringstream ss;

    varA.a = 5;
    varA.b = 10;

    // cout    " << " -- это потоковый оператор.
    //  "  < некоторый буффер  > <  Что-то 2>  < Что-то 3> и тд " << '\n'
    // std::endl = '\n' + flush() -- это манипулятор, который добавляет символ новой строки и очищает буфер вывода.
    // flush() -- те отчищает буффер вывода.
    //  "  < некоторый буффер  > <  Что-то 2>  < Что-то 3> и тд " << '\n'

    std::cout << "\nHex значение структуры в памяти: 0x"
         << std::hex
         << std::uppercase << std::setfill('0') << std::setw(16)
         << *reinterpret_cast<uint64_t*>(&varA)
         // << std::dec
         << std::endl << std::endl;


    std::cout << "varA.a: " << varA.a << ", varA.b: " << varA.b << '\n';
    std::cout << "Size of s_A: " << sizeof(varA) << " bytes" << '\n';
    std::cout << "a + b = " << varA.a + varA.b << std::endl;

    ss << "varA.a: " << varA.a << ", varA.b: " << varA.b << '\n';
    std::cout << ss.str() ;

    std::cout << "Hello, World!" << std::endl;
    return 0;
}

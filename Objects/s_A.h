#pragma once // header guard

#include <iostream>

class s_A {
    public:

        s_A() : a(50), b(100) {
            std::cout << "Constructor A called, initializing resources..." << std::endl;
            a = 50;
            b = 100;
        } // public constructor

        ~s_A() {
            std::cout << "Destructor A called, cleaning up resources..." << std::endl;
        } // public destructor


    public:
        int a;
        int b;

        void set_a(int value);
        void set_b(int value);
        int get_a();
        int get_b();
        int Sum();

    private:
        int pSum();
}; // class s_A;

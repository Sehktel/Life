#pragma once // header guard

class s_B {
    public:
        s_B() : a(50), b(100) {
            std::cout << "Constructor B called, initializing resources..." << std::endl;
            a = -50;
            b = -100;
        } // public constructor

        ~s_B() {
            std::cout << "Destructor B called, cleaning up resources..." << std::endl;
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

}; // class s_B;

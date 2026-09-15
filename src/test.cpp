#include <iostream>
using namespace std;

struct animal{
    virtual ~animal() = default;
};

struct dog : animal{

};

int main(){
    animal* a;
    dog brux;
    a = &brux;
    cout << (typeid(*a).name()) << "\n";
}
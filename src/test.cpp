#include <iostream>
#include <string>
#include <sstream>

int main(){
    std::string s{"process running 1"};
    std::string key, value;
    std::istringstream linestream(s);
    linestream >> key;
    std::cout << key << '\n';
    linestream >> key;
    std::cout << key << '\n';
}
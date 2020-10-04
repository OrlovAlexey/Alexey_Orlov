#include <iostream>
#include <string>

int main() {
    std::string s;
    int st[10000];
    std::cin >> s;
    int k=0;
    while(s != "exit") {
        if(s == "push") {
            int n;
            std::cin >> n;
            k++;
            st[k-1] = n;
            std::cout << "ok" << std::endl;
        }
        if(s == "pop") {
            if(!(k == 0)) {
                std::cout << st[k-1] << std::endl;
                k--;
            }
            else{
                std::cout << "error" << std::endl;
            }
        }
        if(s == "back") {
            if(!(k == 0)) {
                std::cout << st[k-1] << std::endl;
            }
            else{
                std::cout << "error" << std::endl;
            }
        }
        if(s == "size") {
            std::cout << k << std::endl;
        }
        if(s == "clear") {
            while(!(k == 0)) {
                k--;
            }
            std::cout << "ok" << std::endl;
        }
        std::cin >> s;
    }
    std::cout << "bye" << std::endl;
    return 0;
}

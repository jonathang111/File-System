#include "DirectoryRead/FileRead.h"

int main(){
    std::cout << "starting read..." << std::endl;
    Foldirs test("/Library");
    std::cout << "done reading" << std::endl;
    //test.PrintDatabase();
    std::cout << "Files stored: ";
    test.PrintSize();
    std:: cout << std::endl;
}
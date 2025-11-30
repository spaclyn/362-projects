#include <iostream> // inpute-output stream

int main() {
    std::cout << "Hello, World" << std::endl; // Print to the console
    return 0; //indicate that the program ended successfully
}

int age = 20; // interger variable for whole numbers;
double gpa = 3.5; // double variable for decimal numbers;
char grade = 'A'; // char variable for single characters;
bool isMale = true; // bool variable for true or false values
std::string name = "John"; // string variable for text

#include <iostream>
#include <string>

int main() {
    std::string name;
    int age;

    std::cout << "Enter your name: ";
    std::cin >> name; //inputs name from user
    std::cout << "Enter your age: ";
    std::cin >> age; //inputs age from user

    std::cout << "Hello " << name << ", you are " << age << " years old." << std::endl;
    return 0;
}



#include <iostream>
#include <string>
#include <algorithm> // for std::max

int main() {
    int testScore;
    int gradePoint;
    std::string name;

    std::cout << "Enter your name: ";
    std::cin >> name;
    std::cout << "Enter your test score: ";
    std::cin >> testScore;
    std::cout << "Enter your grade point: ";
    std::cin >> gradePoint;

    std::cout << "Hello " << name << ", your test score is " << testScore << " and your grade point is " << gradePoint << "." << std::endl;
    std::cout << "If you add your test score and grade point, you get " <<  (testScore + gradePoint) << "." << std::endl;
    std::cout << "If you subtract your test score from your grade point, you get " << (gradePoint - testScore) << "." << std::endl;
    std::cout << "If you multiply your test score and grade point, you get " << (testScore * gradePoint) << "." << std::endl;
    std::cout << "your largest score is " << std::max(testScore, gradePoint) << "." << std::endl;
    std::cout << "If you got below 60, you are failing. You are " << (testScore < 60 ? "failing." : "passing.") << std::endl;
    return 0;
}
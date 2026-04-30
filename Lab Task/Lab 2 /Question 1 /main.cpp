#include <iostream> 
using namespace std; 
 
int add(int, int); 
int subtract(int, int); 
int multiply(int, int); 
 
int main(){ 
    int choice, num1, num2; 
 
    cout << "Choose an operation:\n"; 
    cout << "1. Add\n"; 
    cout << "2. Subtract\n"; 
    cout << "3. Multiply\n"; 
    cout << "Enter choice: "; 
    cin >> choice; 
 
    cout << "Enter two numbers: "; 
    cin >> num1 >> num2; 
 
    switch (choice) 
    { 
        case 1: 
            cout << "Result: " << add(num1, num2); 
            break; 
        case 2: 
            cout << "Result: " << subtract(num1, num2); 
            break; 
        case 3: 
            cout << "Result: " << multiply(num1, num2); 
            break; 
        default: 
            cout << "Invalid choice!"; 
    } 
 
    return 0; 
} 

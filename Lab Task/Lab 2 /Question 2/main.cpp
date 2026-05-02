#include <iostream> 
#include "convert.h" 
using namespace std; 
int main() 
{ 
double celsius, fahrenheit; 
cout << "Enter temperature in Celsius: "; 
cin >> celsius; 
fahrenheit = celsiusToFahrenheit(celsius); 
cout << "Temperature in Fahrenheit: " << fahrenheit << endl; 
return 0; 
} 

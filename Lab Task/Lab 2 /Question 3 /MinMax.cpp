#include <iostream> 
#include <cstdlib>    
#include <cctype>     
using namespace std; 
 
bool isNumber(const char* str) 
{ 
    int i = 0; 
    if(str[0] == '-' || str[0] == '+') i = 1; 
    for(; str[i] != '\0'; i++) 
        if(!isdigit(str[i])) 
            return false; 
    return true; 
} 
 
int main(int argc, char* argv[]) 
{ 
 
    if(argc < 2) 
    { 
        cout << "Error: No numbers provided!" << endl; 
        cout << "Usage: ./minmax num1 num2 num3 ..." << endl; 
        return 1; 
    } 
 
    int arr[argc - 1]; 
 
    for(int i = 1; i < argc; i++) 
    { 
        if(!isNumber(argv[i])) 
        { 
            cout << "Error: Invalid input '" << argv[i] << "'" << 
endl; 
            return 1; 
        } 
        arr[i - 1] = atoi(argv[i]); 
    } 
 
 
    int min = arr[0], max = arr[0]; 
    for(int i = 1; i < argc - 1; i++) 
    { 
        if(arr[i] < min) min = arr[i]; 
        if(arr[i] > max) max = arr[i]; 
    } 
 
    cout << "Minimum: " << min << endl; 
    cout << "Maximum: " << max << endl; 
 
    return 0; 
} 

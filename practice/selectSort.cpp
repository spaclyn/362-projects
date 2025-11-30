#include <iostream>
#include <vector>
using namespace std;

void SelectionSort(vector<int>& arr) {
    int n = arr.size();
    
    for (int i = 0; i < n - 1; i++) {
        int minInd = i;
        
        //min element in unsorted array
        for (int j = i + 1; j < n; j++) {
            if (arr[j] < arr[minInd]) {
                minInd = j;
            }
        }
        
        //swap min and first element
        if (minInd != i) {
            swap(arr[i], arr[minInd]);
        }
    }
}

int main() {
    int n;
    while (true) {
        //array size input
        cout << "Enter the number of elements (n): ";
        cin >> n;
    
        //input validation
        if (n <= 0 || n > 10) {
            cout << "Error: Array size must be between 1 and 10" << endl;
            // return 1;
        } else {
            break;
        }
    }

    
    //maximum value constraint
    const int maxVal = 10000;
    
    vector<int> arr(n);
    
    //array elements input
    cout << "Enter " << n << " integers (values between -" << maxVal << " and " << maxVal << "):" << endl;
    for (int i = 0; i < n; i++) {
        cin >> arr[i];
        
        //value range validation
        if (arr[i] < -maxVal || arr[i] > maxVal) {
            cout << "Warning: Value " << arr[i] << " is outside the valid range! Consider revising." << endl;
        }
    }
    
    //selection sort call
    SelectionSort(arr);
    
    //output sorted array
    cout << "\nSorted array in ascending order:" << endl;
    for (int i = 0; i < n; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
    
    return 0;
}
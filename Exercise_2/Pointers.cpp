#include <iostream>
#include <stdio.h>

using namespace std;

void swap_classic(int* x, int* y) {
	int tmp = *x;
	*x = *y;
	*y = tmp;
}

void swap_modern(int& x, int& y) { // x and y are passed by parameters. The parameters are references to variables (5 or 7)
	// Everything done to x and y in the function will persist. 
	int tmp = x;
	x = y;
	y = tmp;
}

void swap_modern2(int& x, int& y) { // x and y are passed by parameters. The parameters are references to variables (5 or 7)
	// Everything done to x and y in the function will persist. 
	x = 3000;
	y = 4000;
}

void swap_byValue(int x, int y) { // This can not be used as the parameters passed by value
	int tmp = x;
	x = y;
	y = tmp;
}

int * swap_byValue2(int x, int y) { // Here we are copying x and y into new addresses of arr[0] and arr[1]
	static int arr[2];
	int tmp = x;
	x = y;
	y = tmp;
	arr[0] = x;
	arr[1] = y;

	return arr;
}

int main() {

	int x = 5, y = 7;

	cout << "Address of x: " << &x << endl;
	cout << "Address of y: " << &y << endl;
	cout << "Value of x: " << x << endl;
	cout << "Value of y: " << y << endl;
	cout << " " << endl;
	//
	//swap_classic(&x, &y);

	//cout << "swap_classic" << endl;
	//cout << "Address of x: " << &x << endl;
	//cout << "Address of y: " << &y << endl;
	//cout << "Value of x: " << x << endl;
	//cout << "Value of y: " << y << endl;
	//cout << " " << endl;

	//swap_modern(x, y);

	//cout << "swap_modern" << endl;
	//cout << "Address of x: " << &x << endl;
	//cout << "Address of y: " << &y << endl;
	//cout << "Value of x: " << x << endl;
	//cout << "Value of y: " << y << endl;
	//cout << " " << endl;

	swap_byValue(x, y);

	cout << "swap_byValue" << endl;
	cout << "Address of x: " << &x << endl;
	cout << "Address of y: " << &y << endl;
	cout << "Value of x: " << x << endl;
	cout << "Value of y: " << y << endl;
	cout << " " << endl;

	int * pointer = swap_byValue2(x, y);

	cout << "swap_byValue2, returns pointer" << endl;
	cout << "Address of arr[0], (x): " << &pointer[0] << endl;
	cout << "Address of arr[1], (y): " << &pointer[1] << endl;
	cout << "Value of arr[0], (x): " << pointer[0] << endl;
	cout << "Value of arr[1], (y): " << pointer[1] << endl;
	cout << " " << endl;

	swap_modern2(x, y);

	cout << "swap_modern2" << endl;
	cout << "Address of x: " << &x << endl;
	cout << "Address of y: " << &y << endl;
	cout << "Value of x: " << x << endl;
	cout << "Value of y: " << y << endl;
	cout << " " << endl;

	//int x = -42, y = 301;

	//int *pointer1 = &x;
	//int *pointer2 = &y;
	////int* x = p;

	//cout << "Address of pointer1 (&pointer1): " << &pointer1 << endl;
	//cout << "Address of x (&x): " << &x << endl;

	//cout << "Content of pointer1 (*pointer1): " << *pointer1 << endl;
	////cout << "Content of *x (*x): " << *x << endl; //Does not work
	//cout << "Content of address of pointer1 (*&pointer1): " << *&pointer1 << endl;

	//*pointer1 = 90;
	//cout << "Address of pointer1 (&pointer1): " << &pointer1 << endl;
	//cout << "Address of x (&x): " << &x << endl;
	//cout << "Content of pointer1 (*pointer1): " << *pointer1 << endl;

	//cout << "int x: " << x << endl;

	//cout << 

	////cout << "Address of p (&p): " << &p << endl;
	////cout << "Contents of address p (*p): " << *p << endl;
	////cout << "p (*&p): " << *&p << endl;

}
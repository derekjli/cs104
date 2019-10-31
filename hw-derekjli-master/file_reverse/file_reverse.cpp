#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char *argv[])
{

    char x;

	if (argc<2){
		cout << "Please enter the name of the file you want to reverse." << endl;
		return 1;
	}

	ifstream myfile;
	myfile.open(argv[1]);

	if (myfile.fail()){
		cout << "failed to open file: " << argv[1] << endl;
		return -1;
	}

	int len;
	myfile >> len;

	char *og = new char[len];	
	myfile.read(og, len);

    while(myfile >> x)
        cout << x;

    //input original array in reverse into reversed array
	for (int i=0; i<len; i++){
		cout << og[len - 1 - i];
	}

	myfile.close();

	//deallocate memory of array pointed to by pointers
	delete[] og;

}

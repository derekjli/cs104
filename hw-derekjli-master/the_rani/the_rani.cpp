#include <fstream>
#include <iostream>
#include <string>
#include <cstring>
#include <sstream>

using namespace std;

class StringArray2D {
public:
	StringArray2D();
	~StringArray2D();

	StringArray2D(StringArray2D const& o);
	StringArray2D& operator=(StringArray2D const& o);

	void add();
	void add(int n, string const& str);
	void remove(int x, int y);

	string& at(int x, int y);

	int size();
	int size(int n);

private:
	/*
	This makes sure that memory at data[x][y] is allocated.
	*/
	void ensureLocation(int x, int y);
	/*
	This doesn't ensure that data[x][y] is valid but makes sure that data at row X is at least Y columns long
	*/
	void ensureSize(int x, int y);

	void destruct();

	int size_;
	int* sizes_;
	string** data_;
};

StringArray2D::StringArray2D()
	: size_(0), sizes_(NULL), data_(NULL) {}

StringArray2D::~StringArray2D() {
	destruct();
}

void StringArray2D::destruct() {
	delete[] sizes_;
	for (int i = 0; i < size_; ++i)
		delete[] data_[i];
	delete[] data_;
}

StringArray2D::StringArray2D(StringArray2D const& o) {
	*this = o;
}

//Assignment function
StringArray2D& StringArray2D::operator=(StringArray2D const& o) {
	if (this == &o) {
		return *this;
	}

	//Deallocate memory
	destruct();

	size_ = o.size_;
	sizes_ = new int[size_];

	data_ = new string * [size_];

	// Copy over the data
	for (int i = 0; i < size_; ++i) {
		sizes_[i] = o.sizes_[i];
		if (sizes_[i] > 0) {
			data_[i] = new string[sizes_[i]];

			for (int j = 0; j < sizes_[i]; ++j) {
				data_[i][j] = o.data_[i][j];
			}
		}
		else {
			sizes_[i] = 0;
		}
	}

	return *this;
}

string& StringArray2D::at(int x, int y) {
	ensureLocation(x, y);
	return data_[x][y];
}

// Adding a topmost dimension is pretty trivial
void StringArray2D::add() {
	ensureSize(this->size() + 1, 0);
}

// Adding to the second location isn't that hard either
void StringArray2D::add(int n, string const& str) {
	int last = this->size(n);
	ensureLocation(n, this->size(n));
	data_[n][last] = str;
}

// Just trivial getters
int StringArray2D::size() { return size_; }
int StringArray2D::size(int n) { ensureSize(n + 1, 0);  return sizes_[n]; }

//allocate new memory for some x,y element
void StringArray2D::ensureLocation(int x, int y) {
	if (x >= size_ && x > -1) {
		// We need to reallocate the outer dimension.
		string** newData = new string * [x + 1];
		// We need to copy the old data into this new array
		memcpy(newData, data_, sizeof(string*) * size_);
		// Set the rest to 0s
		memset(newData + size_, 0, sizeof(string*) * (x + 1 - size_));
		// Deallocate the old array
		delete[] data_;
		// Do the same for sizes
		int* newSizes = new int[x + 1];
		memcpy(newSizes, sizes_, sizeof(int) * size_);
		// Set the rest to 0s
		memset(newSizes + size_, 0, sizeof(int) * (x + 1 - size_));
		delete[] sizes_;
		// Swap it out
		data_ = newData;
		sizes_ = newSizes;
		size_ = x + 1;
	}
	if (y >= sizes_[x] && y > -1) {
		// We need to reallocate the inner dimension
		string* newData = new string[y + 1];
		// We can't use memcpy here because string is a C++ type! We need to copy manually
		for (int i = 0; i < this->size(x); ++i) newData[i] = data_[x][i];
		// Deallocate the old array
		delete[] data_[x];
		// Thankfully we don't need to do any trickery with sizes here, we just need to overwrite it
		sizes_[x] = y + 1;
		data_[x] = newData;
	}
}

void StringArray2D::ensureSize(int x, int y) {
	// We'll do some dirty reuse here ;)
	ensureLocation(x - 1, y - 1);
}

void StringArray2D::remove(int x, int y) {
	// We don't care about the first coord being too big
	// But we resize the second coord to be the tightest array possible
	string* oldArray = data_[x];
	// We create a new array with exactly one less size and copy everything but element y into it
	string* newArray = new string[sizes_[x] - 1];

	int offs = 0;
	for (int i = 0; i < sizes_[x]; ++i) {
		if (i != y) {
			newArray[offs] = oldArray[i];
			++offs;
		}
	}

	// Adjust values
	--sizes_[x];
	delete[] oldArray;
	data_[x] = newArray;
}

enum ReadError {
	NO_ERROR, NO_ARGUMENT, WRONG_TYPE,
};

// The actual solver
class TheRani {
public:
	TheRani(char const* inPath, char const* outPath);
	void execute();

private:
	void error(int line, char const* message);

	ReadError read_int(stringstream& ss, int& n);
	bool read_int_smart(int line, stringstream& ss, int& n);

	void start(int n);
	void add();
	void move(int x, int y, int n, int m);
	void query(int x, int n);

	StringArray2D subject_history;
	ifstream infile;
	ofstream outfile;
};

TheRani::TheRani(char const* inPath, char const* outPath)
	: infile(inPath), outfile(outPath) {
}

bool TheRani::read_int_smart(int line, stringstream& ss, int& n) {
	ReadError err = read_int(ss, n);

	if (err == NO_ARGUMENT) {
		error(line, "too few arguments");
		return true;
	}
	else if (err == WRONG_TYPE) {
		error(line, "expected integer argument");
		return true;
	}

	return false;
}

// Helper for reading an int
ReadError TheRani::read_int(stringstream& ss, int& n) {
	string piece;
	if (!(ss >> piece)) {
		return NO_ARGUMENT;
	}
	// This piece is either an int or we err out
	if (piece.find('.') != string::npos) {
		// The string contains a dot, must be a double
		return WRONG_TYPE;
	}

    //Check for non-int string values
	stringstream ss2;
	ss2 << piece;
	if (!(ss2 >> n)) {
		return WRONG_TYPE;
	}
	return NO_ERROR;
}

// Helper for errors
void TheRani::error(int line, char const* message) {
	outfile << "Error on line" << line << ":" << message;
	return;
}

void TheRani::execute() {
	int lineCounter = 1;

	string line;
	while (!getline(infile, line).eof())
	{
		stringstream ssline;
		ssline << line;

		string command;
		ssline >> command;

		//If something other than start is called on line 1
		if (lineCounter == 1 && command != "START") {
			error(lineCounter, "no subjects yet");
			return;
		}

		if (command == "START") {
			int n;

			if (read_int_smart(lineCounter, ssline, n)) {
				return;
			}

			if (n < 0) {
                error(lineCounter, "argument out of range");
			}

			start(n);
		}
		else if (command == "ADD") {
			add();
		}
		else if (command == "MOVE") {
			int x, y, n, m;

			if (read_int_smart(lineCounter, ssline, x)) {
				return;
			}
			if (read_int_smart(lineCounter, ssline, y)) {
				return;
			}
			if (read_int_smart(lineCounter, ssline, n)) {
				return;
			}
			if (read_int_smart(lineCounter, ssline, m)) {
				return;
			}

            //Out of range errors
			if (x < 0 || x >= subject_history.size()) {
                error(lineCounter, "argument out of range");
			}
			if (y < 0 || y >= subject_history.size()) {
                error(lineCounter, "argument out of range");
			}
			if (n > m) {
				error(lineCounter, "invalid range of subjects to move");
			}
			if (n < 0 || n >= subject_history.size(x)) {
                error(lineCounter, "argument out of range");
			}
			if (m < 0 || m >= subject_history.size(x)) {
                error(lineCounter, "argument out of range");
			}

			move(x, y, n, m);
		}
		else if (command == "QUERY") {
			int x, y;
			if (read_int_smart(lineCounter, ssline, x)) {
				return;
			}
			if (read_int_smart(lineCounter, ssline, y)) {
				return;
			}

            //Out of range errors
			if (x < 0 || x >= subject_history.size()) {
                error(lineCounter, "argument out of range");
			}
			if (y < 0 || y >= subject_history.size(x)) {
                error(lineCounter, "argument out of range");
			}

			query(x, y);
		}
		else {
            // Invalid command
			error(lineCounter, "command doesn't exist");
			return;
		}
		lineCounter++;
	}
}

//Create set of experiments with sizes and data array
//Populate subject pool
void TheRani::start(int n) {

	subject_history = StringArray2D();
	for (int i = 0; i < n; ++i) {
		subject_history.add(0, "");
	}
}

void TheRani::add() {
	subject_history.add();
}

void TheRani::move(int x, int y, int n, int m) {
	for (int i = n; i <= m; ++i) {
		// Move subject 'n' from experiment x to y
		// We move 'n' and not 'i' because the elements "shift" as we move them around
		// First we copy it
		string subject = subject_history.at(x, n);
		// Then we remove
		subject_history.remove(x, n);
		// Register the new experiment, if it's nonzero
		if (y != 0) {
			subject += " ";
			stringstream stream;
			stream << y;
			subject += stream.str();
		}
		// Add to the new one
		subject_history.add(y, subject);
	}
}

void TheRani::query(int x, int n) {



	string subject = subject_history.at(x, n);
	//if subject has been moved from 0, it will ahve a ' 4'
	//otherwise it wont' have anything.
	if (subject.length() > 0) {
		if (subject[0] == ' ') {
			// we cut down the space
			subject = subject.substr(1);
		}
	}
	// Output it
	outfile << subject << endl;
}

int main(int argc, char* argv[]) {
  if (argc < 3) {
      cerr << "Please provide an input and output file!" << endl;
      return 1;
  }
  TheRani tr(argv[1], argv[2]);
  tr.execute();
  return 0;
}


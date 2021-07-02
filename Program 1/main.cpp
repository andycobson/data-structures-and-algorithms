#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <chrono>
using namespace std;

/* Programmer: Andrew Jacobsson
 * Assignment: Progam 1
 * Class: 3110-021
 *
 * This progam reads in a csv file that contains information on child care centers and sorts it by name using
 * 	different sorting algorithms.
 *
 * My implementation looks for the name of the care center while reading the csv file row by row. Once the
 * 	name is found, I store the name and the orignal row data into a pair<string, string> container, which
 * 	then the pair container is stored in a vector. All the name comparisons on done using the first element
 * 	of the pair but when the position needs to change in the vector then the whole pair object is moved.
 */

void insertionSort(vector<pair<string, string>>&, int start, int end);
void merge(vector<pair<string, string>>&, int, int, int);
void mergeSort(vector<pair<string, string>>&, int, int);
void swap(vector<pair<string, string>>&, int, int);
int partition(vector<pair<string, string>>&, int, int);
void quickSort(vector<pair<string, string>>&, int, int);
void quickSortHybrid(vector<pair<string, string>>&, int, int);
int leftChild(int);
void perculate(vector<pair<string, string>>&, int, int);
void heapSort(vector<pair<string, string>>&);
void fillVector(vector<pair<string, string>>&, string, string*);
void exportVectorToCsv(vector<pair<string, string>>&, string*, string, string);

int main() {

	string fileName = "Child_Care_Centers.csv";
	vector<pair<string, string>> items;
	string columnHeaders = "";
	string* headptr = &columnHeaders;
	string sortType = "";
	fillVector(items, fileName, headptr);

	cout << "1 - Insertion Sort\n2 - Merge Sort\n3 - Quick Sort\n4 - Hybrid-Quick Sort\n5 - Heap Sort\nChoose your sorting algorithm: ";
	int choice;
	cin >> choice;


	//The vector is passed by reference to save space
	//
	auto t1 = chrono::high_resolution_clock::now();
	switch (choice) {
	case 1:
		sortType = "insertion";
		insertionSort(items, 0, items.size() - 1);
		break;
	case 2:
		sortType = "merge";
		mergeSort(items, 0, items.size() - 1);
		break;
	case 3:
		sortType = "quick";
		quickSort(items, 0, items.size() - 1);
		break;
	case 4:
		sortType = "hybrid";
		quickSortHybrid(items, 0, items.size() - 1);
		break;
	case 5:
		sortType = "heap";
		heapSort(items);
		break;
	}
	auto t2 = chrono::high_resolution_clock::now();
	auto ms_int = chrono::duration_cast<chrono::milliseconds>(t2 - t1);


	exportVectorToCsv(items, headptr, fileName, sortType);
	cout << "This ran in " << ms_int.count() << " milliseconds using sort method: " + sortType << endl;

	return 0;
}

void insertionSort(vector<pair<string, string>>& csvItems, int start, int end) {
	pair <string, string> key;
	int j;
	for (int i = start + 1; i < end + 1; i++) {
		key = csvItems.at(i);
		j = i - 1;
		while ((j >= start) && (csvItems.at(j).first.compare(key.first) > 0)) {
			csvItems.at(j + 1) = csvItems.at(j);
			j--;
		}
		csvItems.at(j + 1) = key;
	}
}


void merge(vector<pair<string, string>>& csvItems, int p, int q, int r) {
	int n1 = q - p + 1;
	int n2 = r - q;
	int i;
	int j;

	vector<pair<string, string>> L(n1, make_pair("", ""));
	vector<pair<string, string>> R(n1, make_pair("", ""));
	for (i = 0; i < n1; i++) {
		L.at(i) = csvItems.at(p + i);
	}
	for (j = 0; j < n2; j++) {
		R.at(j) = csvItems.at(q + 1 + j);
	}
	i = 0;
	j = 0;

	int k = p;
	while (i < n1 && j < n2) {
		if (L.at(i).first.compare(R.at(j).first) <= 0) {
			csvItems.at(k) = L.at(i);
			i++;
		}
		else {
			csvItems.at(k) = R.at(j);
			j++;
		}
		k++;
	}

	while (i < n1) {
		csvItems.at(k) = L.at(i);
		i++;
		k++;
	}
	while (j < n2) {
		csvItems.at(k) = R.at(j);;
		j++;
		k++;
	}
}

void mergeSort(vector<pair<string, string>>& csvItems, int p, int r) {
	if (p < r) {
		int q = p + (r - p) / 2;
		mergeSort(csvItems, p, q);
		mergeSort(csvItems, q + 1, r);
		merge(csvItems, p, q, r);
	}
}

void swap(vector<pair<string, string>>& csvItems, int i, int j) {
	pair<string, string> temp;
	temp = csvItems.at(i);
	csvItems.at(i) = csvItems.at(j);
	csvItems.at(j) = temp;
}

int partition(vector<pair<string, string>>& csvItems, int p, int r) {
	pair<string, string> x = csvItems.at(r);
	int i = p - 1;
	for (int j = p; j < r; j++) {
		if (csvItems.at(j).first.compare(x.first) <= 0) {
			i++;
			swap(csvItems, i, j);
		}
		swap(csvItems, i + 1, r);
	}
	return i + 1;
}

void quickSort(vector<pair<string, string>>& csvItems, int p, int r) {
	if (p < r) {
		int q = partition(csvItems, p, r);
		quickSort(csvItems, p, q - 1);
		quickSort(csvItems, q + 1, r);
	}
}

void quickSortHybrid(vector<pair<string, string>>& csvItems, int p, int r) {
	if (p + 20 < r) {
		int q = partition(csvItems, p, r);
		quickSortHybrid(csvItems, p, q - 1);
		quickSortHybrid(csvItems, q + 1, r);
	}
	else {
		insertionSort(csvItems, p, r - p);
	}
}


int leftChild(int i) {
	return 2 * i + 1;
}

void perculate(vector<pair<string, string>>& csvItems, int i, int n) {
	int child;
	pair<string, string> temp;

	for (temp = csvItems.at(i); leftChild(i) < n; i = child) {
		child = leftChild(i);
		if (child != n - 1 && csvItems.at(child).first.compare(csvItems.at(child + 1).first) < 0) {
			++child;
		}
		if (temp.first.compare(csvItems.at(child).first) < 0) {
			csvItems.at(i) = csvItems.at(child);
		}
		else {
			break;
		}
	}
	csvItems.at(i) = temp;
}

void heapSort(vector<pair<string, string>>& csvItems) {
	for (int i = csvItems.size() / 2 - 1; i >= 0; --i) {
		perculate(csvItems, i, csvItems.size());
	}
	for (int j = csvItems.size() - 1; j > 0; --j) {
		swap(csvItems, 0, j);
		perculate(csvItems, 0, j);
	}
}

void fillVector(vector<pair<string, string>>& items, string fileName, string* columnHeaders) {
	ifstream inFile(fileName);
	if (!inFile.is_open()) {
		cout << "Opening file failed" << endl;
		return;
	}

	string record;
	int columnCounter;
	string recordName;
	string token;

	int rows = 2;
	if (inFile.good()) {
		//Store the column headers seperate from vector.
		getline(inFile, *columnHeaders);
		//Read the rest of the lines 
		while (getline(inFile, record)) {
			istringstream line(record);
			columnCounter = 1;
			recordName = "";
			int iteratorIndex = 0;
			int commaCounter = 0;
			bool quoteFound = false;
			string commaString = "";
			//Only want the data in the name column which is column 5 or if the name
			//	contains a comma then read line char by char to find a quote pair.

			// Use the find method to see it there is a quote in the row data.
			size_t found = record.find('"');
			if (found != string::npos) {
				// Quote found
				for (string::iterator it = record.begin(); it != record.end(); ++it) {
					switch (*it) {
					case '"':
						// A quote is caught, is there a comma on either side, if so this is a new field with a comma. 
						if (record.at(iteratorIndex - 1) == ',' || record.at(iteratorIndex + 1) == ',') {
							quoteFound = !quoteFound;
						}
						else {
							// A quote is inside a field. When this happens there is 3 pairs of quotes.
							if (record.at(iteratorIndex - 1) != '"')
								commaString.push_back('"');
						}
						if (quoteFound == false && commaCounter == 4) {
							//Comma counter is pretty much a collumn counter. We have reached the 5th field (index + 1) also we have finished
							//	building as string as we found the 2nd quote of the pair.
							recordName = commaString;
						}
						break;
					default:
						if (quoteFound == true) {
							// The 1st quote of the pair was found, build the name string character by character until the 2nd quote is detected.
							commaString.push_back(*it);
						}
						else {
							if (*it == ',') {
								// Again, this is more of a column counter.
								commaCounter++;
							}
						}
					}
					iteratorIndex += 1;
					if (commaCounter > 4 && recordName == "") {
						// A quote (comma) was found in the row string, but if it isnt the name column then just parse the the string normally.
						while (getline(line, token, ',')) {
							if (columnCounter == 5) {
								recordName = token;
								break;
							}
							columnCounter++;
						}
						break;
					}
					else if (commaCounter > 4 && recordName != "") {
						// If searching past the name column (commaCounter) and the name has been found then leave the loop.
						break;
					}
				}
			}
			else {
				// No quotes or commas were found so just use the normal delimit.
				while (getline(line, token, ',')) {
					if (columnCounter == 5) {
						recordName = token;
						break;
					}
					columnCounter++;
				}
			}
			items.push_back(make_pair(recordName, record));

		}
	}
}


void exportVectorToCsv(vector<pair<string, string>>& items, string* columnHeaders, string fileName, string st) {
	ofstream ofFile;
	fileName = "(" + st + "Export)" + fileName;
	// Create new sorted csv
	ofFile.open(fileName);
	ofFile << *columnHeaders << endl;
	for (int j = 0; j < items.size(); j++) {
		ofFile << items.at(j).second << endl;
	}
	ofFile.close();
}

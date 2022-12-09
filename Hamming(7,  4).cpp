#include <iostream>
#include <string>
#include <sstream>
#include <bitset>

using namespace std;

bool* S2BS(const string& ascii);
string BS2S(bool b[], int size);
bool* H74_encode(bool b[], int size);
bool* H74_decode(bool c[], int size);
void spoilBit(bool* c, int size);

int main() {
	srand(time(NULL));

	string msg = "amessage";
	int size = (int)msg.length() * 7;
	int size_h = (((int)msg.length() * 7) / 4) * 7;
	cout << "Msg: " << msg << endl << endl;

	bool* b = S2BS(msg);
	cout << "b = ";
	for (int i = 0; i < size; i++) {
		cout << b[i];
	}
	cout << endl << endl;


	bool* c = H74_encode(b, size_h);
	cout << "c = ";
	for (int i = 0; i < size_h; i++) {
		cout << c[i];
	}
	cout << endl;

	spoilBit(c, size_h);

	bool* b_prim = H74_decode(c, size_h);
	cout << "b_prim = ";
	for (int i = 0; i < size; i++) {
		cout << b_prim[i];
	}
	cout << endl << endl;


	cout << "Msg: " << BS2S(b_prim, size) << endl << endl;

	return 0;
}

bool* S2BS(const string& ascii) {
	ostringstream oss("");

	for (auto b : ascii) {
		oss << bitset<7>(b);
	}

	string s = oss.str();
	bool* b = new bool[s.length()];

	for (int i = 0; i < s.length(); i++) {
		b[i] = s[i] - '0';
	}

	return b;
}

string BS2S(bool b[], int size) {
	ostringstream oss("");

	for (int i = 0; i < size; i++) {
		oss << b[i];
	}

	stringstream ss(oss.str());
	string s = "";
	char c = ' ';

	while (ss.good()) {
		bitset<7> bs;
		ss >> bs;
		c = char(bs.to_ulong());
		s += c;
	}

	return s;
}

bool* H74_encode(bool b[], int size) {
	bool* c = new bool[size];
	bool temp[4];
	int i = 0;
	int k = 0;

	while (i < size && k < size) {
		for (int j = 0; j < 4; j++) {
			temp[j] = b[j + k];
		}

		c[i] = (temp[0] + temp[1] + temp[3]) % 2;
		c[i + 1] = (temp[0] + temp[2] + temp[3]) % 2;
		c[i + 2] = temp[0];
		c[i + 3] = (temp[1] + temp[2] + temp[3]) % 2;
		c[i + 4] = temp[1];
		c[i + 5] = temp[2];
		c[i + 6] = temp[3];

		i += 7;
		k += 4;
	}

	return c;
}

bool* H74_decode(bool c[], int size) {
	bool* b_prim = new bool[size];
	bool temp[7];
	int i = 0;
	int k = 0;

	bool x1_prim = 0;
	bool x2_prim = 0;
	bool x4_prim = 0;
	bool x1_S = 0;
	bool x2_S = 0;
	bool x4_S = 0;
	int S = 0;

	while (i < size && k < size) {
		for (int j = 0; j < 7; j++) {
			temp[j] = c[j + k];
		}

		x1_prim = (temp[2] + temp[4] + temp[6]) % 2;
		x2_prim = (temp[2] + temp[5] + temp[6]) % 2;
		x4_prim = (temp[4] + temp[5] + temp[6]) % 2;

		x1_S = (temp[0] + x1_prim) % 2;
		x2_S = (temp[1] + x2_prim) % 2;
		x4_S = (temp[3] + x4_prim) % 2;

		S = x1_S * pow(2, 0) + x2_S * pow(2, 1) + x4_S * pow(2, 2);

		if (S != 0) {
			temp[S - 1] = !temp[S - 1];
		}

		b_prim[i] = temp[2];
		b_prim[i + 1] = temp[4];
		b_prim[i + 2] = temp[5];
		b_prim[i + 3] = temp[6];

		i += 4;
		k += 7;
	}

	return b_prim;
}

void spoilBit(bool* c, int size) {
	int rng = (rand() % (size - 1)) + 0;
	c[rng] = !c[rng];

	cout << "c_s=";
	for (int i = 0; i < size; i++) {
		cout << c[i];
	}
	cout << endl << endl;
}
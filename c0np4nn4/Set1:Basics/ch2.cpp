#include <iostream>
#include <string>

using namespace std;

string hex_to_bytes(string& hexValue);
string fixed_xor(string& lValue, string& rValue); // left-Value, right-Value

int main() {
	string hxValue[2] = { "1c0111001f010100061a024b53535009181c", "686974207468652062756c6c277320657965" };
	
	hxValue[0] = hex_to_bytes(hxValue[0]);
	hxValue[1] = hex_to_bytes(hxValue[1]);
	
	string result = fixed_xor(hxValue[0], hxValue[1]);
	
	cout << result << endl;
	
	// check
//	string test = "746865206b696420646f6e277420706c6179";
//	cout << hex_to_bytes(test) << endl;
	return 0;
}

string hex_to_bytes(string& hexValue) {
	string result;

	for (int i = 0; i < hexValue.length(); i += 2) {
		int t1, t2;
		
		if ( hexValue[i] - 'a' < 0 ) { t1 = hexValue[i] - '0'; }
		else { t1 = hexValue[i] - 'a' + 10; }
		
		if ( hexValue[i + 1] - 'a' < 0 ) { t2 = hexValue[i + 1] - '0'; }
		else { t2 = hexValue[i + 1] - 'a' + 10; }
		
		result += (t1*16 + t2);
	}
	
	return result;
}

string fixed_xor(string& lValue, string& rValue) {
	unsigned int len = lValue.size();
	
	string ret = "";
	for (int i = 0; i < len; i++) {
		ret += (lValue[i] ^ rValue[i]);
	}
	
	return ret;
}

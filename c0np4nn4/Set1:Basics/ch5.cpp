#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

using namespace std;

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

string bytes_to_hex(string& byteStream) {
	const char hexChars[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };
	string ret = "";
	
	for (int i = 0; i < byteStream.size(); i++) {
		char nibble[2];
		nibble[0] = (byteStream[i] & 0xf0) >> 4;
		nibble[1] = (byteStream[i] & 0x0f);
		
		
		for (int j = 0; j < 2; j++) {
			ret += hexChars[nibble[j]];
		}
	}
	
	return ret;
}

string fixed_xor(string& lValue, string& rValue) {
	unsigned int len = lValue.size();
	
	string ret = "";
	for (int i = 0; i < len; i++) {
		ret += (lValue[i] ^ rValue[i]);
	}
	
	return ret;
}

string repeating_key_generator(string& key, const long long len) {
	string ret = "";
	
	for (int i = 0; i < (len / key.size()); i++) {
		ret += key;
	}
	
	for (int i = 0; i < (len % key.size()); i++) {
		ret += key[i];
	}
	
	return ret;
}

string encrypt_repeating_key_cipher(string& key, const string& fileName) {
	string ret;
	
	ifstream ifs(fileName);
	if (true == ifs.is_open()) {
		string tmpMsg;
		
		ifs.seekg(0, ios::end);
		long long size = ifs.tellg();
		size -= 1;
		tmpMsg.resize(size);
		ifs.seekg(0, ios::beg);
		ifs.read(&tmpMsg[0], size);
		
		string keyStream = repeating_key_generator(key, tmpMsg.size());
		tmpMsg = fixed_xor(tmpMsg, keyStream);
		
		ret = tmpMsg;
	}
	
	return ret;
}


int main() {
	string ct = "";
	string key = "ICE";
	string fileName = "msg.txt";
	
	ct = encrypt_repeating_key_cipher(key, fileName);
	
	cout << bytes_to_hex(ct) << endl;
	return 0;
}

/*
 * Ref.
 * [C++ File I/O] : https://computer-science-student.tistory.com/48
*/

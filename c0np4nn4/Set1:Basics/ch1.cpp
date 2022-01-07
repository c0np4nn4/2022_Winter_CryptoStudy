#include <iostream>
#include <string>

using namespace std;

string hex_to_bytes(string&);
string base64_encode(string&);
string base64_decode(string&);

int main() {
	string hexValue = "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d";
	string byteStream;
	
	byteStream = hex_to_bytes(hexValue);
	
	// -- check
//	for (int i = 0; i < byteStream.size(); i++) {
//		cout << byteStream[i];
//	}
	
	// char to base64 encoding
//	string result = base64_encode(byteStream);
//	cout << result << endl;
//	cout << base64_decode(result) << endl;
	
	
//	string t = "ab";
//	string test = base64_encode(t);
//	cout << test << endl;
//	cout << base64_decode(test) << endl;
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

string base64_encode(string& byteStream) {
	string result;
	string base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	unsigned char block[4] = { '\x00', };
	
	// when blocks are successively filled with byte.
	if (byteStream.length() % 3 == 0) {
		for (int i = 0; i < byteStream.length() - 2; i += 3) {		
			// manipulate
			block[0] = (byteStream[i] & 0xfc) >> 2;
			block[1] = ((byteStream[i] & 0x03) << 4 | (((byteStream[i+1] & 0xf0) >> 4) & 0x0f) );
			block[2] = ((byteStream[i+1] & 0x0f) << 2 | (byteStream[i+2] & 0xc0) >> 6);
			block[3] = byteStream[i+2] & 0x3f;
			
			for (int j = 0; j < 4; j++) {
				result += base64_chars[block[j]];
			}
		}
	}
	
	// with padding..
	else {
		unsigned char block_3[3] = { '\x00', };
		for (int j = 0; j < byteStream.length() % 3; j++) {
			block_3[j] = byteStream[j + (byteStream.length() / 3) * 3];
		}

		block[0] = (block_3[0] & 0xfc) >> 2;
		block[1] = ((block_3[0] & 0x03) << 4 | (((block_3[1] & 0xf0) >> 4) & 0x0f) );
		block[2] = ((block_3[1] & 0x0f) << 2 | (block_3[2] & 0xc0) >> 6);
		block[3] = block_3[2] & 0x3f;
		
		for (int j = 0; block[j] != '\x00'; j++) {
			result += base64_chars[block[j]];
		}
		
		for (int j = 0; j < (3 - byteStream.length() % 3); j++) {
			result += '=';
		}
	}
	return result;
}


string base64_decode(string& byteStream) {
	string result;
	string base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	
	unsigned char block_3[3] = {'\x00', };
	unsigned char block_4[4] = {'\x00', };
	
	if (byteStream.length() % 4 == 0) {
		for (int i = 0; i < byteStream.length() - 3; i += 4) {
			
			for (int j = 0; j < 64; j++) {
				if (base64_chars[j] == byteStream[i+0]) block_4[0] = j;
				if (base64_chars[j] == byteStream[i+1]) block_4[1] = j;
				if (base64_chars[j] == byteStream[i+2]) block_4[2] = j;
				if (base64_chars[j] == byteStream[i+3]) block_4[3] = j;
			}
			
			block_3[0] = (block_4[0] << 2) | ((block_4[1] & 0x30) >> 4);
			block_3[1] = ((block_4[1] & 0x0f) << 4) | ((block_4[2] & 0x3c) >> 2);
			block_3[2] = ((block_4[2] & 0x03) << 6) | (block_4[3]);
			
			for (int j = 0; j < 3; j++) {
				result += block_3[j];
			}
		}
	}
	
	return result;
}

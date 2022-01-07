#include <iostream>
#include <string>

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

string fixed_xor(string& lValue, string& rValue) {
	unsigned int len = lValue.size();
	
	string ret = "";
	for (int i = 0; i < len; i++) {
		ret += (lValue[i] ^ rValue[i]);
	}
	
	return ret;
}

double scoring_alphabets(string& text) {
	double scoreTable[26] = { 8.2, 1.5, 2.8, 4.3, 13.0, 2.2, 2, 6.1, 7, 0.15,
										0.77, 4, 2.5, 6.7, 7.5, 1.9, 0.095, 6, 6.3, 9.1,
										2.8, 0.98, 2.4, 0.15, 2, 0.074 };
	double score = 0.0;
	for (int i = 0; i < text.size(); i++) {
		if (text[i] >= 'A' && text[i] <= 'Z') score += scoreTable[text[i] - 'A'];
		else if (text[i] >= 'a' && text[i] <= 'z') score += scoreTable[text[i] - 'a'];
		else if (text[i] == ' ') score += 20.0;
	}
	
	return score;
}

string recover_single_byte_key_cipher(string& cipherText) {
	
	double maxScore = 0.0;
	string pt = "";
	string ct = "";
	for (int i = 0; i < cipherText.size(); i++) {
		ct += cipherText[i];
	}
	
	int key = 0;
	for (int i = 0; i < 256; i++) {
		string keyStream = "";
		key = i;
		
		for (int j = 0; j < ct.size(); j++) {
			keyStream += key;
		}
		
		string tmp = fixed_xor(ct, keyStream);
		
		if (maxScore < scoring_alphabets( tmp )) {
			maxScore = scoring_alphabets( tmp );
			pt = fixed_xor(ct, keyStream);
		}
		
	}
	
	return pt;
}

int main() {
	string hexValue = "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736";
	string cipherText = hex_to_bytes(hexValue);
	
	string plainText = recover_single_byte_key_cipher(cipherText);
	cout << plainText << endl;
	
	return 0;
}

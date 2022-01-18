#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

string fixed_xor(string& lValue, string& rValue) {
	unsigned int len = lValue.size();
	
	string ret = "";
	for (int i = 0; i < len; i++) {
		ret += (lValue[i] ^ rValue[i]);
	}
	
	return ret;
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

int get_hamming_distance(string& a, string& b) {
	int count = 0;
	for (int i = 0; i < a.length(); i++) {
		int tmp = (a[i] & 0xff) ^ (b[i] & 0xff);
		while (tmp) {
			count += tmp & 0x01;
			tmp = tmp >> 1;
		}
	}
	
	return count;
}

int find_key_size(string& ct) {
	
	double normalized = 1000.0f;
	int ret = 0;
	
	for (int keySize = 2; keySize <= 40; keySize++) {
				
		int hamming_distance = 0;
		
		for (int i = 0; i < (ct.length() / keySize) - 1; i++) {
			string blocks[2];
			
			blocks[0] = ct.substr( (keySize * (0 + i)), keySize );
			blocks[1] = ct.substr( (keySize * (1 + i)), keySize );
			
			hamming_distance += get_hamming_distance(blocks[0], blocks[1]) / keySize;
		}
		
		
		double tmpNormalized = static_cast<double>(hamming_distance) / (ct.length() / keySize - 1);
		if (normalized > tmpNormalized) {
			normalized = tmpNormalized;
			ret = keySize;
		}
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



int main() {
	ifstream ifs("cipher_hex.txt");
	stringstream ss;
	string ct;
	
	ss << ifs.rdbuf();
	
	// base64 decode
//	ct = ss.str();
//	ct = base64_decode(ct);
//	ct = hex_to_bytes(ct);
	
	
	// hamming distance test
	string a = "this is a test";
	string b = "wokka wokka!!!";
	cout << get_hamming_distance(a, b) << endl;
	
	ct = ss.str();
	ct = hex_to_bytes(ct);
	
	// keysize guess
	int keySize = find_key_size(ct);
	cout << keySize << endl;
	
	
	// block transpose
	vector<string> blocks;
	blocks.resize(keySize);
	for (int i = 0; i < ct.length(); i++) {
		blocks[i % keySize] += ct[i];
	}
	
	
	string key = "";
	// single-bit XOR
	for (int i = 0; i < keySize; i++) {
		
		char singleKey = 'a';
		
		double maxScore = 0.0;
		
		for (unsigned char j = 0; j < 255; j++) {
			
			string singleKeyStream = "";
			double tmpScore = 0.0;
			
			for (int k = 0; k < blocks[i].length(); k++) {
				singleKeyStream += j;
			}
			
			string xorResult = fixed_xor(blocks[i], singleKeyStream);
			tmpScore = scoring_alphabets( xorResult );
			
			if (maxScore <= tmpScore) {
				maxScore = tmpScore;
				singleKey = j;
			}
			
		}
		
		key += singleKey;
		// alphabet frequency
			// scoring	
	}
	
	cout << "[+] key : \"" << key << "\"" << endl;
	
	
	
	// decrypt the message
	string keyStream = "";
	for (int i = 0; i < (ct.length() / key.length()); i++) {
		keyStream += key;
	}
	
	keyStream += key.substr(0, (ct.length() % key.length()));
	string pt = fixed_xor(keyStream, ct);
	
	cout << "[+] pt : \n" << pt << endl;
	

	return 0;
}

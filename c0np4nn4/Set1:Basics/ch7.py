import base64
from Crypto.Cipher import AES

def main():
    f = open("1_7.txt", "r")
    
    ct = []
    while True:
        line = f.readline()
        if not line: break
        ct += line.strip()

    ct = "".join(ct)
    ct = base64.b64decode(ct)
    
    key = b"YELLOW SUBMARINE"

    aes = AES.new(key, AES.MODE_ECB)

    pt = aes.decrypt(ct)

    print(pt)

if __name__ == "__main__":
    main()

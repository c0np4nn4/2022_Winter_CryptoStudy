from Crypto.Cipher import AES

def main():
    f = open("1_8.txt", "r")

    msgs = []
    while True:
        line = f.readline()
        if not line: break
        msgs.append(bytes.fromhex(line))

    for msg in msgs:
        #print(len(msg)) == 160
        blocks = []
        for i in range(0, len(msg), 16):
            blocks.append(msg[i:i+16])
        blocks_set = set(blocks)

        if (len(blocks) != len(blocks_set)):
            print("[+] AES_ECB Detected : " + str(msg))


if __name__ == "__main__":
    main()

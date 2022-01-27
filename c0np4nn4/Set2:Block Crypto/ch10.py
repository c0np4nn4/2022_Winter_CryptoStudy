from Crypto.Cipher import AES
import base64

def bytes_xor(b1, b2):
    assert len(b1) == len(b2)
    
    ret = bytes(x ^ y for x, y in zip(b1, b2))

    return ret

def pkcs7(msg, block_size):
    assert type(msg) == bytes
    assert type(block_size) == int

    if len(msg) == block_size:
        return msg

    else:
        pad = block_size - (len(msg) % block_size)
        pad = pad * (bytes.fromhex("{0:02x}".format(pad)))
        return msg + pad


def AES_CBC_encrypt(pt, key, IV):
    assert len(IV) == 16
    assert type(pt) == bytes
    assert type(key) == bytes
    
    pt = pkcs7(pt, 16)
    
    aes = AES.new(key, AES.MODE_ECB)
    
    ct = []
    ct.append( aes.encrypt(bytes_xor(pt[0:16], IV)) )
    for i in range(16, len(pt), 16):
        block = bytes_xor(pt[i:i+16], ct[-1])
        ct.append(aes.encrypt(block))
   
    ct = b"".join(ct)

    return ct


def AES_CBC_decrypt(ct, key, IV):
    assert len(IV) == 16
    assert type(ct) == bytes
    assert type(key) == bytes

    aes = AES.new(key, AES.MODE_ECB)
    
    ct = pkcs7(ct, 16)

    pt = []
    pt.append( bytes_xor(aes.decrypt(ct[0:16]), IV) )

    for i in range(16, len(ct), 16):
        block = aes.decrypt(ct[i:i+16])
        pt.append(bytes_xor(ct[i-16:i], block))
    pt = b"".join(pt)
    return pt



def main():
    f = open("2_10.txt", "r")
    ct = []
    while True:
        line = f.readline()
        if not line: break
        ct.append(line)


    ct = "".join(ct)
    ct = base64.b64decode(ct)
    ct = bytes(ct)
   
    key = b"YELLOW SUBMARINE"
    IV = b'\x00' * 16
    print(AES_CBC_encrypt(ct, key, IV)) 
    print(AES_CBC_decrypt(ct, key, IV))


if __name__ == "__main__":
    main()



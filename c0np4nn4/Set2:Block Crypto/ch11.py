import random
import struct
from Crypto.Cipher import AES

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

def encryption_oracle(msg, key):
    assert type(msg) == bytes
    assert type(key) == bytes
    
    # append arbitary bytes to the msg
    pre = b""
    for _ in range(random.randrange(5,11)):
        pre += struct.pack("B", random.randrange(0,256))

    pos = b""
    for _ in range(random.randrange(5,11)):
        pos += struct.pack("B", random.randrange(0,256))
    
    msg = pre + msg + pos


    # encrypt message with either ECB or CBC in 1/2 possibilities
    chance = random.randrange(0,2)
    ret = b""

    if chance == 0:
        print("ECB")
        aes = AES.new(key, AES.MODE_ECB)
        msg = pkcs7(msg, 16)
        ret = aes.encrypt(msg)

    elif chance == 1:
        print("CBC")
        IV = b"".join( struct.pack("B", random.randrange(0, 256)) for _ in range(16) )
        ret = AES_CBC_encrypt(msg, key, IV)

    return ret

def AES_MOO_detection(msgs):
    assert type(msgs) == bytes
    
    # ECB
    blocks = []
    for i in range(0, len(msgs), 16):
        blocks.append(msgs[i:i+16])
    blocks_set = set(blocks)

    if (len(blocks) != len(blocks_set)):
        print("[+] AES_ECB detected")

    # CBC
    else:
        print("[+] AES_CBC detected")



def main():
    
    key = b""
    for i in range(16):
        n = random.randrange(0,256)
        key += struct.pack("B", n)
   
    msg = ""
    f = open("2_11.txt", "r")
    while True:
        line = f.readline()
        if not line: break
        msg += line

    msg = bytes(msg.encode())

    ct = encryption_oracle(msg, key)
   
     
    AES_MOO_detection(ct)










if __name__ == "__main__":
    main()

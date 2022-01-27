def pkcs7(msg, block_size):
    assert type(msg) == bytes
    assert type(block_size) == int

    if len(msg) == block_size:
        return msg

    else:
        pad = block_size - (len(msg) % block_size)
        pad = pad * (bytes.fromhex("{0:02x}".format(pad)))
        return msg + pad



def main():
    msg = b"YELLOW SUBMARINE"
    block_size = 20

    print(pkcs7(msg, block_size))

if __name__ == "__main__":
    main()

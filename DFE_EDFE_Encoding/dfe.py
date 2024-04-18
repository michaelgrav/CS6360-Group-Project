import math

def encode(n, b):
    if n == 0:
        return '0' * b
    else:
        clz = b - int(math.log2(n)) - 1
        len_u = math.ceil(math.log2(b))
        len_l = b - math.ceil(math.log2(b))

        if clz < len_u - 1:
            raise ValueError("Number out of bounds for encoding.")
        else:
            u = (b - clz) << len_l
            l = n << (clz - len_u + 1)
            l_mask = (1 << len_l) - 1

            # Convert the encoded number to binary and pad with zeros to the left to make it b bits long
            return bin(u | (l & l_mask))[2:].zfill(b)

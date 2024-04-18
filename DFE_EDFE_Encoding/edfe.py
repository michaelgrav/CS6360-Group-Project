import math

def encode(n, b):
    if n == 0:
        return '0' * b
    else:
        abs_n = abs(n)
        clz = b - int(math.log2(abs_n)) - 1
        len_u = math.ceil(math.log2(b))
        len_l = b - math.ceil(math.log2(b))

        if clz <= 1:
            raise ValueError("Number out of bounds for encoding.")
        elif clz <= len_u:
            encoded = n ^ (1 << (b - 2))
        else:
            u = (b - clz) << (len_l - 2)
            l = abs_n << (clz - len_u - 1)
            l_mask = (1 << (len_l - 2)) - 1

            if n < 0:
                encoded = ~(u | (l & l_mask))
            else:
                encoded = u | (l & l_mask)

        # Convert the encoded number to binary and pad with zeros to the left to make it b bits long
        return bin(encoded)[2:].zfill(b)

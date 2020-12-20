substitution = {
    'a': 'e',
    'b': 'i',
    'c': 'k',
    'd': 'd',
    'e': 'a',
    'f': 'f',
    'g': 'g',
    'h': 'h',
    'i': 'b',
    'j': 'l',
    'k': 'c',
    'l': 'j',
    'm': 'n',
    'n': 'm',
    'o': 't',
    'p': 'q',
    'q': 'p',
    'r': 'r',
    's': 'z',
    't': 'o',
    'u': 'u',
    'v': 'v',
    'w': 'x',
    'x': 'w',
    'y': 'y',
    'z': 's',
    '{': 'X',
    '}': 'Y',
    '_': 'LOL',
}


def encrypt(plaintext: str) -> str:
    ciphertext = ''

    for letter in plaintext:
        ciphertext += substitution[letter]

    return ciphertext


if __name__ == '__main__':
    plaintext = input()
    print(encrypt(plaintext))




def check_valid(string):
    if string[:4] != "mlh{" and string[-1] != "}":
        return False

    string = string[4:-1]

    magic_numbers = [0x6f, 0x68, 0x5f, 0x73, 0x65, 0x63, 0x5f, 0x6d, 0x79,
                     0x5f, 0x6c, 0x6f, 0x76, 0x65]

    for i in range(len(magic_numbers)):
        if ord(string[i]) != magic_numbers[i]:
            return False

    return True


if __name__ == "__main__":
    string = input()
    if check_valid(string):
        print("Weeeeeeeeeeeeeeeeeeeeeeeeeeeeee")
    else:
        print("Booooooooooooooooooooooooooooooooooo")

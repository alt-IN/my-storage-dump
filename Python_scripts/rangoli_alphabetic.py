# Only one line of input containing the size of the rangoli

def print_rangoli(size):
    M = (size-1)*4 + 1
    N = (size-1)*2 + 1
    alph = 'abcdefghijklmnopqrstuvwxyz'

    if size > 0 and size < 27:
        for i in range(int(N/2)+1):
            line = ''     
            p = alph[size-1-i]
            for j in range(i):
                line += alph[size-1-j]
                line += '-'
            p = line + p + ''.join(reversed(line))
            p = p.center(M, '-')
            print(p)
        for i in range(int(N/2)-1, -1 , -1):
            line = ''     
            p = alph[size-1-i]
            for j in range(i):
                line += alph[size-1-j]
                line += '-'
            p = line + p + ''.join(reversed(line))
            p = p.center(M, '-')
            print(p)



if __name__ == '__main__':
    n = int(input())
    print_rangoli(n)

import random

file = open ('test/test10.txt', 'w')

file.write ("TEST_10\n")

n = random.randint(1,100)
m = random.randint(100, 10000)

file.write (str(n) + '\n')
file.write (str(m)+'\n')

for i in range (1, m):
    file.write (str(random.randint (1, round(m/2))))
    file.write ('\n')

file.close ()

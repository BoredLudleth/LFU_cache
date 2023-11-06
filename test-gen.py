import random

file = open ('test13.txt', 'w')

n = random.randint(15000,20000)
m = random.randint(150000, 200000)

file.write (str(n) + '\n')
file.write (str(m)+'\n')

for i in range (1, m):
    file.write (str(random.randint (1, round(m/6))))
    file.write ('\n')

file.close ()

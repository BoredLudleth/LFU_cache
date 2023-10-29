import random

file = open ('test11.txt', 'w')

n = random.randint(1,100)
m = random.randint(100, 10000)

file.write (str(n) + '\n')
file.write (str(m)+'\n')

for i in range (1, m):
    file.write (str(random.randint (1, round(m/6))))
    file.write ('\n')

file.close ()

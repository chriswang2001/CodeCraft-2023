data = [[],[],[],[]]

with open('./log2.txt', 'r') as f:
    for line in f.readlines():
        score, Map, alpha, beta = [eval(i) for i in line.split()]
        data[Map-1].append([score, alpha, beta])

for i in range(4):
    data[i].sort(reverse = True)
    if data[i] == []:
        continue
    opt = data[i][0]
    score, alpha, beta = opt
    print("Optimal parameter for map{}: alpha={} and beta={}, the score is {}.".format(i+1, alpha, beta, score))

data = []
with open('./log.txt', 'r') as f:
    for score in f.readlines():
        data.append([eval(i) for i in score.split()])
for i in range(4):
    data.sort(key = lambda x:x[i], reverse = True)
    opt = data[0]
    score, alpha, beta = opt[i], opt[-2], opt[-1]
    print("Optimal parameter for map{}: alpha={} and beta={}, the score is {}.".format(i+1, alpha, beta, score))
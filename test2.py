import os
from decimal import Decimal

def score(map):
    # path = '/home/cloud/CLionProjects/HuaweiComp3'
    # f=os.popen('cd '+path)
    # print(f.read())
    f=os.popen('./run.sh {}'.format(map))
    string = f.read()
    numbers = []
    pos = string.find('score')
    for i in range(pos, len(string)):
        c = string[i]
        if '0' <= c <= '9':
            numbers.append(c)
    scores = eval(''.join(numbers))
    # print(scores)
    return scores

data = []
with open('./log.txt', 'r') as f:
    for line in f.readlines():
        data.append([eval(i) for i in line.split()])

for map in range(1,5):
    data.sort(key = lambda x:x[map-1], reverse=True)
    alpha_opt, beta_opt = data[0][-2], data[0][-1]
    if alpha_opt == 0: alpha_opt += 0.05
    if beta_opt == 0: beta_opt += 0.05
    print((alpha_opt,beta_opt))
    for i in range(1,20):
        for j in range(1,20):
            alpha = '{0:0^8}'.format(str(
                Decimal(alpha_opt + (i - 10) / 20 / 10).quantize(Decimal('0.000001'))
            ))
            beta = '{0:0^8}'.format(str(
                Decimal(beta_opt + (j - 10) / 20 / 10).quantize(Decimal('0.000001'))
            ))

            # print(alpha_opt,alpha)
            # print(beta_opt,beta)
            print('alpha='+alpha+' ; beta='+beta)
            with open('./SDK/c++/robot.cpp', 'r') as f:
                content = f.read()
            with open('./SDK/c++/robot.cpp', 'w') as f:
                content = content[:15] + alpha + content[23:38] + beta + content[46:]
                f.write(content)
            scores = score(map)
            print(scores)
            print()
            with open('./log2.txt', 'a') as f:
                content = f.write(str(scores)+' '+str(map)+' '+alpha+' '+beta+'\n')
            # f = os.popen('rm -r /home/cloud/CLionProjects/HuaweiComp3/replay')
            # f = os.popen('mkdir /home/cloud/CLionProjects/HuaweiComp3/replay')

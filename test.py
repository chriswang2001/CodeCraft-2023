import os

def score():
    # path = '/home/cloud/CLionProjects/HuaweiComp3'
    # f=os.popen('cd '+path)
    # print(f.read())
    f=os.popen('./myrunall.sh')
    string = f.read()
    numbers = []
    pos = string.find('score')
    for i in range(pos, len(string)):
        c = string[i]
        if '0' <= c <= '9':
            numbers.append(c)
    scores = [eval(''.join(numbers[0+6*i:6+6*i])) for i in range(4)]
    print(scores)
    print(sum(scores))
    return scores


for i in range(20):
    for j in range(20):
        alpha = '{0:0^4}'.format(str(i/20))
        beta = '{0:0^4}'.format(str(j/20))
        print('alpha='+alpha+' ; beta='+beta)
        with open('./SDK/c++/robot.cpp', 'r') as f:
            content = f.read()
        with open('./SDK/c++/robot.cpp', 'w') as f:
            content = content[:15] + alpha + content[19:34] + beta + content[38:]
            f.write(content)
        scores = score()
        print()
        with open('./log.txt', 'a') as f:
            content = f.write(' '.join(str(i) for i in scores)+' '+alpha+' '+beta+'\n')
        # f = os.popen('rm -r /home/cloud/CLionProjects/HuaweiComp3/replay')
        # f = os.popen('mkdir /home/cloud/CLionProjects/HuaweiComp3/replay')


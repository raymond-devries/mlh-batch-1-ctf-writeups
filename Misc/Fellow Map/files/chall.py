import random

cipher = ['6PJH', '6PW3', '6PPG', '6PMV', '6JG7', '7M29', '6MQQ', '7P3H', '6PRR', '7P5C', '6MXQ', '7P62', '6PHP', '7P4P', '6PV6']
random.shuffle(cipher)

salt = "2222+22"
for i in cipher:
    print(i+salt)

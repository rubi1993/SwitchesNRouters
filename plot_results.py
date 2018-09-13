import matplotlib.pyplot as plt
import numpy
import math

file = open("test.txt", "r")
data = file.readlines()
num_of_rules = []
trie_of_tries_memory = []
trie_of_tries_runtime = []
tree_trie_epsilon_memory = []
tree_trie_epsilon_runtime = []

for line in data:
    contents = line.split(" ")
    num_of_rules.append(contents[0])
    trie_of_tries_memory.append(contents[1])
    trie_of_tries_runtime.append(contents[2])
    tree_trie_epsilon_memory.append(contents[3])
    tree_trie_epsilon_runtime.append(contents[4])

file.close()

figure1 = plt.figure()
axes1 = figure1.add_subplot(111)
axes1.set(ylabel = 'Estimated Bytes per Rule', xlabel='Number of Rules')
axes1.plot(num_of_rules, trie_of_tries_memory, color = 'red', label = 'Trie of Tries')
axes1.plot(num_of_rules, tree_trie_epsilon_memory, color = 'blue', label = 'Tree Trie Epsilon')
axes1.legend(loc='best')
plt.show()                                

figure1 = plt.figure()
axes1 = figure1.add_subplot(111)
axes1.set(ylabel = 'Runtime', xlabel='Number of Rules')
axes1.plot(num_of_rules, trie_of_tries_runtime, color = 'red', label = 'Trie of Tries')
axes1.plot(num_of_rules, tree_trie_epsilon_runtime, color = 'blue', label = 'Tree Trie Epsilon')
axes1.legend(loc='best')
plt.show()           


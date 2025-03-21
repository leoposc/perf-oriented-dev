
import matplotlib.pyplot as plt
import numpy as np
import os 
import re


plt.style.use('ggplot')

'''
data = {
    'program_name': [],
}
'''

def pare_numbers_from_dir(directory):
    data = {}

    # pattern to read real number from /bin/time output
    pattern = r'\d+\.\d+'

    for filename in os.listdir(directory):
        file_path = os.path.join(directory, filename)
        program_name = filename.split('_')[0]

        if program_name not in data:
            data[program_name] = []


        with open(file_path, 'r') as file:
            content = file.read()

            # find first match in file
            match = re.search(pattern, content)

            if match:
                data[program_name].append(float(match.group(0)))

    
    create_boxplot(data)
    print(data)
    print(data.keys())
    print(data.values())

def create_boxplot(data):

    fig = plt.figure(figsize =(10, 5))
    xlabels = data.keys()

    data_values = [d for d in data.values()]
    plt.boxplot(data_values, showmeans=True, showfliers=True)


    plt.xticks(range(1, len(xlabels) + 1), xlabels)
    plt.ylabel('Time (s)')
    plt.title('Execution time for different programs')
    plt.show()


if __name__ == '__main__':
    dir_path = 'small_samples/benchmarks'
    pare_numbers_from_dir(dir_path)
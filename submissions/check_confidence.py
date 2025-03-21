import subprocess
import time
import numpy as np
import os
import re
import matplotlib.pyplot as plt

plt.style.use('ggplot')

# Directory where benchmark results are stored
BENCHMARK_DIR = "benchmarks"

def run_benchmark_script():
    """Runs the benchmark Bash script and waits for it to complete."""
    print("Starting benchmark script...")
    subprocess.run(["./small_samples/run_programs_once.sh"], check=True)  # Ensure script is executable
    print("Benchmark script finished.")

def parse_execution_times(directory):
    """Parses execution times from benchmark output files."""
    data = {}
    pattern = r'(\d+\,\d+)'  # Extract floating-point numbers

    for filename in os.listdir(directory):
        file_path = os.path.join(directory, filename)
        program_name = filename.split('_')[0]

        if program_name not in data:
            data[program_name] = []

        with open(file_path, 'r') as file:
            content = file.read()
            match = re.search(pattern, content)  
            
            if match:
                data[program_name].append(float(match.group(0).replace(',', '.')))

    return data

def confidence_interval(values, confidence=0.95):
    """Computes the confidence interval width for a given dataset."""
    if len(values) < 2:
        return float("inf") #
    mean = np.mean(values)
    stddev = np.std(values, ddof=1) 
    n = len(values)

    # Approximate t-value (for small sample sizes)
    t_value = 1.96 if n > 30 else 2.262

    ci_width = t_value * (stddev / np.sqrt(n))
    return ci_width / mean  

def monitor_benchmark():
    """Runs benchmarks, checks confidence, and generates a boxplot when ready."""
    while True:
        run_benchmark_script()
        data = parse_execution_times(BENCHMARK_DIR)

        # Check confidence for all programs
        confidence_reached = all(
            confidence_interval(times) < 0.1 for times in data.values() if len(times) > 1
        )

        if confidence_reached:
            print("Confidence reached! Generating boxplot...")
            create_boxplot(data)
            break
        else:
            print("Confidence not reached. Running another round...")

def create_boxplot(data):
    """Creates and displays a boxplot of execution times."""
    fig = plt.figure(figsize=(10, 5))
    xlabels = data.keys()
    data_values = list(data.values())

    plt.boxplot(data_values, showmeans=True, showfliers=True)
    plt.xticks(range(1, len(xlabels) + 1), xlabels)
    plt.ylabel("Time (s)")
    plt.title("Execution time for different programs")
    plt.show()

if __name__ == '__main__':
    monitor_benchmark()

import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

sns.set_theme(style="whitegrid")

try:
    df = pd.read_csv("benchmark_results.csv")

    plt.figure(figsize=(10, 6))

    ax = sns.lineplot(
        data=df,
        x="Size",
        y="Operations",
        hue="Solution",
        style="Solution",
        markers=True,
        dashes=False
    )

    ax.set_xscale('log')
    ax.set_yscale('log')

    ax.set_title('Solution Performance vs. Data Size (10-second test)', fontsize=16)
    ax.set_xlabel('Number of Students (Log Scale)', fontsize=12)
    ax.set_ylabel('Operations Completed (Log Scale)', fontsize=12)

    ax.set_xticks(df['Size'].unique())
    ax.get_xaxis().set_major_formatter(plt.ScalarFormatter())

    plt.legend(title='Solution Type')
    plt.grid(True, which="both", ls="--")
    plt.tight_layout()
    plt.savefig("benchmark_performance.png")

    print("Plot saved to benchmark_performance.png")

except FileNotFoundError:
    print("Error: benchmark_results.csv not found.")
    print("Please run your C++ benchmark program first to generate the file.")
except Exception as e:
    print(f"An error occurred: {e}")
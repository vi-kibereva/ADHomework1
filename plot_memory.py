import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
from matplotlib.ticker import ScalarFormatter

sns.set_theme(style="whitegrid")

try:
    df = pd.read_csv("memory_results.csv")
    plt.figure(figsize=(10, 6))

    ax = sns.lineplot(
        data=df,
        x="Size",
        y="MemoryDeltaBytes",
        hue="Solution",
        style="Solution",
        markers=True,
        dashes=False,
        linewidth=2.5
    )

    # Use log scales for both axes
    ax.set_xscale('log')
    ax.set_yscale('log')

    # Set titles and labels
    ax.set_title('Memory Usage vs. Data Size', fontsize=16)
    ax.set_xlabel('Number of Students (Log Scale)', fontsize=12)
    ax.set_ylabel('Memory Usage in Bytes (Log Scale)', fontsize=12)

    # Format axes to show plain numbers, not scientific notation
    ax.get_xaxis().set_major_formatter(ScalarFormatter())
    ax.get_yaxis().set_major_formatter(ScalarFormatter())
    ax.set_xticks(df['Size'].unique())

    # Save the plot to a file
    plt.legend(title='Solution Type')
    plt.grid(True, which="both", ls="--")
    plt.tight_layout()
    plt.savefig("memory_usage.png")

    print("Plot saved to memory_usage.png")

except FileNotFoundError:
    print("Error: memory_results.csv not found.")
    print("Please run your C++ benchmark program first to generate the file.")
except Exception as e:
    print(f"An error occurred: {e}")
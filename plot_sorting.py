import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
from matplotlib.ticker import ScalarFormatter

sns.set_theme(style="whitegrid")

try:
    df = pd.read_csv("../sorting_results.csv")
    plt.figure(figsize=(10, 6))

    ax = sns.lineplot(
        data=df,
        x="Size",
        y="TimeMilliseconds",
        hue="SortType",
        style="SortType",
        markers=True,
        dashes=False,
        linewidth=2.5
    )

    ax.set_xscale('log')
    ax.set_yscale('log')

    ax.set_title('Sorting Algorithm Performance', fontsize=16)
    ax.set_xlabel('Number of Students (Log Scale)', fontsize=12)
    ax.set_ylabel('Time in Milliseconds (Log Scale)', fontsize=12)

    ax.get_xaxis().set_major_formatter(ScalarFormatter())
    ax.get_yaxis().set_major_formatter(ScalarFormatter())
    ax.set_xticks(df['Size'].unique())

    # Save the plot to a file
    plt.legend(title='Sorting Algorithm')
    plt.grid(True, which="both", ls="--")
    plt.tight_layout()
    plt.savefig("../sorting_performance.png")

    print("Plot saved to sorting_performance.png")

except FileNotFoundError:
    print("Error: sorting_results.csv not found.")
except Exception as e:
    print(f"An error occurred: {e}")
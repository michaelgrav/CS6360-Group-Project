import pandas as pd
import matplotlib.pyplot as plt


def Plot_Fetch(file_id):
    colors = ['#77A1D3', '#79CBCA']

    df = pd.read_csv(f'./fetch_log{file_id}.csv', index_col=0)
    ax = df.plot(kind='bar', color=colors, figsize=(10, 6))

    plt.xlabel('Number of Test Runs', fontsize=14, labelpad=10)
    plt.ylabel('Time Taken (Nano-Second)', fontsize=14, labelpad=10)
    plt.title('Fetch Results', fontsize=16, pad=20)
    plt.xticks(rotation=0, fontsize=10)
    plt.yticks(fontsize=10)
    plt.grid(axis='y', linestyle='--', alpha=0.7)

    # Removing the borders
    ax.spines['top'].set_visible(False)
    ax.spines['right'].set_visible(False)
    ax.spines['left'].set_linewidth(0.5)
    ax.spines['bottom'].set_linewidth(0.5)

    # Adding annotations
    for i in ax.patches:
        ax.text(i.get_x() + i.get_width() / 2, i.get_height() + 0.5,
                str(round(i.get_height(), 2)), fontsize=10, ha='center', va='bottom')

    plt.tight_layout()
    plt.show()


fetch_list = [1,2,3]
for fileId in fetch_list:
    Plot_Fetch(fileId)
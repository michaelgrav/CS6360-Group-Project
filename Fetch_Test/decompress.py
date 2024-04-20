import pandas as pd
import gzip

def decompress_and_load_gzip_csv(gzip_file):
    with gzip.open(gzip_file, 'rt') as f:
        df = pd.read_csv(f)
    return df

def export_dataframe_to_csv(df, output_file):
    df.to_csv(output_file, index=False)

compressed_file = "../Data/Datasets/amounts-compressed-dfe-gzip.csv"
output_file = "./dataset.csv"

# Decompress GZIP file and load into DataFrame
df = decompress_and_load_gzip_csv(compressed_file)

# Export DataFrame to CSV
export_dataframe_to_csv(df, output_file)

print("Decompressed data has been exported to:", output_file)

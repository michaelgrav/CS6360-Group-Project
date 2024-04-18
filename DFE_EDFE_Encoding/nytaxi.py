import pandas as pd

import dfe
import edfe

# Read the CSV file
df = pd.read_csv('TaxiData/yellow_tripdata_2024-01.csv', low_memory=False)

# Select columns of interest
df = df[['trip_distance', 'fare_amount', 'tip_amount', 'tolls_amount', 'total_amount']]

# Filter out rows with negative values
df = df[(df >= 0).all(1)]

# Drop rows with NaN values
df = df.dropna()

edfe_df = df.copy()
edfe_df = edfe_df['tolls_amount'].apply(lambda x: edfe.encode(int(x * 100), 32))
print(edfe_df)

dfe_df = df.copy()
dfe_df = dfe_df['tolls_amount'].apply(lambda x: dfe.encode(int(x * 100), 32))
print(dfe_df)

df = df['tolls_amount'].apply(lambda x: format(int(x * 100), '032b'))


print(df)

# Write the rows to a text file
df.to_csv('./TaxiData/amounts.csv', index=False, header=False)
df.to_csv('./TaxiData/amounts-compressed-int-zstd.csv', index=False, header=False, compression='zstd')
df.to_csv('./TaxiData/amounts-compressed-int-zip.csv', index=False, header=False, compression='zip')
df.to_csv('./TaxiData/amounts-compressed-int-gzip.csv', index=False, header=False, compression='gzip')

print(edfe_df)
edfe_df.to_csv('./TaxiData/amounts-edfe.csv', index=False, header=False)
edfe_df.to_csv('./TaxiData/amounts-compressed-edfe-zstd.csv', index=False, header=False, compression='zstd')
edfe_df.to_csv('./TaxiData/amounts-compressed-edfe-zip.csv', index=False, header=False, compression='zip')
edfe_df.to_csv('./TaxiData/amounts-compressed-edfe-gzip.csv', index=False, header=False, compression='gzip')

print(dfe_df)
dfe_df.to_csv('./TaxiData/amounts-dfe.csv', index=False, header=False)
dfe_df.to_csv('./TaxiData/amounts-compressed-dfe-zstd.csv', index=False, header=False, compression='zstd')
dfe_df.to_csv('./TaxiData/amounts-compressed-dfe-zip.csv', index=False, header=False, compression='zip')
dfe_df.to_csv('./TaxiData/amounts-compressed-dfe-gzip.csv', index=False, header=False, compression='gzip')
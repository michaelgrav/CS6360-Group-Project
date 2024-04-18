import pandas as pd

import dfe
import edfe

csv_file_path = './ride_data/ride_data_modified.csv'

df = pd.read_csv(csv_file_path)

coloumn_to_keep = 'start_station_id'

df = df[[coloumn_to_keep]]

edfe_df = df.copy()
edfe_df = edfe_df['start_station_id'].apply(lambda x: edfe.encode(x, 32))
print(edfe_df)

dfe_df = df.copy()
dfe_df = dfe_df['start_station_id'].apply(lambda x: dfe.encode(x, 32))
print(dfe_df)

df = df['start_station_id'].apply(lambda x: format(x, '032b'))

# Write the rows to a text file
df.to_csv('./ride_data/amounts.csv', index=False, header=False)
df.to_csv('./ride_data/amounts-compressed-int-zstd.csv', index=False, header=False, compression='zstd')
df.to_csv('./ride_data/amounts-compressed-int-zip.csv', index=False, header=False, compression='zip')
df.to_csv('./ride_data/amounts-compressed-int-gzip.csv', index=False, header=False, compression='gzip')

edfe_df.to_csv('./ride_data/amounts-edfe.csv', index=False, header=False)
edfe_df.to_csv('./ride_data/amounts-compressed-edfe-zstd.csv', index=False, header=False, compression='zstd')
edfe_df.to_csv('./ride_data/amounts-compressed-edfe-zip.csv', index=False, header=False, compression='zip')
edfe_df.to_csv('./ride_data/amounts-compressed-edfe-gzip.csv', index=False, header=False, compression='gzip')

dfe_df.to_csv('./ride_data/amounts-dfe.csv', index=False, header=False)
dfe_df.to_csv('./ride_data/amounts-compressed-dfe-zstd.csv', index=False, header=False, compression='zstd')
dfe_df.to_csv('./ride_data/amounts-compressed-dfe-zip.csv', index=False, header=False, compression='zip')
dfe_df.to_csv('./ride_data/amounts-compressed-dfe-gzip.csv', index=False, header=False, compression='gzip')

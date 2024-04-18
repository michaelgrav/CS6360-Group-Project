import pandas as pd
import edfe, dfe

csv_file_path = './pollution/pollution_us_2000_2016.csv'

df = pd.read_csv(csv_file_path)

coloumn_to_keep = 'County Code'

df = df[[coloumn_to_keep]]

edfe_df = df.copy()
edfe_df = edfe_df['County Code'].apply(lambda x: edfe.encode(x, 32))
print(edfe_df)

dfe_df = df.copy()
dfe_df = dfe_df['County Code'].apply(lambda x: dfe.encode(x, 32))
print(dfe_df)

df = df['County Code'].apply(lambda x: format(x, '032b'))

# Write the rows to a text file
df.to_csv('./pollution/amounts.csv', index=False, header=False)
df.to_csv('./pollution/amounts-compressed-int-zstd.csv', index=False, header=False, compression='zstd')
df.to_csv('./pollution/amounts-compressed-int-zip.csv', index=False, header=False, compression='zip')
df.to_csv('./pollution/amounts-compressed-int-gzip.csv', index=False, header=False, compression='gzip')

edfe_df.to_csv('./pollution/amounts-edfe.csv', index=False, header=False)
edfe_df.to_csv('./pollution/amounts-compressed-edfe-zstd.csv', index=False, header=False, compression='zstd')
edfe_df.to_csv('./pollution/amounts-compressed-edfe-zip.csv', index=False, header=False, compression='zip')
edfe_df.to_csv('./pollution/amounts-compressed-edfe-gzip.csv', index=False, header=False, compression='gzip')

dfe_df.to_csv('./pollution/amounts-dfe.csv', index=False, header=False)
dfe_df.to_csv('./pollution/amounts-compressed-dfe-zstd.csv', index=False, header=False, compression='zstd')
dfe_df.to_csv('./pollution/amounts-compressed-dfe-zip.csv', index=False, header=False, compression='zip')
dfe_df.to_csv('./pollution/amounts-compressed-dfe-gzip.csv', index=False, header=False, compression='gzip')

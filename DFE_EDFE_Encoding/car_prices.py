import pandas as pd
import dfe, edfe

csv_file_path = './car_prices/car_prices.csv'

df = pd.read_csv(csv_file_path)

coloumn_to_keep = 'year'

df = df[[coloumn_to_keep]]

dfe_df = df.copy()
dfe_df['year'] = dfe_df['year'].apply(lambda x: dfe.encode(x, 32))

edfe_df = df.copy()
edfe_df['year'] = edfe_df['year'].apply(lambda x: edfe.encode(x, 32))

df['year'] = df['year'].apply(lambda x: format(x, '032b'))

# Write the rows to a text file
df.to_csv('./car_prices/amounts.csv', index=False, header=False)
df.to_csv('./car_prices/amounts-compressed-int-zstd.csv', index=False, header=False, compression='zstd')
df.to_csv('./car_prices/amounts-compressed-int-zip.csv', index=False, header=False, compression='zip')
df.to_csv('./car_prices/amounts-compressed-int-gzip.csv', index=False, header=False, compression='gzip')

edfe_df.to_csv('./car_prices/amounts-edfe.csv', index=False, header=False)
edfe_df.to_csv('./car_prices/amounts-compressed-edfe-zstd.csv', index=False, header=False, compression='zstd')
edfe_df.to_csv('./car_prices/amounts-compressed-edfe-zip.csv', index=False, header=False, compression='zip')
edfe_df.to_csv('./car_prices/amounts-compressed-edfe-gzip.csv', index=False, header=False, compression='gzip')

dfe_df.to_csv('./car_prices/amounts-dfe.csv', index=False, header=False)
dfe_df.to_csv('./car_prices/amounts-compressed-dfe-zstd.csv', index=False, header=False, compression='zstd')
dfe_df.to_csv('./car_prices/amounts-compressed-dfe-zip.csv', index=False, header=False, compression='zip')
dfe_df.to_csv('./car_prices/amounts-compressed-dfe-gzip.csv', index=False, header=False, compression='gzip')
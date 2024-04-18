import pandas as pd
import dfe, edfe

# Read the first CSV file
df1 = pd.read_csv('indiv14/indiv_header_file.csv')

# Read the second CSV file, using the header from the first file
df2 = pd.read_csv('indiv14/itcont.txt', header=None, names=df1.columns, delimiter="|", low_memory=False)

# Merge the two dataframes
merged_df = pd.concat([df1, df2], ignore_index=True)

merged_df = merged_df[['TRANSACTION_AMT']]
merged_df = merged_df[['TRANSACTION_AMT']].astype(int, errors='raise')


edfe_df = merged_df.copy()
edfe_df['TRANSACTION_AMT'] = edfe_df['TRANSACTION_AMT'].apply(lambda x : edfe.encode(abs(x), 32))

dfe_df = merged_df.copy()
dfe_df['TRANSACTION_AMT'] = dfe_df['TRANSACTION_AMT'].apply(lambda x: dfe.encode(abs(x), 32))


merged_df['TRANSACTION_AMT'] = merged_df['TRANSACTION_AMT'].apply(lambda x: format(abs(int(x)), '032b'))

# Write the rows to a text file
merged_df.to_csv('./indiv14/amounts.csv', index=False, header=False)
merged_df.to_csv('./indiv14/amounts-compressed-int-zstd.csv', index=False, header=False, compression='zstd')
merged_df.to_csv('./indiv14/donations-compressed-reg-int.zip', index=False, header=False, compression='zip')
merged_df.to_csv('./indiv14/amounts-compressed-int-gzip.csv', index=False, header=False, compression='gzip')

edfe_df.to_csv('./indiv14/amounts-edfe.csv', index=False, header=False)
edfe_df.to_csv('./indiv14/amounts-compressed-edfe-zstd.csv', index=False, header=False, compression='zstd')
edfe_df.to_csv('./indiv14/donations-compressed-edfe.zip', index=False, header=False, compression='zip')
edfe_df.to_csv('./indiv14/amounts-compressed-edfe-gzip.csv', index=False, header=False, compression='gzip')

dfe_df.to_csv('./indiv14/amounts-dfe.csv', index=False, header=False)
dfe_df.to_csv('./indiv14/amounts-compressed-dfe-zstd.csv', index=False, header=False, compression='zstd')
dfe_df.to_csv('./indiv14/donations-compressed-dfe.zip', index=False, header=False, compression='zip')
dfe_df.to_csv('./indiv14/amounts-compressed-dfe-gzip.csv', index=False, header=False, compression='gzip')
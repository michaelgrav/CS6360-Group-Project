import pandas as pd

import dfe
import edfe

# Read the CSV file
df = pd.read_csv('Small LCL Data/LCL-June2015v2_0.csv', low_memory=False)
df2 = pd.read_csv('Small LCL Data/LCL-June2015v2_1.csv', low_memory=False)
df3 = pd.read_csv('Small LCL Data/LCL-June2015v2_2.csv', low_memory=False)
df4 = pd.read_csv('Small LCL Data/LCL-June2015v2_3.csv', low_memory=False)
df5 = pd.read_csv('Small LCL Data/LCL-June2015v2_4.csv', low_memory=False)
df6 = pd.read_csv('Small LCL Data/LCL-June2015v2_5.csv', low_memory=False)
df7 = pd.read_csv('Small LCL Data/LCL-June2015v2_6.csv', low_memory=False)
df8 = pd.read_csv('Small LCL Data/LCL-June2015v2_7.csv', low_memory=False)
df9 = pd.read_csv('Small LCL Data/LCL-June2015v2_8.csv', low_memory=False)
df10 = pd.read_csv('Small LCL Data/LCL-June2015v2_9.csv', low_memory=False)

df = pd.concat([df, df2, df3, df4, df5, df6, df7, df8, df9, df10], ignore_index=True)
df = df.dropna()
df = df[df['KWH/hh (per half hour) '] != 'Null']
df = df[['KWH/hh (per half hour) ']].astype(float, errors='raise')
df = df.rename(columns={'KWH/hh (per half hour) ': 'Home'})


df['Home'] = df['Home'].apply(lambda x: int(x * 1000))

edfe_df = df.copy()
edfe_df['Home'] = edfe_df['Home'].apply(lambda x: edfe.encode(x, 32))

dfe_df = df.copy()
dfe_df['Home'] = dfe_df['Home'].apply(lambda x: dfe.encode(x, 32))

df['Home'] = df['Home'].apply(lambda x: format(int(x), '032b'))

# Write the rows to a text file
df.to_csv('./Small LCL Data/data/amounts.csv', index=False, header=False)
df.to_csv('./Small LCL Data/data/amounts-compressed-int-zstd.csv', index=False, header=False, compression='zstd')
df.to_csv('./Small LCL Data/data/amounts-compressed-int-zip.csv', index=False, header=False, compression='zip')
df.to_csv('./Small LCL Data/data/amounts-compressed-int-gzip.csv', index=False, header=False, compression='gzip')

print(edfe_df)
edfe_df.to_csv('./Small LCL Data/data/amounts-edfe.csv', index=False, header=False)
edfe_df.to_csv('./Small LCL Data/data/amounts-compressed-edfe-zstd.csv', index=False, header=False, compression='zstd')
edfe_df.to_csv('./Small LCL Data/data/amounts-compressed-edfe-zip.csv', index=False, header=False, compression='zip')
edfe_df.to_csv('./Small LCL Data/data/amounts-compressed-edfe-gzip.csv', index=False, header=False, compression='gzip')

print(dfe_df)
dfe_df.to_csv('./Small LCL Data/data/amounts-dfe.csv', index=False, header=False)
dfe_df.to_csv('./Small LCL Data/data/amounts-compressed-dfe-zstd.csv', index=False, header=False, compression='zstd')
dfe_df.to_csv('./Small LCL Data/data/amounts-compressed-dfe-zip.csv', index=False, header=False, compression='zip')
dfe_df.to_csv('./Small LCL Data/data/amounts-compressed-dfe-gzip.csv', index=False, header=False, compression='gzip')
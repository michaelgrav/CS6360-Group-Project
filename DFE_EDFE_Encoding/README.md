# Folder Contents
This folder contains the code we used to convert the datasets into their DFE/EDFE format and to test compressibility.

The dfe.py and edfe.py file contain the encoding algorithm used for DFE/EDFE.

The other python files are used to both compress the dataset with Zstandard, GZIP, and ZIP, and to convert the dataset into DFE/EDFE

# How to run
Please download each dataset which you wish to test on from the readme in the root of the project. After downloading, dezip the dataset and place the csv file(s) into their respective folders in this one.

Special cases
- TaxiData: Make sure to convert the parquet file into a CSV file before putting it into the folder.
- Small LCL Data (energy usage): Move the first 10 csv files from the data folder into the "Small LCL Folder" within this project

- Download Dataset and move into required folder
- Run the corresponding Python file to the dataset which you want to test one. The Python file will read the dataset, encode/compress it, and output newly encoded and compressed files.
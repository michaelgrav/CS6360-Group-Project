# CS6360-Group-Project
# Group Members
- Michael Graves MDG200001
- Avinash Vadivelu AXV200086
- Malay Bhavsar MXB230055
- Yu Liang YXL230034 
- Qin Qin QXQ220003

# Folder Structure
The project is split into different folders for each test we did on DFE/EDFE, and has folders for the Python scripts we used for data visualization as well. The folders are listed below

| Folder Name       | HTML                                                                                                                                                                                                                   |
|-------------------|------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| DFE_EDFE_Encoding | <ul><li>Houses the Python scripts we used to convert the datasets to their DFE/EDFE encoded files</li><li>Contains scripts that we used to compress the DFE/EDFE encoded files with Zstandard, GZIP, and ZIP</li></ul> |
| Fetch_Test        | Contains the C++ code used for the fetch test                                                                                                                                                                          |
| Data              | Contains the Python code used for the graphs/charts seen inside the final paper.                                                                                                                                       |
| Scan_test              | Contains the C++ code (Bitweaving and Byteslice) used for the scan test.                                                                                                                                       |



# Datasets used
## Research Paper Datasets
- Taxi Cab Data: https://d37ci6vzurychx.cloudfront.net/trip-data/yellow_tripdata_2024-01.parquet
  - Note that we converted this dataset from parquet to CSV using an online tool before we inputted it into our scripts
- Election Donations: https://www.fec.gov/files/bulk-downloads/2014/indiv14.zip
- Energy Usage: https://data.london.gov.uk/download/smartmeter-energy-use-data-in-london-households/04feba67-f1a3-4563-98d0-f3071e3d56d1/Partitioned%20LCL%20Data.zip

## Our datasets
- Car Prices: https://www.kaggle.com/code/narayanyadav/notebooka6567793c7/input
- Pollution: https://www.kaggle.com/code/spoorthy31/data-visualization/input
- Ride Data: https://www.kaggle.com/code/omkarpatil2401/ride-data-anlaysis/input

# How to run the code
## Scan Test
### Steps
- Git clone the repo.
- Open the folder 'Testing'.
- Copy the dataset file(donations-compressed-reg-int/dfe/edfe) you want to test on.
- Paste into one of the three(ByteSlice/BitweavingV/BitweavingH) corresponding folders (e.g. BitweavingVEDFE).
- In main function, change the path to the absolute path of the dataset on your device.
- Run main.cpp.

### Results
- Our testing result is stored in Testing/CS6360 Testing Result.xlsx
- The result plot is Testing/resultPlot/png

## Fetch Test
### Steps
- Run decompress.py to decompress the required dataset
- Compile and run fetch_test.cpp

### Results
- The results will be stored inside a file within the folder called "timing_log.txt"

## DFE/EDFE Encoding Datasets
### Steps
- Within the DFE_EDFE_Encoding folder will be many Python files. Each one corresponds to one of the 6 datasets that we used.
- See the README file located within the folder for instructions on how to run

### Results
- The Python files will output one of the 6 datasets encoded into DFE/EDFE, and compressed with Zstandard, ZIP, and GZIP
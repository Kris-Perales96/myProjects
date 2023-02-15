from asyncio.windows_events import NULL
from tempfile import tempdir
import pandas as pd
import math
from datetime import datetime

file_name = "Austin_weather_data_2016.xlsx"
output_file_name = "formatted_weather_file2016.xlsx"

input_excel_file2015 = pd.read_excel(file_name, sheet_name='Austin 2015')
input_excel_file2016 = pd.read_excel(file_name, sheet_name='Austin 2016')

formatted_excel_file = pd.DataFrame()
formatted_excel_file2016 = pd.DataFrame()
minutes_of_importance = 53
excel_wind_data_label = 'wind_spd (mph)'
excel_weather_data_label = 'Weather Condition'

for key, row in input_excel_file2015.iterrows():
	current_day_minutes = row['date'].minute
	
	if current_day_minutes == minutes_of_importance:
		formatted_excel_file = pd.concat([formatted_excel_file, row.to_frame().T], ignore_index = True)
		

for key, row in input_excel_file2016.iterrows():
	current_day_minutes = row['date'].minute
	
	if current_day_minutes == minutes_of_importance:
		formatted_excel_file2016 = pd.concat([formatted_excel_file2016, row.to_frame().T], ignore_index = True)

formatted_excel_file[excel_wind_data_label] = formatted_excel_file[excel_wind_data_label].fillna(0.0)
formatted_excel_file2016[excel_wind_data_label] = formatted_excel_file2016[excel_wind_data_label].fillna(0.0)
formatted_excel_file[excel_weather_data_label] = formatted_excel_file[excel_weather_data_label].fillna('Clear')
formatted_excel_file2016[excel_weather_data_label] = formatted_excel_file2016[excel_weather_data_label].fillna('Clear')

formatted_excel_file.to_excel(output_file_name, sheet_name= 'Austin 2015')
with pd.ExcelWriter(output_file_name, mode='a') as writer:
	formatted_excel_file2016.to_excel(writer, sheet_name = 'Austin 2016')
	
	
	


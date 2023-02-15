from asyncio.windows_events import NULL
from cmath import nan
import pandas as pd
import math

excel_file_name = 'formatted_weather_file2016.xlsx'
sheet_name2015 = 'Austin 2015'
sheet_name2016 = 'Austin 2016'
output_file_name = 'Austin_2016_formatted_nCalculated.xlsx'

Austin2015_input = pd.read_excel(excel_file_name, sheet_name = sheet_name2015)
Austin2016_input = pd.read_excel(excel_file_name, sheet_name = sheet_name2016)

milesph_to_metersps = 2.23693629 #Divide the mph by this constant
vc = 3
vr = 12
vs = 25
Pr = 2
It = 1000
solarEfficiency = 0.20
panelSize = (41.5 * 0.0254) * (62.6 * 0.0254)


def calculatePowerFromWind(wind_speed):
    if wind_speed < vc:
        return 0
    elif wind_speed >= vc and wind_speed < vr:
        return ((math.pow(wind_speed,3) - math.pow(vc,3)) / (math.pow(vr,3) - math.pow(vc,3))) * Pr
    elif wind_speed >= vr and wind_speed < vs:
        return Pr 
    else:
        return 0

def calculatePowerFromSolar(temperature, timeOfDay):
    temperature_in_celsius = (temperature - 32) / 1.8
    if(timeOfDay>6 and timeOfDay < 20):
        if(timeOfDay <=16):
            solarPower = (timeOfDay / 12) * solarEfficiency * panelSize * It * (1-0.005*(temperature_in_celsius - 25))
        else:
            solarPower = ((timeOfDay-12)/12) * solarEfficiency * panelSize * It * (1-0.005*(temperature_in_celsius - 25))
    else:
        solarPower = 0
    return solarPower
    
windPower2015_hr = []
windPower2016_hr = []
wind_speed_ms2015 = []
wind_speed_ms2016 = []

solarPower2015_hr =[]
solarPower2016_hr = []

number_of_entires2015 = 8680
number_of_entires2016 = 8772

possible_wind_values_int = 26
wind_speed_posibilities = {}
wind_speed_posibilities2016 ={}

possible_temperature_values = 106
temperature_probabilities2015 = {}
temperature_probabilities2016 = {}



possible_weather_values = ['Haze', 'Partly Cloudy', 'Clear', 'Scattered Clouds', 'Patches of Fog', 'Overcast', 'Light Drizzle', 'Light Rain', 'Rain', 
                           'Thunderstorms and Rain', 'Mostly Cloudy', 'Fog', 'Light Thunderstorms and Rain', 'Light Freezing Rain', 'Shallow Fog', 'Heavy Rain',
                           'Light Freezing Drizzle', 'Thunderstorm', 'Heavy Thunderstorms and Rain', 'Mist', 'Smoke']


weather_conditions_posibilities = {}
weather_conditions_posibilities2016 = {}
weather_wind_speed_probabilities2015 = {}
weather_wind_speed_probabilities2016 = {}
bayes_weather_wind_speed2015 = {}
bayes_weather_wind_speed2016 = {}

for i in range(possible_wind_values_int):
    wind_speed_posibilities[i] = 0
    wind_speed_posibilities2016[i] = 0
    
for val in possible_weather_values:
    weather_conditions_posibilities[val] = 0
    weather_conditions_posibilities2016[val] = 0
    weather_wind_speed_probabilities2015[val] = {}
    weather_wind_speed_probabilities2016[val] = {}
    bayes_weather_wind_speed2015[val] = {}
    bayes_weather_wind_speed2016[val] = {}
    
for key in weather_wind_speed_probabilities2015:
    for i in range(possible_wind_values_int):
        weather_wind_speed_probabilities2015[key][i] = 0
        weather_wind_speed_probabilities2016[key][i] = 0

    
for i in range(possible_temperature_values):
    temperature_probabilities2015[i] = 0
    temperature_probabilities2016[i] = 0

for key, row in Austin2015_input.iterrows():
    temp_windspeed = row['wind_spd (mph)'] / milesph_to_metersps
    temp_weather_condition = row['Weather Condition']
    temp_temperature = row['temp (F)']
    temp_index = math.floor(temp_windspeed)
    weather_wind_speed_probabilities2015[temp_weather_condition][temp_index] +=1
    
    wind_speed_ms2015.append(temp_windspeed)
    windPower2015_hr.append(calculatePowerFromWind(row['wind_spd (mph)'] / milesph_to_metersps))
    solarPower2015_hr.append(calculatePowerFromSolar(row['temp (F)'], row['date'].hour))
    
    temp_temperature_index = math.floor(temp_temperature)
    wind_speed_posibilities[(temp_index)]+=1
    temperature_probabilities2015[temp_temperature_index]+=1
    weather_conditions_posibilities[temp_weather_condition]+=1


#Probabilities for the possible wind speeds, weather conditions and temperature in 2015
for key in wind_speed_posibilities.keys():
    wind_speed_posibilities[key] = wind_speed_posibilities[key]/ number_of_entires2015 
for key in weather_conditions_posibilities.keys():
    weather_conditions_posibilities[key] = weather_conditions_posibilities[key] / number_of_entires2015
for key in temperature_probabilities2015.keys():
    temperature_probabilities2015[key] = temperature_probabilities2015[key] / number_of_entires2015   

Austin2015_input['wind_spd (m/s)'] = wind_speed_ms2015
Austin2015_input['Wind Power(kWh)'] = windPower2015_hr
Austin2015_input['Solar Power hourly (kWh)'] = solarPower2015_hr

current_month = 0
total = 0
total_solar_power = 0
total_hours = 0
austin2015_monthly_wind_power = []
austin2015_monthly_solar_power = []
austin2016_monthly_wind_power = []
austin2016_monthly_solar_power = []

#Monthly based power calculations for 2015
for key, row in Austin2015_input.iterrows():
    if current_month == 0:
        current_month = 1
        total = (row['Wind Power(kWh)'])
        total_solar_power = row['Solar Power hourly (kWh)']
        total_hours+=1
    elif current_month == row['date'].month:
        total += (row['Wind Power(kWh)'])
        total_solar_power += row['Solar Power hourly (kWh)']
        total_hours+=1
    elif current_month != row['date'].month:
        austin2015_monthly_wind_power.append(total)
        austin2015_monthly_solar_power.append(total_solar_power)
        current_month += 1
        total_solar_power = row['Solar Power hourly (kWh)']
        total = row['Wind Power(kWh)']
        total_hours = 0
    elif current_month == 12  and row['date'].hour == 11 and row['date'].minute ==53:
        austin2015_monthly_wind_power.append(total)
        austin2015_monthly_solar_power.append(total_solar_power)
        current_month += 1
        total_hours = 0
        
austin2015_monthly_wind_power.append(total)
austin2015_monthly_solar_power.append(total_solar_power)
austin2015_monthly_wind_power = pd.Series(austin2015_monthly_wind_power)
austin2015_monthly_solar_power = pd.Series(austin2015_monthly_solar_power)
Austin2015_input = pd.concat([Austin2015_input, austin2015_monthly_wind_power], ignore_index=True, axis=1)
Austin2015_input = pd.concat([Austin2015_input, austin2015_monthly_solar_power], ignore_index=True, axis = 1)

for key, row in Austin2016_input.iterrows():
    temp_windspeed = row['wind_spd (mph)'] / milesph_to_metersps
    temp_weather_condition = row['Weather Condition']
    temp_temperature = row['temp (F)']
    temp_index = math.floor(temp_windspeed)
    weather_wind_speed_probabilities2016[temp_weather_condition][temp_index] +=1
    
    wind_speed_ms2016.append(row['wind_spd (mph)'] / milesph_to_metersps)
    windPower2016_hr.append(calculatePowerFromWind(row['wind_spd (mph)'] / milesph_to_metersps))
    solarPower2016_hr.append(calculatePowerFromSolar(row['temp (F)'], row['date'].hour))
    
    temp_temperature_index = math.floor(temp_temperature)
    wind_speed_posibilities2016[(temp_index)]+=1
    temperature_probabilities2016[temp_temperature_index]+=1
    weather_conditions_posibilities2016[temp_weather_condition]+=1

#Probabilities for the possible wind speeds in 2016
for key in wind_speed_posibilities2016.keys():
    wind_speed_posibilities2016[key] = wind_speed_posibilities2016[key]/number_of_entires2016
for key in weather_conditions_posibilities2016.keys():
    weather_conditions_posibilities2016[key] = weather_conditions_posibilities2016[key] / number_of_entires2016
for key in temperature_probabilities2016.keys():
    temperature_probabilities2016[key] = temperature_probabilities2016[key] / number_of_entires2016 

for key in weather_conditions_posibilities:
    for val in wind_speed_posibilities:
        weather_wind_speed_probabilities2015[key][val] = (weather_wind_speed_probabilities2015[key][val] / number_of_entires2015) * 100
        weather_wind_speed_probabilities2016[key][val] = (weather_wind_speed_probabilities2016[key][val] / number_of_entires2016) * 100


 

Austin2016_input['wind_spd (m/s)'] = wind_speed_ms2016
Austin2016_input['Wind Power(kWh)'] = windPower2016_hr
Austin2016_input['Solar Power hourly (kWh)'] = solarPower2016_hr

#Monthly based power calculations for 2016
current_month = 0
for key, row in Austin2016_input.iterrows():
    hour_count = 0
    if current_month == 0:
        current_month = 1
        total = (row['Wind Power(kWh)'])
        total_solar_power = row['Solar Power hourly (kWh)']
        total_hours+=1
    elif current_month == row['date'].month:
        total += (row['Wind Power(kWh)'])
        total_solar_power += row['Solar Power hourly (kWh)']
        total_hours+=1
    elif current_month != row['date'].month:
        austin2016_monthly_wind_power.append(total)
        austin2016_monthly_solar_power.append(total_solar_power)
        current_month += 1
        total = row['Wind Power(kWh)']
        total_solar_power = row['Solar Power hourly (kWh)']
        total_hours = 0

austin2016_monthly_wind_power.append(total)
austin2016_monthly_solar_power.append(total_solar_power)
austin2016_monthly_wind_power = pd.Series(austin2016_monthly_wind_power)
austin2016_monthly_solar_power = pd.Series(austin2016_monthly_solar_power)
Austin2016_input = pd.concat([Austin2016_input, austin2016_monthly_wind_power], ignore_index=True, axis=1)
Austin2016_input = pd.concat([Austin2016_input, austin2016_monthly_solar_power], ignore_index=True, axis = 1)

Austin2015_input.to_excel(output_file_name, sheet_name= 'Austin 2015')
with pd.ExcelWriter(output_file_name, mode='a') as writer:
	Austin2016_input.to_excel(writer, sheet_name = 'Austin 2016')
 
weather_probability_df2015 = pd.DataFrame(data = weather_conditions_posibilities, index = [2015])
weather_probability_df2015 = weather_probability_df2015.T
weather_probability_df2016 = pd.DataFrame(data = weather_conditions_posibilities2016, index = [2016])
weather_probability_df2016 = weather_probability_df2016.T
weather_probability_df2015 = pd.concat([weather_probability_df2015, weather_probability_df2016], ignore_index = False, axis=1)
weather_probability_excel_name = 'Weather_Probability2015_2016.xlsx'
weather_probability_df2015.to_excel(weather_probability_excel_name)

wind_speed_probabilitiesdf2015 = pd.DataFrame(data=wind_speed_posibilities, index = [2015])
wind_speed_probabilitiesdf2015 = wind_speed_probabilitiesdf2015.T
wind_speed_probabilitiesdf2016 = pd.DataFrame(data=wind_speed_posibilities2016, index = [2016])
wind_speed_probabilitiesdf2016 = wind_speed_probabilitiesdf2016.T
wind_speed_probabilitiesdf2015 = pd.concat([wind_speed_probabilitiesdf2015, wind_speed_probabilitiesdf2016], ignore_index = False, axis=1)
wind_probability_excel_name = 'Wind_Speed_Probability2015_2016.xlsx'
wind_speed_probabilitiesdf2015.to_excel(wind_probability_excel_name)

temperature_probabilitiesdf2015 = pd.DataFrame(data=temperature_probabilities2015, index = [2015])
temperature_probabilitiesdf2015 = temperature_probabilitiesdf2015.T
temperature_probabilitiesdf2016 = pd.DataFrame(data=temperature_probabilities2016, index = [2016])
temperature_probabilitiesdf2016 = temperature_probabilitiesdf2016.T
temperature_probabilitiesdf2015 = pd.concat([temperature_probabilitiesdf2015, temperature_probabilitiesdf2016], ignore_index = False, axis=1)
temperature_probability_excel_name = 'Temperature_Probability2015_2016.xlsx'
temperature_probabilitiesdf2015.to_excel(temperature_probability_excel_name)

weather_wind_speed_probabilities2015df = pd.DataFrame(data=weather_wind_speed_probabilities2015['Haze'], index = ['Haze'])
weather_wind_speed_probabilities2015df = weather_wind_speed_probabilities2015df.T
weather_wind_speed_probabilities2016df = pd.DataFrame(data=weather_wind_speed_probabilities2016['Haze'], index = ['Haze'])
weather_wind_speed_probabilities2016df = weather_wind_speed_probabilities2016df.T


joint_prob_array2015 = []
joint_prob_arraydf2015 = []
joint_prob_array2016 = []
joint_prob_arraydf2016 = []
for key  in weather_conditions_posibilities:
    joint_prob_array2015.append(pd.DataFrame(data=weather_wind_speed_probabilities2015[key], index = [key]))
    joint_prob_array2016.append(pd.DataFrame(data=weather_wind_speed_probabilities2016[key], index = [key]))
for i in range(21):
    joint_prob_arraydf2015.append(joint_prob_array2015[i].T)
    joint_prob_arraydf2016.append(joint_prob_array2016[i].T)
    
    
joint_probdf = joint_prob_arraydf2015[0]
joint_probdf2016 = joint_prob_arraydf2016[0]
for i in range(20):
    joint_probdf = pd.concat([joint_probdf, joint_prob_arraydf2015[i+1]], ignore_index= False, axis = 1)
    joint_probdf2016 = pd.concat([joint_probdf2016, joint_prob_arraydf2016[i+1]], ignore_index= False, axis = 1)
    

    
joint_prob_weather_wind_excel_name2015 = 'conditional_Probability2015.xlsx'
joint_prob_weather_wind_excel_name2016 = 'conditional_Probability2016.xlsx'

joint_probdf.to_excel(joint_prob_weather_wind_excel_name2015)
joint_probdf2016.to_excel(joint_prob_weather_wind_excel_name2016)

for key in possible_weather_values:
    for i in range(possible_wind_values_int):
        bayes_weather_wind_speed2015[key][i] = ((weather_conditions_posibilities[key]) * (weather_wind_speed_probabilities2015[key][i]/100)) * 100
        bayes_weather_wind_speed2016[key][i] = ((weather_conditions_posibilities2016[key]) * (weather_wind_speed_probabilities2016[key][i]/100)) * 100

bayes_prob_array2015 = []
bayes_prob_arraydf2015 = []
bayes_prob_array2016 = []
bayes_prob_arraydf2016 = []
for key  in weather_conditions_posibilities:
    bayes_prob_array2015.append(pd.DataFrame(data=bayes_weather_wind_speed2015[key], index = [key]))
    bayes_prob_array2016.append(pd.DataFrame(data=bayes_weather_wind_speed2016[key], index = [key]))
for i in range(21):
    bayes_prob_arraydf2015.append(bayes_prob_array2015[i].T)
    bayes_prob_arraydf2016.append(bayes_prob_array2016[i].T)
    
    
bayes_probdf = bayes_prob_arraydf2015[0]
bayes_probdf2016 = bayes_prob_arraydf2016[0]
for i in range(20):
    bayes_probdf = pd.concat([bayes_probdf, bayes_prob_arraydf2015[i+1]], ignore_index= False, axis = 1)
    bayes_probdf2016 = pd.concat([bayes_probdf2016, bayes_prob_arraydf2016[i+1]], ignore_index= False, axis = 1)
    

    
bayes_prob_weather_wind_excel_name2015 = 'joint_Probability2015.xlsx'
bayes_prob_weather_wind_excel_name2016 = 'joint_Probability2016.xlsx'

bayes_probdf.to_excel(bayes_prob_weather_wind_excel_name2015)
bayes_probdf2016.to_excel(bayes_prob_weather_wind_excel_name2016)
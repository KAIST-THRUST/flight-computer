import csv
import re

def get_files(test_list = "./log_SDcard/raw_files/test_list.txt"):
    with open(test_list, "r") as file:
        files = file.readlines()

    files = [file.strip() for file in files]
    files = [file.split("#")[0] for file in files]
    files = [file.strip() for file in files]

    return files

def parse_log(input_file, output_file):
    gps_data_1 = [['Time (ms)', 'Lat (deg)', 'Lon (deg)', 'AltMSL (m)', 'Geoid (m)']]
    gps_data_2 = [['Lat_Launch_Site (deg)', 'Lon_Launch_Site (deg)', 'AltMSL_Launch_Site (m)', 'Geoid_Launch_Site (m)']]
    
    ahrs_data_1 = [['Time (ms)', 'AccX (m/s2)', 'AccY (m/s2)', 'AccZ (m/s2)']]
    ahrs_data_2 = [['GyroX (rad/s)', 'GyroY (rad/s)', 'GyroZ (rad/s)']]
    ahrs_data_3 = [['qX', 'qY', 'qZ', 'qW']]
    
    bmp_data_1 = [['Time (ms)', 'p_baro (hPa)', 'T_baro (degC)']]
    bmp_data_2 = [['P_Launch_Site (hPa)', 'T_Launch_Site (degC)']]
    
    adc_data = [['Time (ms)', 'V_bat (V)', 'p_chamber (barg)']]
    
    nav_data_1 = [['Time (ms)', 'posE (m)', 'posN (m)', 'posU (m)']]
    nav_data_2 = [['velE (m/s)', 'velN (m/s)', 'velU (m/s)']]
    nav_data_3 = [['max_alt (m)']]
    
    with open(input_file, "r") as file:
        for line in file:
          if line.startswith('['):
            time, log_type, data = line.strip().split(' ', 2)
            time = time[1:-1]
            if log_type == "[DATA]":
              data = data[5:]
              data = data.strip()
              numbers = re.findall(r"[-+]?\d*\.\d+|\d+", data)

              first_word = data.split(':')[0]
              if first_word == "Lat":
                gps_data_1.append([time] + numbers)
              elif first_word == "Lat_LS":
                gps_data_2.append(numbers)
              elif first_word == "Acc":
                ahrs_data_1.append([time] + numbers)
              elif first_word == "Ang vel":
                ahrs_data_2.append(numbers)
              elif first_word == "Quat":
                ahrs_data_3.append(numbers)
              elif first_word == "Pres":        
                bmp_data_1.append([time] + numbers)
              elif first_word == "Pres_AVG":
                bmp_data_2.append(numbers)
              elif first_word == "Volt":
                adc_data.append([time] + numbers)
              elif first_word == "pos": 
                nav_data_1.append([time] + numbers)
              elif first_word == "vel":
                nav_data_2.append(numbers)
              elif first_word == "max_alt": 
                nav_data_3.append(numbers)          

    gps_data = [gps_data_1[i] + gps_data_2[i] for i in range(len(gps_data_1))]
    ahrs_data = [ahrs_data_1[i] + ahrs_data_2[i] + ahrs_data_3[i] for i in range(len(ahrs_data_1))]
    bmp_data = [bmp_data_1[i] + bmp_data_2[i] for i in range(len(bmp_data_1))]
    nav_data = [nav_data_1[i] + nav_data_2[i] + nav_data_3[i] for i in range(len(nav_data_1))]

    len_diff = len(gps_data) - len(nav_data)
    nav_data = [nav_data[0]] + [[" "] * len(nav_data[0]) for _ in range(len_diff-1)] + nav_data[1:] + [[" "] * len(nav_data[0])]
    concatenated_data = [gps_data[i] + ahrs_data[i] + bmp_data[i] + adc_data[i] + nav_data[i] for i in range(len(gps_data))]

    with open(output_file, "w") as file:
        writer = csv.writer(file)
        writer.writerows(concatenated_data)
        

    print(f"Log file has been parsed to {output_file}")


def main():
    files = get_files()
    for file in files:
        input_file = f"./log_SDcard/raw_files/{file}"
        output_file = f"./log_SDcard/parsed_files/{file.split('/')[-1].replace('.txt', '.csv')}"
        parse_log(input_file, output_file)

if __name__ == "__main__":
    main()
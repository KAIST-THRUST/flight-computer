% MALTAB script for plot ENU position
clear all; clc; close all;
format longG

% read log file
filename = "2024-07-20 02-23-24.csv";
log = readtable(filename);

% launch site configuration
lat_ls_deg = log.Lat_Launch_Site_deg_(end-1); % deg, launch site latitude
lon_ls_deg = log.Lon_Launch_Site_deg_(end-1); % deg, launch site longitude
alt_msl_ls_m = log.AltMSL_Launch_Site_m_(end-1); % m, launch site msl altitude
geoid_ls_m = log.Geoid_Launch_Site_m_(end-1); % m, launch site geoid separation
alt_wgs84_ls_m = alt_msl_ls_m + geoid_ls_m; % m, launch site wgs84 altitude
r_ls_ECEF_m = lla2ecef([lat_ls_deg, lon_ls_deg, alt_wgs84_ls_m], 'wgs84');
DCM_ECEF_2_ENU = [-sind(lon_ls_deg), cosd(lon_ls_deg), 0; 
                  -sind(lat_ls_deg)*cosd(lon_ls_deg), -sind(lat_ls_deg)*sind(lon_ls_deg), cosd(lat_ls_deg);
                  cosd(lat_ls_deg)*cosd(lon_ls_deg), cosd(lat_ls_deg)*sind(lat_ls_deg), sind(lat_ls_deg)];

% lla measurement during experiment
time_stamp_sec = log.Time_ms_ * 0.001; % sec, time stamp measurement
lat_deg = log.Lat_deg_; % deg, latitude
lon_deg = log.Lon_deg_; % deg, longitude
alt_msl_m = log.AltMSL_m_; % m, msl altitude
geoid_m = log.Geoid_m_; % m, geoid separation
alt_wgs84_m = alt_msl_m + geoid_m; % m, wgs84 altitude
r_ECEF_m = lla2ecef([lat_deg, lon_deg, alt_wgs84_m], 'wgs84');
r_ENU_m = (DCM_ECEF_2_ENU * (r_ECEF_m - r_ls_ECEF_m)')'; % convert to ENU coordinate

% plot and compare ENU pos from GPS measurement and navigation solution
figure;
subplot(3, 1, 1);
hold on
plot(time_stamp_sec, r_ENU_m(:, 1), 'DisplayName', 'GPS');
plot(time_stamp_sec, log.posE_m_, 'DisplayName', 'NAV Algorithm');
xlabel("time [sec]")
ylabel("pos E [m]")
title("E pos")
grid on
legend show

subplot(3, 1, 2);
hold on
plot(time_stamp_sec, r_ENU_m(:, 2), 'DisplayName', 'GPS');
plot(time_stamp_sec, log.posN_m_, 'DisplayName', 'NAV Algorithm');
xlabel("time [sec]")
ylabel("pos N [m]")
title("N pos")
grid on
legend show

subplot(3, 1, 3);
hold on
plot(time_stamp_sec, r_ENU_m(:, 3), 'DisplayName', 'GPS');
plot(time_stamp_sec, log.posU_m_, 'DisplayName', 'NAV Algorithm');
xlabel("time [sec]")
ylabel("pos U [m]")
title("U pos")
grid on
legend show

% location
figure;
hold on
plot(r_ENU_m(:, 1), r_ENU_m(:, 2), 'DisplayName', 'GPS');
plot(log.posE_m_, log.posN_m_, 'DisplayName', 'NAV Algorithm');
xlabel("E [m]")
ylabel("N [m]")
title("motion trajectory")
grid on
legend show
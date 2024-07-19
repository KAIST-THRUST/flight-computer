clear; clc; close all;

data = xlsread('gyro_signal_data.xls');

Ts = 0.009994; % sec
f_cutoff = 5; % Hz
tau = 1 / 2/pi / f_cutoff; % time constant
alpha = Ts / (tau + Ts)

N = length(data);

t = data(:, 1); % sec
gyro_x = data(:, 2); % rad/s
gyro_y = data(:, 3); % rad/s
gyro_z = data(:, 4); % rad/s

filtered_gyro_x = zeros(N, 1);
filtered_gyro_y = zeros(N, 1);
filtered_gyro_z = zeros(N, 1);
filtered_gyro_x(1) = gyro_x(1);
filtered_gyro_y(1) = gyro_y(1);
filtered_gyro_z(1) = gyro_z(1);

for i = 2:1:N
    filtered_gyro_x(i) = alpha * gyro_x(i) + (1 - alpha) * filtered_gyro_x(i-1);
    filtered_gyro_y(i) = alpha * gyro_y(i) + (1 - alpha) * filtered_gyro_y(i-1);
    filtered_gyro_z(i) = alpha * gyro_z(i) + (1 - alpha) * filtered_gyro_z(i-1);
end

dt = diff(t);
angular_acc_x = diff(gyro_x) ./ dt;
angular_acc_y = diff(gyro_y) ./ dt;
angular_acc_z = diff(gyro_z) ./ dt;
filtered_angular_acc_x = diff(filtered_gyro_x) ./ dt;
filtered_angular_acc_y = diff(filtered_gyro_y) ./ dt;
filtered_angular_acc_z = diff(filtered_gyro_z) ./ dt;

figure;
subplot(3, 1, 1);
hold on
plot(t, gyro_x, 'DisplayName', 'gyro_x')
plot(t, filtered_gyro_x, 'DisplayName', 'filtered-gyro_x')
legend show
title("Angular Rate: Original vs. Filtered")

subplot(3, 1, 2);
hold on
plot(t, gyro_y, 'DisplayName', 'gyro_y')
plot(t, filtered_gyro_y, 'DisplayName', 'filtered-gyro_y')
legend show

subplot(3, 1, 3);
hold on
plot(t, gyro_z, 'DisplayName', 'gyro_z')
plot(t, filtered_gyro_z, 'DisplayName', 'filtered-gyro_z')
legend show

% Plot angular acceleration obtained by finite difference
figure; 
subplot(3, 1, 1);
hold on
plot(t(1:end-1), angular_acc_x, 'DisplayName', 'angular-acc_x')
plot(t(1:end-1), filtered_angular_acc_x, 'DisplayName', 'filtered-angular-acc_x')
legend show
title("Angular Acceleration: Original vs. Filtered")

subplot(3, 1, 2);
hold on
plot(t(1:end-1), angular_acc_y, 'DisplayName', 'angular-acc_-')
plot(t(1:end-1), filtered_angular_acc_y, 'DisplayName', 'filtered-angular-acc_y')
legend show

subplot(3, 1, 3);
hold on
plot(t(1:end-1), angular_acc_z, 'DisplayName', 'angular-acc_z')
plot(t(1:end-1), filtered_angular_acc_z, 'DisplayName', 'filtered-angular-acc_z')
legend show
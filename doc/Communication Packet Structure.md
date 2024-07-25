# Communication Packet Structure
## ST_BOOT

### Flight Computer -> Ground Station

| Field Name     | Type | Size (Bytes) | Description |
|----------------|--------------|--------------|-------------|
| Current State  | `byte` | 1            | The current state of the flight computer. |
| Total Bytes    | - | 1            | Total number of bytes in the table. |

### Ground Station -> Flight Computer
None

## ST_STANDBY

### Flight Computer -> Ground Station

| Field Name     | Type | Size (Bytes) | Description |
|----------------|----------------|--------------|-------------|
| Current State  | `byte` | 1            | The current state of the flight computer. |
| Time Stamp  | `uint32_t` | 4            | Sensor measurement time stamp (ms). |
| GPS Data  | `float` | 32            | lat (deg), lon (deg), alt (m), geoid (m), lat_ls (deg), lon_ls (deg), alt_ls (m), geoid_ls (m) |
| BNO055 Data | `float` | 40            | acc_x (m/s2), acc_y (m/s2), acc_z (m/s2), g_x (rad/s), g_y (rad/s), g_z (rad/s), q_x, q_y, q_z, q_w (all of them are imu raw measurements). |
| BMP Data  | `float` | 16            | atm_p (hPa), atm_T (degC), atm_p_ls (hPa), atm_T_ls (degC). |
| ADC Data | `float` | 8            | V_bat (V), P_chamber (barg). |
| Total Bytes    | - |101            | Total number of bytes in the table. |

### Ground Station -> Flight Computer
None

## ST_BURN

### Flight Computer -> Ground Station
| Field Name     | Type | Size (Bytes) | Description |
|----------------|----------------|--------------|-------------|
| Current State  | `byte` | 1            | The current state of the flight computer. |
| Time Stamp  | `uint32_t` | 4            | Sensor measurement time stamp (ms). |
| Attitude | `float` | 16 | q_x, q_y, q_z, q_w |
| Nav Solution  | `float` | 24            | pos_E (m), pos_N (m), pos_U (m), vel_E (m/s), vel_N (m/s), vel_U (m/s) |
| ADC Data | `float` | 4            | P_chamber (barg). |
| Total Bytes    | - | 49            | Total number of bytes in the table. |

### Ground Station -> Flight Computer
| Field Name     | Type | Size (Bytes) | Description |
|----------------|----------------|--------------|-------------|
| Emergency Deloyment Signal | `int` | 1            | True / False. |
| Total Bytes    | - | 1            | Total number of bytes in the table. |


## ST_COAST

### Flight Computer -> Ground Station
| Field Name     | Type | Size (Bytes) | Description |
|----------------|----------------|--------------|-------------|
| Current State  | `byte` | 1            | The current state of the flight computer. |
| Time Stamp  | `uint32_t`  | 4            | Sensor measurement time stamp (ms). |
| Attitude | `float` | 16 | q_x, q_y, q_z, q_w |
| Nav Solution  | `float` | 24            | pos_E (m), pos_N (m), pos_U (m), vel_E (m/s), vel_N (m/s), vel_U (m/s) |
| Total Bytes    | - | 45            | Total number of bytes in the table. |

### Ground Station -> Flight Computer
| Field Name     | Type | Size (Bytes) | Description |
|----------------|----------------|--------------|-------------|
| Emergency Deloyment Signal | `int` | 1            | True / False. |
| Total Bytes    | - | 1            | Total number of bytes in the table. |

## ST_DESCENT

### Flight Computer -> Ground Station
| Field Name     | Type | Size (Bytes) | Description |
|----------------|----------------|--------------|-------------|
| Current State  | `byte` | 1            | The current state of the flight computer. |
| Time Stamp  | `uint32_t`  | 4            | Sensor measurement time stamp (ms). |
| Attitude | `float` | 16 | q_x, q_y, q_z, q_w |
| Nav Solution  | `float` | 24            | pos_E (m), pos_N (m), pos_U (m), vel_E (m/s), vel_N (m/s), vel_U (m/s) |
| Total Bytes    | - | 45            | Total number of bytes in the table. |


### Ground Station -> Flight Computer
None

## ST_LANDED

### Flight Computer -> Ground Station
| Field Name     | Type | Size (Bytes) | Description |
|----------------|----------------|--------------|-------------|
| Current State  | `byte` | 1            | The current state of the flight computer. |
| Time Stamp  | `uint32_t` | 4            | Sensor measurement time stamp (ms). |
| GPS | `float` | 16 | lat (deg), lon (deg), alt (m), geoid (m) |
| Total Bytes    | - | 21            | Total number of bytes in the table. |

### Ground Station -> Flight Computer
None

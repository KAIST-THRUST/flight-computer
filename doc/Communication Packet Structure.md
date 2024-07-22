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
| Time Stamp  | `uint32_t` | 4            | Sensor measurement time stamp. |
| GPS Data  | `float` | 32            | lat, lon, alt, geoid, lat_ls, lon_ls, alt_ls, geoid_ls |
| BNO055 Data | `float` | 40            | acc_x, acc_y, acc_z, g_x, g_y, g_z, q_x, q_y, q_z, q_w (all of them are imu raw measurements). |
| BMP Data  | `float` | 16            | atm_p, atm_T, atm_p_ls, atm_T_ls. |
| ADC Data | `float` | 8            | V_bat, P_chamber. |
| Total Bytes    | - |101            | Total number of bytes in the table. |

### Ground Station -> Flight Computer
None

## ST_BURN

### Flight Computer -> Ground Station
| Field Name     | Type | Size (Bytes) | Description |
|----------------|----------------|--------------|-------------|
| Current State  | `byte` | 1            | The current state of the flight computer. |
| Time Stamp  | `uint32_t` | 4            | Sensor measurement time stamp. |
| Attitude | `float` | 16 | q_x, q_y, q_z, q_w |
| Nav Solution  | `float` | 24            | pos_E, pos_N, pos_U, vel_E, vel_N, vel_U |
| ADC Data | `float` | 4            | P_chamber. |
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
| Time Stamp  | `uint32_t`  | 4            | Sensor measurement time stamp. |
| Attitude | `float` | 16 | q_x, q_y, q_z, q_w |
| Nav Solution  | `float` | 24            | pos_E, pos_N, pos_U, vel_E, vel_N, vel_U |
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
| Time Stamp  | `uint32_t`  | 4            | Sensor measurement time stamp. |
| Attitude | `float` | 16 | q_x, q_y, q_z, q_w |
| Nav Solution  | `float` | 24            | pos_E, pos_N, pos_U, vel_E, vel_N, vel_U |
| Total Bytes    | - | 45            | Total number of bytes in the table. |


### Ground Station -> Flight Computer
None

## ST_LANDED

### Flight Computer -> Ground Station
| Field Name     | Type | Size (Bytes) | Description |
|----------------|----------------|--------------|-------------|
| Current State  | `byte` | 1            | The current state of the flight computer. |
| Time Stamp  | `uint32_t` | 4            | Sensor measurement time stamp. |
| GPS | `float` | 16 | lat, lon, alt, geoid |
| Total Bytes    | - | 21            | Total number of bytes in the table. |

### Ground Station -> Flight Computer
None

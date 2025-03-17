# Air Quality Index (AQI) Checker

## Overview
The **AQI Checker** is a simple C program that fetches real-time **Air Quality Index (AQI)** data for a given city using the **OpenWeatherMap API**. It provides insights into air pollution levels and helps users understand air quality conditions.

---

##  Features
- Fetches **real-time AQI data** for any city.
- Displays pollutant levels (**CO, PM2.5, NO2, O3, SO2, PM10**).
- Uses **cURL** to fetch data from the API.
- Shows **AQI categories** for easy understanding.

---

##  Requirements
- **C Compiler (GCC)**
- **cURL** installed on your system
- **API Key** from [OpenWeatherMap](https://home.openweathermap.org/api_keys)

---

##  Setup & Usage

### Step 1: API Key Configuration
1. **Create a `.env` file** in the project directory.
2. Add your API key inside the `.env` file:
   ```
   API_KEY=your_api_key_here
   ```

### Step 2: Build the Program
```sh
gcc -o aqi_checker aqi_checker.c
```

### Step 3: Run the Program
```sh
./aqi_checker
```

###  Step 4: Enter a City Name
When prompted, enter the city name to fetch AQI data.

---

## Example Output
```
========= AIR QUALITY INDEX SYSTEM =========
Enter city name: Delhi
Fetching location data...
Fetching AQI data...

Air Quality Index (AQI) for Delhi:
--------------------------------------
CO: 0.3 ppm
PM2.5: 12 µg/m³
NO2: 20 ppb
O3: 30 ppb
SO2: 5 ppb
AQI Level: Moderate (3)
--------------------------------------
```

---

##  License
This project is open-source and free to use.

---

##  Contributing
Feel free to **fork** this repository, create a new **branch**, and submit a **pull request** if you want to improve this project!

---

##  Contact
For any issues or suggestions, open an **issue** or reach out at **avneeshsrivastava70@gmail.com**.

---

**Happy Coding! **

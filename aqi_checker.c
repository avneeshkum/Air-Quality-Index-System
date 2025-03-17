#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define color codes for better output appearance
#define RESET_COLOR "\x1B[0m"
#define GREEN_COLOR "\x1B[32m"
#define YELLOW_COLOR "\x1B[33m"
#define RED_COLOR "\x1B[31m"
#define CYAN_COLOR "\x1B[36m"
#define BOLD_TEXT "\x1B[1m"

// Function to execute a system call and capture the output
int execute_command(const char *command, char *result, int result_size) {
    FILE *fp = popen(command, "r");
    if (fp == NULL) {
        perror("Error executing command");
        return -1;
    }
    fread(result, 1, result_size - 1, fp);
    pclose(fp);
    return 0;
}

// Function to simulate a loading effect
void loading_animation(const char *message) {
    printf("%s...\n", message);
    fflush(stdout);
}

// Function to display AQI meaning
void display_aqi_legend() {
    printf("\n%s%sAir Quality Index (AQI) Levels:%s\n", BOLD_TEXT, CYAN_COLOR, RESET_COLOR);
    printf("--------------------------------------\n");
    printf("%s1 (Good)%s → Air quality is satisfactory.\n", GREEN_COLOR, RESET_COLOR);
    printf("%s2 (Fair)%s → Acceptable air quality.\n", YELLOW_COLOR, RESET_COLOR);
    printf("%s3 (Moderate)%s → May affect sensitive individuals.\n", RED_COLOR, RESET_COLOR);
    printf("%s4 (Poor)%s → Unhealthy air for some groups.\n", RED_COLOR, RESET_COLOR);
    printf("%s5 (Very Poor)%s → Health effects for everyone.\n", RED_COLOR, RESET_COLOR);
    printf("--------------------------------------\n");
}

int main() {
    char city[100];
    char api_key[] = "Your Api Key"; // Replace with your OpenWeatherMap API key
    char url[500], command[600], choice;
    char response[2000];

    printf("%s%s=========AIR QUALITY INDEX SYSTEM============\n", BOLD_TEXT, CYAN_COLOR);
    printf("  Welcome to the Air Quality Index Checker \n");
    printf("=============================================%s\n\n", RESET_COLOR);

    do {
        // Step 1: Get city name from user
        printf("%sEnter city name:%s ", BOLD_TEXT, RESET_COLOR);
        fgets(city, sizeof(city), stdin);
        city[strcspn(city, "\n")] = 0; // Remove newline character

        // Step 2: Construct Geocoding API URL
        snprintf(url, sizeof(url), "http://api.openweathermap.org/data/2.5/weather?q=%s&appid=%s", city, api_key);

        // Step 3: Fetch coordinates
        snprintf(command, sizeof(command), "curl -s \"%s\"", url);
        loading_animation("Fetching location data");
        memset(response, 0, sizeof(response));

        if (execute_command(command, response, sizeof(response)) != 0) {
            printf("%sError fetching data. Please try again.%s\n", RED_COLOR, RESET_COLOR);
            continue;
        }

        // Step 4: Parse response to extract latitude and longitude
        float lat, lon;
        if (sscanf(response, "{\"coord\":{\"lon\":%f,\"lat\":%f", &lon, &lat) == 2) {
            printf("\n%sLocation Found:%s %s\n", BOLD_TEXT, RESET_COLOR, city);
            printf("Latitude: %.4f, Longitude: %.4f\n\n", lat, lon);

            // Step 5: Fetch AQI data
            snprintf(url, sizeof(url), "http://api.openweathermap.org/data/2.5/air_pollution?lat=%.4f&lon=%.4f&appid=%s", lat, lon, api_key);
            snprintf(command, sizeof(command), "curl -s \"%s\"", url);
            loading_animation("Fetching AQI data");
            memset(response, 0, sizeof(response));

            if (execute_command(command, response, sizeof(response)) != 0) {
                printf("%sError fetching AQI data. Try again.%s\n", RED_COLOR, RESET_COLOR);
                continue;
            }

            // Step 6: Parse AQI data
            int aqi;
            float co, pm25, no2, o3, so2, pm10;

            if (sscanf(response, "{\"coord\":{\"lon\":%*f,\"lat\":%*f},\"list\":[{\"main\":{\"aqi\":%d},\"components\":{\"co\":%f,\"no\":%*f,\"no2\":%f,\"o3\":%f,\"so2\":%f,\"pm2_5\":%f,\"pm10\":%f}}", &aqi, &co, &no2, &o3, &so2, &pm25, &pm10) == 7) {

                // Step 7: Display AQI results
                const char *aqi_status;
                if (aqi == 1) aqi_status = GREEN_COLOR "Good" RESET_COLOR;
                else if (aqi == 2) aqi_status = YELLOW_COLOR "Fair" RESET_COLOR;
                else if (aqi == 3) aqi_status = RED_COLOR "Moderate" RESET_COLOR;
                else if (aqi == 4) aqi_status = RED_COLOR "Poor" RESET_COLOR;
                else aqi_status = RED_COLOR "Very Poor" RESET_COLOR;

                printf("\n%sAir Quality Index (AQI) for %s:%s\n", BOLD_TEXT, city, RESET_COLOR);
                printf("--------------------------------------------\n");
                printf("AQI Value: %d\n", aqi);
                printf("Air Quality Status: %s\n", aqi_status);
                printf("--------------------------------------------\n");

                // Step 8: Display Pollutant Levels
                printf("\n%sDetailed Pollutant Levels:%s\n", BOLD_TEXT, RESET_COLOR);
                printf("%-25s %-20s\n", "Pollutant", "Concentration (\u00b5g/m³)");
                printf("------------------------------------------------\n");
                printf("%-25s %-20.2f\n", "Carbon Monoxide (CO)", co);
                printf("%-25s %-20.2f\n", "Fine Particulate Matter (PM2.5)", pm25);
                printf("%-25s %-20.2f\n", "Nitrogen Dioxide (NO2)", no2);
                printf("%-25s %-20.2f\n", "Ozone (O3)", o3);
                printf("%-25s %-20.2f\n", "Sulfur Dioxide (SO2)", so2);
                printf("%-25s %-20.2f\n", "Particulate Matter (PM10)", pm10);
                printf("------------------------------------------------\n");

                display_aqi_legend();
            } else {
                printf("%sFailed to parse AQI data.%s\n", RED_COLOR, RESET_COLOR);
            }
        } else {
            printf("%sCould not find coordinates for %s.%s\n", RED_COLOR, city, RESET_COLOR);
        }

        // Ask if user wants to check another city
        printf("\n%sWould you like to check another city? (y/n):%s ", BOLD_TEXT, RESET_COLOR);
        scanf(" %c", &choice);
        getchar(); // Consume newline character

    } while (choice == 'y' || choice == 'Y');

    return 0;
}

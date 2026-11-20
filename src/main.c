#include <pigpio.h>
#define LED_PIN 17
#include <stdio.h> 
void logEvent(const char *event, int occupancy, int capacity){
FILE *file = fopen("data/occupancy_log.csv", "a");
if (file == NULL){
printf("Error opening log file.\n");
return;
}
fprintf(file, "%s,%d,%d\n", event, occupancy, capacity);
fclose(file);
}
void updateLED(int occupancy, int capacity) {
if (occupancy >= capacity){
gpioWrite(LED_PIN, 1);
printf("LED Status: ON\n");
} else {
gpioWrite(LED_PIN, 0);
printf("LED Status: Off\n");
}
}

int main()
{
int gpioReady = (gpioInitialise() >= 0);
if (gpioReady) {
gpioSetMode(LED_PIN, PI_OUTPUT);
gpioWrite(LED_PIN, 0); 
} else { 
printf ("GPIO unavailable. LED simulation only.\n");
}

	int capacity = 5;
	int occupancy = 0;;
	int choice;

//Keep program running until user exits

while (1) 
{
	printf("\nStudy Room Occupancy Tracker\n");
	printf("Current Occupancy: %d/%d\n", occupancy, capacity);
if (occupancy == capacity)
{
printf("Status: ROOM FULL\n");;
}
else
{
printf ("STATUS: SPACE AVAILABLE\n");
}
if (gpioReady) {
updateLED(occupancy, capacity); 
} else { 
printf("LED Status : %s\n", occupancy >= capacity ? "ON" : "OFF");
}
printf("\n1. Student enters\n");
printf("2. Student exits\n");
printf("3. Change room capacity\n");
printf("4. Exit Program\n");
printf("Enter choice: ");

if (scanf("%d", &choice) != 1)
{
printf ("Invalid input.\n");
return 1;
}

if (choice == 1) 
{
if (occupancy < capacity) 
{
occupancy ++;
printf("Student entered.\n");
logEvent("ENTER", occupancy, capacity);
}
else
{
printf("Room is full. Entry denied.\n");
}
} 

else if (choice == 2) {
if (occupancy > 0) {
occupancy--;
printf("Student exited.\n");
logEvent("EXIT", occupancy, capacity);
} else {
printf("room is empty.\n");
}
}

else if (choice == 3) {
int newCapacity;
printf("Enter new capacity: ");

if(scanf("%d", &newCapacity) != 1) {
printf("Invalid input.\n");
return 1;
}

if (newCapacity > 0 && newCapacity >= occupancy) {
capacity = newCapacity;
printf("Capacity updated.\n");
logEvent("CAPACITY_CHANGED", occupancy, capacity);
} else {
printf("Invalid capacity.\n");
}
}

else if (choice == 4) {
printf("Exiting program.\n");
break;
}
else {
printf("Invalid choice.\n");
}
}
if (gpioReady) {
gpioWrite(LED_PIN, 0);
gpioTerminate();
}
return 0;
}


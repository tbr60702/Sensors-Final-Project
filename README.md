# Sensors-Final-Project

This contains my experimental and final code (pending more testing on an actual grill) for the project

The goal of this project was to create a working prototype of a grill/smoker controller. It should do 5 things.

	1. Read the temperature of the food
    
	2. Read the temperature of the grill
    
	3. Allow the person grilling (ME) to input a temperature that I consider the food to be done at.
    
	4. Allow the person grilling (ME) to input a temperature that I want the grill to maintain.
    
	5. Use the 4 temperatures above to automatically control the grill and display active temperatures
    
To accomplish numbers 1-2, I plan to use type K thermocouples. I chose K type because they're readily available and cheap. The food temp will be measured with a probe style thermocouple, obviously.


For 3-4, I plan on using an LCD screen and buttons as a menu:
  
  ON STARTUP: (Press right/left to change screens)
        
		Screen 1 - "FOOD TEMPERATURE SETPOINT" & number value displayed - increase/decrease with up/down buttons
        
		Screen 2 - "GRILL TEMPERATURE SETPOINT" & number value displayed - increase/decrease with up/down buttons
        
		Screen 3 - "PRESS SELECT TO RUN" - pressing select exits setup and starts running the grill controler
        
  GRILL CONTROLLER SCREEN: (after select has been pressed on previous screen 3)
        
		line 1 - "FOOD TEMP     GRILL TEMP"
        
		line 2 - "XXX.XX        XXX.XX" 
        
		Fan control decribed below running in the backround

Now accomplishing number 5 requires some considerations. 1. How can the fire be stoked? 2. How can it be choked off? 3. How hard should it be stoked? 4. How do you prevent too hot a fire?

		For #1, I chose to use a fan. 

		For #2 & 4, my solution is passive. I plan to use a small fan, and block off the rest of the air intake, and make the exhaust valve narrow as well. This means little air will feed the fire if the fan is off, naturally and passiviley choking it off to limit tempereature.

		For #3, it will require testing to fine tune, and may even change depending on the grill. I plan to structure my code for the fan to blow air for shorter intervals when the grill temp is closer to the set temperature.

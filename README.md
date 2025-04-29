# Digital Stopwatch with Count Modes.
![image](https://github.com/user-attachments/assets/fbe68bc4-8c35-4962-bf6a-657117d75ba3)

# Project Overview:
This project aims to design and implement a digital stopwatch using the ATmega32 microcontroller and six multiplexed seven-segment displays (common anode). The stopwatch offers two operational modes:

1. Increment Mode (Default): The stopwatch counts upwards, displaying hours, minutes, and seconds.
2. Countdown Mode: The stopwatch counts downwards from a user-defined time, functioning as a countdown timer.

The system includes features like reset, pause, resume, mode toggle, and time adjustments via push buttons. LED indicators and a buzzer provide visual and auditory alerts.

# Features:
- Reset Function: Resets the time to zero in both modes.
- Pause Function: Pauses the counting, allowing the stopwatch to hold its current value.
- Resume Function: Resumes counting from the paused time.
- Mode Toggle: Switches between increment mode and countdown mode.
- Time Adjustment: Allows the user to adjust the hours, minutes, and seconds when in countdown mode.
- Alarm Function: Triggers a buzzer when the countdown reaches zero.
- LED Indicators: Indicates the current mode of the stopwatch:

  1. Red LED for increment mode.
  2. Green LED for countdown mode.


# Hardware Components:
1. Microcontroller: ATmega32.
2. Display: Six multiplexed seven-segment displays (common anode), driven by a 7447 BCD to seven-segment decoder connected to PORTC.
3. Push Buttons:
    - Reset: Connected to PD2 (INT0) with an internal pull-up resistor.
    - Pause: Connected to PD3 (INT1) with an external pull-up resistor.
    - Resume: Connected to PB2 (INT2) with an internal pull-up resistor.
    - Mode Toggle: Connected to PB7.
    - Hour Adjustment

4. Alarm/Buzzer: Connected to PD0, activated when the countdown reaches zero.
5. LED Indicators:
    - Red LED for increment mode (connected to PD4).
    - Green LED for countdown mode (connected to PD5).
  

# Operation Description:
The stopwatch operates in two distinct modes, increment mode (default) and countdown mode, with the following functionalities:

1. Increment Mode (Default Mode):
    - Upon powering the system, the stopwatch starts in increment mode.
    - The time counts upward from zero, displaying hours, minutes, and seconds across six seven-segment displays.
    - The red LED (connected to PD4) turns on, indicating that the stopwatch is in increment mode.
    - The stopwatch continues counting until paused or reset.
    - The Reset Button (connected to PD2, INT0) resets the time to zero.
    - The Pause Button (connected to PD3, INT1) pauses the stopwatch.
    - The Resume Button (connected to PB2, INT2) resumes the counting from the paused state.
  
2. Countdown Mode:
    - The stopwatch can switch to countdown mode by pressing the Mode Toggle Button (connected to PB7).
    - In countdown mode, the user can set a specific time to count down from, using the adjustment buttons for hours, minutes, and seconds
    - The Green LED (connected to PD5) turns on, indicating that the stopwatch is in countdown mode.
    - After setting the desired time, the user can press the Resume Button (connected to PB2, INT2) to start the countdown.
    - When the countdown reaches zero, an alarm buzzer (connected to PD0) is activated to alert the user.

3. Switching Between Modes:
    - The stopwatch can toggle between increment and countdown modes using the Mode Toggle Button (connected to PB7).
    - The corresponding LED indicator (red for increment mode, yellow for countdown mode) will light up to reflect the current mode.

4. Time Adjustment in Countdown Mode:
    - In countdown mode, the stopwatch must be paused to adjust the time.
    - The user can then increment or decrement the hours, minutes, and seconds using the dedicated adjustment buttons.
    - Once the adjustments are made, pressing the Resume Button starts the countdown.

5. Buzzer Activation:
    - The buzzer is triggered when the countdown timer reaches zero, signaling the end of the countdown.
    - The buzzer remains on until manually reset or toggled by the user.

# Door_Locker_Security_Systems
```
This project is designed and implemented based on the layered architecture
with the specifications of Door Locker Security System  to unlock a door using a password.
```
## **Using two ATmega32 Microcontrollers with frequency 8Mhz.**

**Mc1 → HMI_ECU (Human Machine Interface)** with 2x16 LCD and 4x4 keypad.

**HMI_ECU** is just responsible interaction with the user just take inputs through keypad and display messages on the LCD.


![image](https://github.com/YoussefGobran/Door_Locker_Security_Systems/assets/132088403/d4c8a61c-5738-490e-ac09-c0e07c7c291b)

**Mc2 → Control_ECU** with EEPROM, Buzzer, and Dc-Motor.

**CONTROL_ECU** is responsible for all the processing and decisions in the system like password checking, open the door and activate the system alarm.

![image](https://github.com/YoussefGobran/Door_Locker_Security_Systems/assets/132088403/90fbf803-525d-434d-9235-019aeeb18437)

## **System Sequence:**

```
Step1 – Create a System Password
```
- The LCD should display “Please Enter Password”.
- Enter a password consists of 5 numbers, Display * in the screen for each number.
- Press **enter** button (choose any button in the keypad as enter button).
- Ask the user to renter the same password for confirmation by display this message
    “Please re-enter the same Pass”.
- Enter a password consists of 5 numbers, Display * in the screen for each number.
- Press **enter** button (choose any button in the keypad as enter button).
- **HMI_ECU** should send the two passwords to the **Control_ECU** through the **UART**.


- If the two passwords are **matched** then the system has a password now and save it
    inside the **EEPORM** and go to **Step 2**.
- If the two passwords are **unmatched** then repeat **step 1** again.
```
Step2 - Main Options
```
- The LCD will always display the main system option:
  
![image](https://github.com/YoussefGobran/Door_Locker_Security_Systems/assets/132088403/7e0494aa-c6c5-4b04-9627-50a0933d3a76)

```
Step3 - Open Door (+)
```

- The LCD should display “Please Enter Password”.
- Enter the password then press **enter** button (choose any button in the keypad as
    enter button).
- **HMI_ECU** should send the Password to the **Control_ECU** and it should compare it
    with the one saved in the **EEPROM**.
- if two passwords are **matched** :
    - **rotates** motor for 15-seconds **CW** and display a message on the screen
       “Door is Unlocking”
    - hold the motor for 3-seconds.
    - **rotates** motor for 15-seconds **A-CW** and display a message on the screen
       “Door is Locking”

```
Step 4 - Change Password (-)
```

- The LCD should display “Please Enter Password”.
- Enter the password then press **enter** button (choose any button in the keypad as
    enter button).
- **HMI_ECU** should send the Password to the **Control_ECU** and it should compare it
    with the one saved in the **EEPROM**.
- if two passwords are matched:
    - Repeat Step 1.
```
Step 5
```
- if the two passwords are **unmatched** at step 3 (+ : Open Door) or step 4 (- : Change
    Password)
- Ask the user one more time for the password.
- The LCD should display “Please Enter Password” like that:
- Enter the password then press **enter** button (choose any button in the keypad as
    enter button).
- **HMI_ECU** should send the password to the **Control_ECU** and it should compare it
    with the one saved in the **EEPROM**.
- if two passwords are matched then open the door or change the password in steps
    3 and 4.
- If the two passwords are **not matched** again then ask the user **one last time** for the
    password.
- if two passwords are matched then open the door or change the password in steps
    3 and 4.


- If the two passwords are not matched for the **third consecutive** time, then:
    - Activate Buzzer for 1-minute.
    - Display error message on LCD for 1 minute.
    - System should be locked no inputs from Keypad will be accepted during
       this time period.
    - Go to Step 2 the main options again.






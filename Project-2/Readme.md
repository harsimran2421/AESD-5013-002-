Execution steps for BBG code
  - cd Project-2/src_file/
  - make
  - ./gpio.sh(to export the gpio pins used in main code)
  - ./uart-main

Execution steps for Freertos
  - import CCS project
  - build
  - debug

Sensors interfaced at remote node
  - HCSR04 ultrasonic sensor
  - MQ3 Alcohol sensor

Peripherals Used at remote node
  - ADC 
  - GPIO
  - UART
  - I2C

Peripherals used at control node
  - GPIO
  - UART

Sequence of Operation
  - Sensor values are read at the remote node, stored in a structure and the structure is then added to a queue.
  - Next the structure is sent to the control node(BBG) via UART communication.
  - The Control node makes decision based on sensor values and sends a reply back to the remote node.
  - the remote node drives the output devices based on the command recieved from the control node.

Main tasks at remote node

  Ultrasonic sensor task 
    - Sensor value is taken after every 100 ms and sent to the control node via a structure.
    - Control node compares the sensor value with the threshold set and replies with the appropriate command.
    - A motor and a led device are interfaced as output devices. If the threshold is crossed the motor stops and the LED is pin is set high indicating the threshold.
    - Since the application focusses on vehicale safety the motor indicates the vehicle motion and if anything is detected within 10 cm the motor stops.
    - Since its a critical application in case the connection with the control node terminates the remote node takes over the decision making role and stops the motor in case a collision is detected.

  Alcohol sensor task
    - Sensor value is taken after every 100 ms and sent to the control node via a structure.
    - Control node compares the sensor value with the threshold set and replies with the appropriate command.
    - A buzzer and a led device are interfaced as  output devices for this sensor. If the alcohol thresold is crossed the buzzer and led pin are set high.
    - Since the application focusses on vehicle safety the buzzer pin is set high for small amount of alcohol content to for safety measures.
    - The remote node takes over the decision making role to in case the communication with the control node is hampered.

Main tasks at control node
    
  Decision task
    - This task sends the appropriate command back to remote node based on the sensore value recieved for remote node

  Logging task
    - This task logs all the relevant information to a log file without hampering the control node operation.
    - The log file is retained in case of power loss or any other issues to view after the node reboots.
    - All the alert messages data values and other activities are logged in a log file.

  Led matrix task
    - 8x8 led matrix is interfaced to display the operational status of the device.
        1. Both thr sensors are operational and UART communication is working properly.
        2. If any one of the sensor is disconnected.
        3. If both the sensors are disconnected. 
        4. If uart communication is interrupted.

Extra credits Implemented
  - Control node operational status using LED matrix and on board LEDS
  - Control node collecting remote logs after restoration of communication achieved with the help of queues
  - 2 Non-I2C based sensors used.
  - Non LED output devices used(1. Buzzer, 2. Motor)
  - 5 output devices used(3 leds and 2 other output devices)
  - Fast recovery of the system, resumes communication within 100 ms after the connection is restored. Remote node takes over the decision making role in case the UART communication is hampered.


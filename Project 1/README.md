Execution steps
  - cd /src_file/
  - make cross (if you need a functional executable which does not include the debug print statements in case of system failure detailed error information will not be printed)
   - ./cross-main LOG_FILE_NAME
  - make debug (if you need a functional executable which includes the debug print statements in case of system failure detailed error information will be printed)
   - ./debug-cross LOG_FILE_NAME
  - make client-cross (to create executable of an external process which requests data from our function)
   - ./client    

Execution Steps
  - cd/UnitTest/
  - make unittest (generates a "test" executable for Beagle Bone Green)
   - ./test to run all the unit tests for LIGHT,TEMPERATURE and CONVERSION 

Note: The threshold values defining the system state as Light/dark or Cool/Hot depends upon the threshold hold values set as 0-20 Lux for Dark and less than 25C for "COOL".
The default units for temperature is set as Celsius.

Initailly, at runtime, all the Unittests are ran,testing complete functionality for the Light sensor, Temperature sensor and Unit Conversion.Testing includes configuring, writing and reading various 
functionality for light and temperature sensors, testing unit conversion. Once all the testing modules run successfully, the system proceeds further craeting threads for different tasks,light, temp, logging and socjet task.

heartbeat implementation: pthread_cond_timedwait api are used to implement the heartbeat requirement


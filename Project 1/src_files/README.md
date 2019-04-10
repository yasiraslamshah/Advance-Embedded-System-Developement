Execution steps
  - cd /src_file/
  - make cross (if you need a functional executable which does not include the debug print statements in case of system failure detailed error information will not be printed)
    ./cross-main LOG_FILE_NAME
  - make debug (if you need a functional executable which includes the debug print statements in case of system failure detailed error information will be printed)
    ./debug-cross LOG_FILE_NAME
  - make client-cross (to create executable of an external process which requests data from our function)
    

Execution Steps
  cd/UnitTest/
  make unittest (generates a "test" executable for Beagle Bone Green)
  Execute ./test to run all the unit tests for LIGHT,TEMPERATURE and CONVERSION 



Bussiness logic is implemented here.

* configurationSubsystem: Subsystem in charge of communicating with the user and modifying the datalogger's internal parameters. 
* loggerMetadata: The datalogger internal parameters are defined here.
* loggerSubsystem: Subsystem in charge of handling the storage of information.  
* main: Application starts here.
* processingSubsystem: Subsystem in charge of getting information from the sensors, process them and sending the information to other subsystems.
* connectivitySubsystem: Subsystem in charge of managing Internet communication, gets info from other subsystems and transmits them to the internet.
* utilities: Generic functions that might be used by other subsystems.
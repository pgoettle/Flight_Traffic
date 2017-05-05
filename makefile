CC = g++
Flags = -Wall -g -std=c++14
objects = Parser.o Flight.o Flight_Location.o Flight_Schedule.o Time_Data.o Cell.o Cell_Tier.o Spacecraft.o Antenna.o Mobile_Terminal.o Neighbor_Satellite.o

traffic : $(objects)
	$(CC) $(Flags) -o traffic $(objects)
Parser.o : Parser.cpp
	$(CC) $(Flags) -c Parser.cpp
Flight.o : Flight.hpp Flight.cpp
	$(CC) $(Flags) -c Flight.cpp
Flight_Location.o : Flight_Location.cpp Flight_Location.hpp
	$(CC) $(Flags) -c Flight_Location.cpp
Flight_Schedule.o : Flight_Schedule.cpp Flight_Schedule.hpp
	$(CC) $(Flags) -c Flight_Schedule.cpp
Time_Data.o : Time_Data.cpp Time_Data.hpp
	$(CC) $(Flags) -c Time_Data.cpp
Cell.o : Cell.cpp Cell.hpp
	$(CC) $(Flags) -c Cell.cpp
Cell_Tier.o : Cell_Tier.cpp Cell_Tier.hpp
	$(CC) $(Flags) -c Cell_Tier.cpp
Spacecraft.o : Spacecraft.cpp Spacecraft.hpp
	$(CC) $(Flags) -c Spacecraft.cpp
Antenna.o : Antenna.cpp Antenna.hpp
	$(CC) $(Flags) -c Antenna.cpp
Mobile_Terminal.o : Mobile_Terminal.cpp Mobile_Terminal.hpp
	$(CC) $(Flags) -c Mobile_Terminal.cpp
Neighbor_Satellite.o : Neighbor_Satellite.cpp Neighbor_Satellite.hpp
	$(CC) $(Flags) -c Neighbor_Satellite.cpp

clean :
	rm traffic $(objects)

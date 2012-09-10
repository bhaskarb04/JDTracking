#
#	Makefile
#   vrViewer
#
#	Created by Iowa State University on 09/07/2012
#	Copyright 2009 Iowa State University. All rights reserved.
#	
#

SRC_DIR=../src
CV_HEADERS1=/home/users/bhaskarb/OpenCV-2.4.2/build/include/
CV_HEADERS2=/home/users/bhaskarb/OpenCV-2.4.2/build/include/opencv
CV_HEADERS3=/home/users/bhaskarb/OpenCV-2.4.2/build/include/opencv2
CV_LIBS=/home/users/bhaskarb/OpenCV-2.4.2/build/lib
# Specify the objects to be generated from source files
tracker = main.o \
	  Tracker.o	
application = myTracker
	
# Setup the targets
all: $(tracker) $(application)


# Setup the OSG and FMOD includes and libs
INCLUDES += -I$(CV_HEADERS1) -I$(CV_HEADERS2) -I$(CV_HEADERS3)
EXTRA_LIBS += -L$(CV_LIBS) $(CV_LIBS)/*.so

# Generate all the object files for all the source files
$(tracker): %.o: $(SRC_DIR)/%.cpp
	g++ $(INCLUDES) -c $< -o $@
	
$(application): $(tracker) $(SRC_DIR)/Tracker.h
	g++ $(tracker) $(EXTRA_LIBS) -o $@ 

# Methods for cleanup
clean:
	rm *.o myTracker


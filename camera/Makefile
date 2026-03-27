CXX      ?= g++
CXXFLAGS  = -std=c++17 -fPIC
MOC       = moc

QT_CFLAGS  = $(shell pkg-config --cflags Qt5Widgets Qt5Gui Qt5Core)
QT_LIBS    = $(shell pkg-config --libs   Qt5Widgets Qt5Gui Qt5Core)
CV_CFLAGS  = $(shell pkg-config --cflags opencv4)
CV_LIBS    = $(shell pkg-config --libs   opencv4)
INCLUDE    = -I./include

SRCS        = main.cpp mainwindow.cpp moc_mainwindow.cpp
TARGET      = inspector

GREEN_SRCS  = main_green.cpp green_window.cpp moc_green_window.cpp
GREEN_TARGET = green_inspector

all: $(TARGET) $(GREEN_TARGET)

moc_mainwindow.cpp: mainwindow.h
	$(MOC) $< -o $@

moc_green_window.cpp: green_window.h
	$(MOC) $< -o $@

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $@ $(SRCS) \
		$(QT_CFLAGS) $(CV_CFLAGS) \
		$(QT_LIBS)   $(CV_LIBS)   \
		$(INCLUDE) -lMVSDK

$(GREEN_TARGET): $(GREEN_SRCS)
	$(CXX) $(CXXFLAGS) -o $@ $(GREEN_SRCS) \
		$(QT_CFLAGS) $(CV_CFLAGS) \
		$(QT_LIBS)   $(CV_LIBS)   \
		$(INCLUDE) -lMVSDK

clean:
	rm -f $(TARGET) $(GREEN_TARGET) moc_mainwindow.cpp moc_green_window.cpp

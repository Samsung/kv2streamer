# KV2Streamer

Kinect V2 Streamer (KV2Streamer) is a library that allows developers to access the new Kinect V2 sensor data and tracking capabilities from a non-Windows OS. 

KV2Streamer provides both a server-side application (KV2Server) that can stream out Kinect V2 data, including tracking data, to multiple client-side applications accessing the client-side API (KV2Client) running on non-Windows OS over LAN.

---
## 1. KV2Server

KV2Streamer provides the server-side application that retrieves data from the Kinect sensor and streams it out to some defined multicast IP.

Unlike it's client-side counterpart, KV2Server is an application on its own.
Once it is build, you can run the executable from the terminal by supplying 
a multicast IP argument which will be used to identify the server.

However, you have to run the application from the gstreamer bin directory.

In summary:

1. Build the application using Visual Studio (see instructions below)
2. Open the command prompt.
3. Change directory:	`$ cd C:\gstreamer\1.0\x86\bin`
4. Run the executable: 	`$ [executable path] <multicast-IP>`, 
	for example:`$ C:\Users\demo\projects\kv2streamer\server\kv2server_VS12\Debug\kv2server_application.exe 224.1.1.1`


### 1.1 How To Build KV2Server

Follow the steps below to build and run the kv2streamer server application. It is assumed that you have installed
the Kinect SDK which you can find here: https://www.microsoft.com/en-us/download/details.aspx?id=44561

1. Install gstreamer from http://gstreamer.freedesktop.org/data/pkg/windows/:  
    gstreamer-1.0-devel-x86-1.4.0.msi  
    gstreamer-1.0-x86-1.4.0.msi

2. Install boost: http://www.boost.org/users/history/version_1_56_0.html

3. Open the Visual Studio Solution file in /server/kv2server_VS12
now and try building. If doesn't work, see below:

Note: The following instructions should have already been setup in the project you cloned, but
in case you need to set things up yourself:

4. Add ALL .cpp files contained in thes following directories to your project (right click on project > Add > Existing Item):  
	
  kv2server-applications/kv2server   
	kv2server-applications/oscpack  (BUT EXCLUDE kv2server-applications/oscpack/ip/posix)	   
	kv2streamer-lib/gst-wrapper   
	kv2streamer-lib/oscpack-wrapper   

5. Configuration Properties > C/C++ > General > Additional Include Directories:  

	C:\Program Files\boost\boost_1_56_0   
	$(KINECTSDK20_DIR)\inc   
	$(GSTREAMER_1_0_ROOT_X86)\include\gstreamer-1.0   
	$(GSTREAMER_1_0_ROOT_X86)\include\glib-2.0   
	$(GSTREAMER_1_0_ROOT_X86)\lib\glib-2.0\include   
	$(SolutionDir)\..   
	$(SolutionDir)\..\kv2server   
	$(SolutionDir)\..\oscpack   
	$(SolutionDir)\..\..\kv2streamer-lib   

6. Configuration Properties > VC++ Directories > Library Directories:  
	
  C:\Program Files\boost\boost_1_56_0\stage\lib   
	$(KINECTSDK20_DIR)\lib\x86   
	$(GSTREAMER_1_0_ROOT_X86)\lib   

7. Configuration Properties > Linker > Input:   
	
  Ws2_32.lib   
	winmm.lib   
	kinect20.lib   
	gstreamer-1.0.lib   
	glib-2.0.lib   
	gobject-2.0.lib   
	gmodule-2.0.lib   
	gthread-2.0.lib   
	gstapp-1.0.lib   

8. Setting up your network. Both machines, server (windows 8) and client (ubuntu), should be connected to the same switch.
  On windows 8: go to > Control Panel > Network and Sharin Center > Ethernet > Properties > TCPIP/IPv4:
  
  IP address: 192.168.8.2  
  Subnet Mask: 255.255.255.0  
  Default Gateway: 192.168.8.250  

### 1.2 The Coordinate Mapper Table Generator Application

You can find the Visual Studio in kv2server-applications/KV2ServerCorrdinateMapperTableGenerator.
Run it and you will find instructions on the terminal. Each Kinect V2 camera has its own unique
calibration that can be saved as a binary using this application. If you copy that binary and put it
in the same directory as your client application executable, you will be able to access that conversion
table that is used to map the color image to the depth image, e.g. if you're using the colored-depth streamer
on the client.

All the project settings should have been setup, but in case you need to do everything yourself,
simply include the Kinect SDK and you should be all set:

1. Configuration Properties > C/C++ > General > Additional Include Directories

	$(KINECTSDK20_DIR)\inc

2. Configuration Properties > VC++ Directories > Library Directories
	
	$(KINECTSDK20_DIR)\lib\x86

3. Configuration Properties > Linker > Input
	
  kinect20.lib

---
## 2. KV2Client API

KV2Streamer also provides the client-side API that developers can use to access data from a specific server once the server is running.

This section describes how you can include the KV2Streamer client-side API
in your application which runs on Ubuntu. 

You're free to use any libraries/frameworks to render the received images but a sample
application is provided in sample-client-applications/codelite. The sample applications
uses glfw and run on the Codelite IDE.

To use the client API of KV2Streamer, perform the following steps on your Ubuntu machine:
 
  1.  Setup and test your network  
  2.  Install all the dependencies: boost, gstreamer, kv2streamer version of oscpack, and the kv2streamer library.  
  3.  Include "KV2ClientCommon.h" in your application.  

### 2.1 KV2Client API Instructions

1. Setting up your network  
	Both machines server (windows 8) and client (ubuntu) will be connected to the same switch.
	On ubuntu: go to > System Settings > Network > choose the right one > Options > IPv4 Settings:  
	Address: 192.168.8.3  
	Netmask: 255.255.255.0  
	Gateway: 192.168.8.250  
	Test by trying to ping each other. Make sure that you're not connected to any other network through the a different network card. 

2. Install boost (1.56.0): http://www.boost.org/doc/libs/1_56_0/more/getting_started/unix-variants.html

3. Install gstreamer-1.0

4. Install a modified version of oscpack that you can find in kv2server-applications/oscpack by running "sudo make install".

5. Install the kv2streamer-lib by using CMake, and don't forget to run "sudo make install" at the end to place the built binaries in /usr/local/lib and the headers in /user/local/include.

6. In your build environment, include the following header search paths:

  kv2streamer&oscpack:  
	  /usr/local/include  
  boost:  
	  /usr/local/include/boost  
  gstreamer:	  
	  /usr/local/include/gstreamer-1.0  
	  /usr/include/glib-2.0  
	  /usr/lib/x86_64-linux-gnu/glib-2.0/include  

7. Also add the following library search paths in your build environment:
	
	/usr/local/lib  
	/usr/local/lib/gstreamer-1.0  

8. And link to the following libraries:
    
    libkv2client  
	  libgst-wrapper  
	  liboscpack-wrapper  
	  liboscpack  
	  libboost_system  
	  libboost_thread  
	  libgstreamer-1.0  
	  libgstapp-1.0  
	  libglib-2.0  
	  libgmodule-2.0  
	  libgthread-2.0  
	  libgobject-2.0  

7. Finally, include the following in your application:  
	"include "KV2ClientCommon.h"

And you should be all set!

### 2.2 Sample Build Instructions: Codelite

A sample application, which you can find in /sample-client-applications, is provided so that you can test the library quickly. Follow the steps below to run the sample application.

  1. Install Codelite
  2. Install glfw: http://www.glfw.org/
  3. Run the sample application. You should be able to see live camera image (color, depth, skeleton, etc.) from the server.

If you decide to create a new projects on codelite from scratch, you must do the steps described above, PLUS you have to link to the following libraries:  libGL, libglfw.

---
# Summary
In short, to use KV2Streamer:

1. Build and run the server application on Windows 8 machine.
2. Create your own project and include the KV2Client API on Ubuntu.
3. Run your application.

# Note
- Only works on the same subnet (Multicast TTL default = 1)
- Server can run while handling multiple clients connecting and disconnecting, but not the other way, i.e. restarting the server requires restarting all clients.
- ./shared folder has codes shared by both the server application and the client-side API.

# Contact
Please direct questions and comments to:
	kvnwinata@gmail and anette.vk@samsung.com



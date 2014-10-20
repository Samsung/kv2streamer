#NOTE
We are working hard to release kv2streamer, but it will take some time until all the necessary parts are there, including a detailed and up-to-date installation instructions (yes, README files might be inaccurate). Thank you for your patience!

KV2Streamer
===========

Kinect V2 Streamer (KV2Streamer) is a library that allows developers to access the new Kinect V2 sensor data and tracking capabilities from a non-Windows OS. 

KV2Streamer provides both a server-side application (KV2Server) that can stream out Kinect V2 data, including tracking data, to multiple client-side applications accessing the client-side API (KV2Client) running on non-Windows OS over LAN.

KV2Server
=========
KV2Streamer provides the server-side application that retrieves data from the Kinect sensor and streams it out to some defined multicast IP. For more information, see the README in ./server

KV2Client API
=============
KV2Streamer also provides the client-side API that developers can use to access data from a specific server once the server is running. For more information, see the README in ./client.


Summary
=======
In short, to use KV2Streamer:

1. Build and run the server application on Windows 8 machine.
2. Create your own project and include the KV2Client API on Ubuntu.
3. Run your application.

Note
====
- Only works on the same subnet (Multicast TTL default = 1)
- Server can run while handling multiple clients connecting and disconnecting, but not the other way, i.e. restarting the server requires restarting all clients.
- ./shared folder has codes shared by both the server application and the client-side API.

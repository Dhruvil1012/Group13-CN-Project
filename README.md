Internet TV Application using Multicast

This project, completed as part of B.Tech Sem-V, implements an Internet TV application that uses multicast communication to deliver multimedia content over a network. The project involves creating a Server-Client model using TCP and UDP protocols for transmitting video streams across a Local Area Network (LAN).

Group-13:
Aditya Shah (AU1741007)
Jeel Pujara (AU1741013)
Dhruvil Shah (AU1741024)
Himanshu Bhadania (AU1741055)
Varun Patel (AU1741080)
Guided by: Shashi Prabh

Table of Contents

Introduction
Overview of What We Have Done
Server Description
Client Description
Results
Introduction

This project provides insights into IP multicast and the characteristics of multimedia traffic, with a focus on video streaming. Video traffic is a major driver of mobile data growth, and this project simulates an Internet TV application that uses multicast to deliver video streams over a network.

Key elements include:

Sending station information over TCP.
Transmitting multimedia data over UDP.
Implementing the Any Source Multicast (ASM) model to allow multiple senders in a multicast group, ensuring interoperability across applications on different platforms.
Limiting the scope of multicast to a LAN environment.
Overview of What We Have Done

We have built a Server-Client model with the following functionality:

The server sends station information over a TCP connection.
The client receives the station details and selects a station to watch via a user-friendly graphical interface.
The server broadcasts all channels using UDP and multicast.
Clients can select channels to watch, change channels, pause/play the current channel, or turn off the TV.
The server broadcasts multiple channels simultaneously, and the client selects a channel to join its respective multicast group.
The system replicates a TV-like experience where different clients can join a stream at different times, but they will watch the current broadcast without rewinding the previous content.

Server Description

The server first establishes a TCP connection to send station details to the client.
The server then uses multithreading to broadcast different channels via UDP to multicast groups.
Each channel is streamed continuously, and clients join their selected multicast group to watch a specific channel.
The server keeps broadcasting all channels in parallel, and clients can switch between them by joining different multicast groups.
Client Description

The client first establishes a TCP connection to receive the station information from the server.
The client can select a station to watch using a graphical interface.
The client then joins the respective multicast group to start receiving the video stream over UDP.
Clients can switch between stations by selecting another channel from the interface, and they will automatically join the corresponding multicast group for the new channel.
Late clients receive the live stream as-is, without the ability to view previous content.
Results

The application successfully simulates a multicast-based Internet TV system where multiple clients can select and stream different channels.
Each client can switch channels seamlessly, with streaming delivered via UDP multicast groups.
The server continuously broadcasts channels, and clients can join or leave streams at any time.
The project demonstrates efficient use of multicast for multimedia streaming within a LAN.
License

This project is licensed under the MIT License - see the LICENSE file for details.


# Internet TV Application using Multicast

## Project Overview

This project implements an Internet TV application that utilizes multicast communication to deliver multimedia content over a Local Area Network (LAN). It features a Server-Client model using TCP and UDP protocols for transmitting video streams.

### Group Members
- Aditya Shah (AU1741007)
- Jeel Pujara (AU1741013)
- Dhruvil Shah (AU1741024)
- Himanshu Bhadania (AU1741055)
- Varun Patel (AU1741080)

### Guided by
- Shashi Prabh

## Table of Contents
1. [Introduction](#introduction)
2. [Overview of What We Have Done](#overview-of-what-we-have-done)
3. [Server Description](#server-description)
4. [Client Description](#client-description)
5. [Results](#results)
6. [License](#license)

## Introduction

This project provides insights into IP multicast and the characteristics of multimedia traffic, focusing on video streaming. Video traffic is a significant driver of mobile data growth, and this application simulates an Internet TV system that uses multicast to deliver video streams efficiently.

### Key Elements
- **Sending Station Information**: Transmission over TCP.
- **Multimedia Data Transmission**: Implemented over UDP.
- **Any Source Multicast (ASM)**: Allows multiple senders in a multicast group.
- **LAN Scope Limitation**: Ensures interoperability across applications on different platforms.

## Overview of What We Have Done

We have built a Server-Client model with the following functionalities:
- **TCP Connection**: The server sends station information to the client.
- **Channel Selection**: The client selects a station to watch through a graphical interface.
- **UDP Multicast Broadcasting**: The server broadcasts all channels via UDP.
- **User Controls**: Clients can change channels, pause/play the current stream, or turn off the TV.
- **Multiple Channels**: The server broadcasts several channels simultaneously, allowing clients to join specific multicast groups.

## Server Description

The server establishes a TCP connection to send station details to the client. It utilizes multithreading to broadcast different channels via UDP to multicast groups continuously. Clients can join their selected multicast group to watch a specific channel while the server keeps broadcasting all channels in parallel.

## Client Description

The client first establishes a TCP connection to receive station information from the server. It provides a user-friendly graphical interface for selecting a station to watch. The client joins the corresponding multicast group for the selected channel, enabling it to receive the video stream over UDP. Clients can switch channels seamlessly, automatically joining the multicast group for the new channel.

## Results

The application successfully simulates a multicast-based Internet TV system where multiple clients can select and stream different channels. Key outcomes include:
- Seamless channel switching.
- Efficient video streaming via UDP multicast groups.
- Continuous broadcasting from the server, allowing clients to join or leave streams at any time.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgements

- Special thanks to our guide, Shashi Prabh, for invaluable support and guidance throughout the project.

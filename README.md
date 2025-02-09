# SecuritySystem Project

This repository contains the source code for the SecuritySystem project, developed for the Embedded Software for the Internet of Things course at the University of Trento. The project is designed using Code Composer Studio (Version 12.7.1.00001) and includes all necessary dependencies, which are already present in the repository.  

Follow the instructions below to set up the CCS project.

---

## Setting Up the CCS Project

1. **Clone the Repository**

2. **Set the Workspace in Code Composer Studio (CCS)**
  - Launch Code Composer Studio.
  - When prompted, set the workspace directory to:
    ```
      <project_root>/central/workspace
    ```
    Replace <project_root> with the path to the root folder of the cloned repository.

3. **Configure the Path Variable for Libraries**
  To set up the library path variable in CCS:
    - Navigate to Window → Preferences.
    - In the preferences window:
      - Go to Code Composer Studio → Build → Variables.
      - Click on "New" to create a new variable.
    - Configure the variable as follows:
      - Name: IOT_PROJECT_ROOT
      - Type: Directory
      - Value: Set this to the root folder of the project. By default, this is:
        ```
            SecuritySystem
        ```

## Notes

- On Windows, errors might occur due to excessively long pathnames. To resolve this issue, you can follow the solution outlined in this [Stack Overflow thread](https://stackoverflow.com/questions/22575662/filename-too-long-in-git-for-windows).  
  Specifically, refer to the "Appendix - Doing it all from PowerShell - the copy-paste edition" section.

## MQTT Setup
MQTT (Message Queuing Telemetry Transport) is lightweight and efficient messaging protocol, that uses minimal bandwidth and system resources, which is crucial for constrained environments. 
Its publish-subscribe model enables scalable and decoupled communication between devices while ensuring reliable communication, since it allows messages to be delivered based on priority and network conditions.
Security and scalability are also key strengths, as it can be secured with TLS encryption and authentication mechanisms, ensuring safe and flexible deployments.
In MQTT, the broker is a central server that manages message distribution between clients. It acts as an intermediary, receiving messages from publishers and forwarding them to the appropriate subscribers based on topic-based filtering.

## For this project, two different solutions are available, primarily differing in how the broker is implemented:
### 1. Broker Deployed on an ESP32
In this approach, the broker runs directly on a single ESP32. This solution is ideal for a self-contained system, as all components are deployed on your devices without requiring external infrastructure. However, there are some important trade-offs to consider:
- Security Risks: Since all data is stored locally on the ESP32, there is a higher risk of exposure in case of breaches or device failure.
- Increased Power Consumption & Latency: The ESP32 is a low-power microcontroller, and running the broker on it adds computational overhead, leading to higher latency and reduced efficiency.
- Limited Scalability: This setup is better suited for small-scale applications where minimal communication is needed.
Due to these limitations, we recommend this solution only for simple applications that do not require advanced security or high-performance communication.

### 2. Broker Deployed on a Separate Host
Instead of running the broker on an ESP32, it can be hosted on a separate, more powerful machine. A common choice for this setup is Mosquitto, an open-source MQTT broker. However, we have opted to use EMQX because it offers superior performance, enhanced security, and scalability.

#### Why EMQX?
- High Performance: EMQX supports millions of concurrent connections, making it an excellent choice for IoT applications with multiple devices.
- Strong Security Features: Unlike basic MQTT implementations, EMQX supports TLS encryption for secure communication.
- User & Component Management: EMQX allows user authentication, access control, and device whitelisting, ensuring that only authorized devices can connect to the network.
- Flexibility & Extensibility: It supports various protocols beyond MQTT, such as MQTT over WebSockets, making it more versatile for different applications.
This solution is ideal for larger, more complex IoT deployments where security, scalability, and performance are critical.

## Setup
### First Solution
Use the code provided in the ESP32_broker folder and load it with your network info

### Second Solution
Follow thesetup guide on EMQX on their website, we suggest to use the docker:
[Install EMQX with Docker](https://docs.emqx.com/en/emqx/latest/deploy/install-docker.html)

Keep in mind that everything has to be on the same network to work.
Using the "subscriber" code in the Host_broker folder, add your network info and the broker ip; to do that, once you are in the EMQX interface, open the terminal on your pc and find the IPv4 address using the command "ipconfig", and put that address in the corresponding slot in the code.
Now you can deploy it and this will be the one receiving the status of the sensor and giving them to the MSP432 board.

Set up the sensor using the "publisher" code in the Host_broker folder with the same info as the subscriber, connect your sensors and deploy the code. Now you successfully have integrated the sensor and everything should be visible on the EMQX dashboard.

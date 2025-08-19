---
title: pfSense CLI
parent: pfSense
layout: default
nav_order: 3 
---
# Configuring the pfSense VM ( CLI )

This section will guide you through the initial configuration of the pfSense VM using the CLI (Command Line Interface) and then transitioning to the GUI for further management.

## Step 1: Boot into the pfSense VM
1. Start the pfSense VM from VirtualBox.
2. Upon boot, you will be greeted with the pfSense CLI menu.

## Step 2: Assign Network Interfaces

1. In the pfSense CLI menu, choose **Option 1** to assign interfaces.
2. You will be prompted with several questions to configure the interfaces. Answer as follows:

   - **Should VLANs be set up now?**  
     - Answer: **no** (Press `n`).
   - **WAN Interface Name**  
     - Set the WAN interface name to **em0**.
   - **LAN Interface Name**  
     - Set the LAN interface name to **em1**.
   - **Optional Interfaces**  
     - Since there are no optional interfaces, simply press **Enter** to continue.
   - After confirming the settings, proceed by pressing **Enter**.

## Step 3: Set the LAN Interface IP Address

1. Choose **Option 2** in the pfSense CLI menu to configure the LAN interface's IP address.
2. You will be prompted with several questions to set up the IP address and DHCP settings. Follow the instructions below:

   - **Which interface do you want to configure?**  
     - Select **2** to configure **em1** (LAN interface).
   - **IPv4 address for the LAN interface:**  
     - Answer: **192.168.1.1/24** (This sets the LAN IP address to 192.168.1.1 with a subnet mask of 255.255.255.0).
   - **Should the LAN interface be configured by DHCP?**  
     - Answer: **no** (This prevents DHCP from being used for the LAN interface).
   - **Gateway address for the LAN:**  
     - Simply press **Enter** to leave the gateway address blank (since this is the LAN side, no gateway is needed).
   - **IPv6 address configuration (DHCPv6)?**  
     - Answer: **none** (No IPv6 configuration).
   - **Enable DHCP server on LAN interface?**  
     - Answer: **yes** (This enables the DHCP server for the LAN network).
   - **Start address for the DHCP pool:**  
     - Answer: **192.168.1.100** (This is the starting IP address for the DHCP pool).
   - **End address for the DHCP pool:**  
     - Answer: **192.168.1.110** (This is the ending IP address for the DHCP pool).
   - **Revert management to HTTP (for web access)?**  
     - Answer: **yes** (This enables access to the pfSense web interface over HTTP for easier configuration).
   
3. Press **Enter** to continue.

## Step 4: Transition to GUI Management

At this point, the pfSense configuration via the CLI is complete. We have set up the basic network interfaces, LAN IP address, and DHCP server.

- The remaining pfSense settings and advanced configurations will be managed through the **pfSense GUI**.
- The GUI can be accessed from the **Ubuntu management VM** using the IP address **192.168.1.1** (the LAN IP you just configured).

The reason for using the GUI for further configurations is that managing pfSense settings through the CLI can become cumbersome, especially for advanced features like firewall rules, VLANs, and monitoring.

Once you access the pfSense GUI, you can proceed with configuring more advanced features such as VLANs, firewall rules, VPNs, and other network management tools.


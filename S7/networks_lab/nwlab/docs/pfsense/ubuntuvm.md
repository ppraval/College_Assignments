---
title: Ubuntu VM 
layout: default
parent: pfSense
nav_order: 4
---
# Configuring the pfSense Firewall using the Management Network

This section will guide you through configuring the pfSense firewall using the Management Network in the pfSense GUI. You will configure DNS, the LAN for end-users, and VLANs for segmented network access.

## Step 1: Accessing the pfSense Web Interface

1. **Start the Ubuntu VM**:
   - Open **Ubuntu Settings** > **Network**.
   - Disable and re-enable your network connection.
   - Click on the gear icon next to your network connection to verify that your IP address starts with `192.168.1`.

2. **Access pfSense Web Interface**:
   - Open **Firefox** and navigate to `http://192.168.1.1`.
   - Login with:
     - **Username**: `admin`
     - **Password**: `pfsense`
   - You are now in the pfSense configuration GUI.

3. **Initial Setup**:
   - You can click **Next** a few times in the setup wizard without changing any settings.
   - After the wizard, reload the page and go to the **Status** > **Dashboard**.

## Step 2: Configuring DNS Resolver

1. Go to **Services** > **DNS Resolver**.
2. Enable the **DNS Resolver**.
3. Configure the network interfaces:
   - **Network Interfaces**: Use **Ctrl+Click** to select **LAN** and **localhost**.
   - **Outgoing Network Interfaces**: Select **WAN**.
   - Enable **Forwarding Mode**.
4. Save and apply changes.

## Step 3: General Setup

1. Go to **System** > **General Setup**.
2. Add the following DNS servers:
   - `192.168.254.2`
   - `192.168.254.3`
   - `192.168.254.5`
   - `8.8.8.8`
   - `1.1.1.1`
3. **DNS Resolution Behavior**: Select **Use remote DNS servers (ignore local DNS servers)**.
4. Set **Timezone** to `Asia/Calcutta` under **Correct Localization**.
5. Save and apply the changes.

At this point, your internet connection should be active. Make sure your system is logged into the NITC Wi-Fi outside of the VirtualBox VM.

## Step 4: Package Manager Setup

1. Go to **System** > **Package Manager** > **Available Packages**.
2. If no packages are showing:
   - Restart the pfSense VM.
   - Go to **Ubuntu Settings** > **Network**, disable and re-enable the connection.
   - Once you have the IP address, check if packages are now available.

## Step 5: Configuring LAN for End-User (LAN 190)

1. Go to **Interfaces** > **Interface Assignments**.
2. Add **em2** as the local network for end-users.
3. Save and apply changes.
4. Click on **em2** and rename it to `LAN190` (instead of the default `OPT1`).
5. Enable the interface, choose **Static IPv4**, and set:
   - **IPv4 Address**: `192.168.190.1`
   - **Subnet Mask**: `24`
6. Save and apply changes.
7. In **Status** > **Dashboard**, you should now see all 3 interfaces.

## Step 6: Configuring Firewall Rules for LAN190

1. Go to **Firewall** > **Rules** > **LAN190**.
2. Add a new rule:
   - **Action**: Accept communication.
   - **Protocol**: Change from **TCP** to **any**.
   - Enable **Logging**.
3. Save and apply changes.

## Step 7: Configuring the DHCP Server for LAN190

1. Go to **Services** > **DHCP Server** > **LAN190**.
2. Enable the DHCP server.
3. Set the primary address pool:
   - **Start Address**: `192.168.190.10`
   - **End Address**: `192.168.190.200`
4. Save and apply changes.
5. Click the refresh icon above the configuration components to restart the DHCP server.

## Step 8: Configuring the DNS Resolver for LAN190

1. Go to **Services** > **DNS Resolver**.
2. In **Network Interfaces**, ensure **LAN**, **LAN190**, and **localhost** are selected.
3. Save and apply changes.

## Step 9: Configuring VLANs for End-User (VLAN7, VLAN8, VLAN9)

1. Go to **Interfaces** > **VLANs** > **Edit**.
2. Configure the VLANs:
   - For **VLAN7**:
     - **Parent Interface**: `OPT1 (em2)`
     - **Description**: `VLAN7`
     - **VLAN Tag**: `7`
     - **Priority**: `0`
   - Similarly, configure **VLAN8** and **VLAN9** with tags `8` and `9`, respectively.
3. Save and apply the changes.

## Step 10: Assigning VLAN Interfaces

1. Go to **Interfaces** > **Interface Assignments**.
2. Add the 3 VLANs as **OPT2**, **OPT3**, and **OPT4**.
3. Click on **OPT2**, rename it to **VLAN7**, enable the interface, and set:
   - **IPv4 Address**: `192.168.7.1`
   - **Subnet Mask**: `24`
4. Save and apply the changes.
5. Repeat the same process for **VLAN8** and **VLAN9** (for **OPT3** and **OPT4**).

## Step 11: Configuring Firewall Rules for VLAN7

1. Go to **Firewall** > **Rules** > **VLAN7**.
2. Add a new rule:
   - **Protocol**: Any
   - Enable **Logging**.
3. Save and apply changes.

## Step 12: Configuring DHCP Server for VLANs

1. Go to **Services** > **DHCP Server** and select the respective VLANs (VLAN7, VLAN8, and VLAN9).
2. Enable the DHCP server for each VLAN.
3. Set the address pools for each VLAN:
   - **VLAN7**: `192.168.7.10` to `192.168.7.200`
   - **VLAN8**: `192.168.8.10` to `192.168.8.200`
   - **VLAN9**: `192.168.9.10` to `192.168.9.200`
4. Save and apply changes.
5. Refresh the DHCP server by clicking the refresh icon.

## Step 13: Verify the Network Connection

Open a new tab in Firefox and try accessing **Google**. If the connection isn't working, restart the pfSense VM to resolve any issues.

---

At this point, the pfSense firewall is fully configured and ready for use.


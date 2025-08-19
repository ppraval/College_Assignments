---
title: Installation
layout: default
parent: pfSense
nav_order: 2 
---
# Installation and Basic Setup

This documentation will guide you through the process of setting up VirtualBox and configuring Virtual Machines (VMs) for both **pfSense** and **Ubuntu**. It covers everything from the installation of VirtualBox to setting up the VMs with appropriate resources and network configurations.

## Step 1: Install VirtualBox

To begin, follow the instructions provided at [http://192.168.40.199/vbdb](http://192.168.40.199/vbdb) for installing VirtualBox. The ISO files for both **pfSense** and **Ubuntu** are also available for download from this location.

Alternatively, get the zip file here:   
[VBox-Installation](https://jaefarnc.github.io/nwlab/assets/downloads/Downloadit.zip){: .btn .btn-purple }
[pfSense-ISO(Compressed)](https://repo.ialab.dsu.edu/pfsense/pfSense-CE-2.7.2-RELEASE-amd64.iso.gz){: .btn .btn-purple }
[Ubuntu-ISO](https://releases.ubuntu.com/jammy/ubuntu-22.04.5-desktop-amd64.iso){: .btn .btn-purple }
1. Download and install VirtualBox according to the instructions.
2. Ensure that you have the appropriate **pfSense** and **Ubuntu** ISO files.

## Step 2: Set Up the Virtual Machines

### 2.1 Create the Ubuntu and pfSense VMs
1. **VM Resources**: When setting up both VMs, allocate the following resources:
   - **2 GB RAM**
   - **2 CPUs**
   - **40 GB storage**

2. **Configure the pfSense VM**:
   - For the **Type**, select **BSD**.
   - For the **Version**, choose **FreeBSD (64-bit)**.

### 2.2 Configure Network Interfaces

In VirtualBox, you need to configure host-only networks to ensure proper communication between the VMs.

1. **Create Host-Only Networks**:
   - In VirtualBox, go to **Tools** > **Network**.
   - Create **two Host-Only Networks**.
   - In the **Properties** of each network, **disable the DHCP server**.

2. **Configure Network for pfSense VM**:
   - In the pfSense VM settings, go to **Machine Settings** > **Network**.
   - Enable **Adapter 2** and **Adapter 3**, which should be set to **vboxnet0** and **vboxnet1** respectively. Both should be **Host-Only**.

3. **Configure Network for Ubuntu VM**:
   - In the Ubuntu VM settings, go to **Machine Settings** > **Network**.
   - Set **Adapter 1** to **Host-Only**, and select **vboxnet0** as the network.

## Step 3: Install pfSense

1. **Start the pfSense VM**:
   - Boot the pfSense VM using the **pfSense ISO**.
   
2. **Install pfSense**:
   - Proceed with the default installation settings for pfSense.
   - **Select the disk** for installation (use the space bar to select the disk during the installation process).
   - Follow the on-screen instructions and complete the installation.

3. **Post-Installation Setup**:
   - Once the installation is complete, **shut down the pfSense VM**.
   - In **Machine Settings** > **Storage**, **remove the ISO attachment** from the VM. Ensure that only the **.vdi file** (VirtualBox Disk Image) remains attached.
   - **Start the pfSense VM** again and check if you can see the standard list of options for pfSense configuration.

## Step 4: Install Ubuntu

1. **Start the Ubuntu VM**:
   - Boot the Ubuntu VM using the **Ubuntu ISO**.
   - Remember to check the box for skipping unattended installation.

2. **Follow the Standard Installation Steps**:
   - Proceed with the typical installation steps for Ubuntu, such as selecting language, keyboard layout, disk partitioning, and creating a user account.

Once the Ubuntu VM is installed, your VMs are ready.

## Step 5: Further Configuration

The rest of the configuration steps will be performed inside the VMs. You can now proceed to configure pfSense for network management, including setting up the management network, firewall rules, and VLANs as needed. The Ubuntu VM can be used to manage and interact with the pfSense firewall.


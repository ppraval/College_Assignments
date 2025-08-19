---
title: L2 Switch
layout: default
parent: pfSense 
nav_order: 5 
---
# Configuring the L2 Switch

At this point, you should have created all the necessary gateways. Now, it's time to configure the Layer 2 (L2) switch. Without this configuration, you'll receive raw traffic through the switch, which is not ideal for proper network management.

## Step 1: Prepare the L2 Switch

1. **Collect Equipment**:
   - Obtain the L2 switch and two Ethernet cables from the lab admins.

2. **Power and Reset the Switch**:
   - Plug in the power cable for the switch.
   - To reset the switch, hold the reset button with a pen for **5 seconds** and then release it.
   - Once reset, the switch will blink alternating colors quickly to indicate it has been reset. It will stop blinking once it is ready for use.

3. **Connect the Switch to the Lab System**:
   - Connect an Ethernet cable from **port 2 of the switch** to the Ethernet port of your lab system.
   - If there is an existing Ethernet cable connected to your machine's port, you may need to remove it temporarily and reconnect it after this configuration.

## Step 2: Configure the Lab System Network Settings

1. Open **Ubuntu Settings** > **Network**.
2. Click the gear icon next to the **Wired** connection's on/off button.
3. Go to the **IPv4** tab and set the connection to **Manual**.
4. Configure the settings as follows:
   - **IPv4 Address**: `192.168.1.11`
   - **Subnet Mask**: `255.255.255.0`
   - **Gateway Address**: `192.168.1.1`
5. Turn the wired connection off, wait a few seconds, and then turn it back on. Your IP address should now be `192.168.1.11`.

## Step 3: Access the Switch Management Portal

1. Open **Firefox** and go to `http://192.168.1.10` to access the switch's management portal.
2. The username and password to login to the portal are 'admin'.
   
### Step 4: Create VLANs

1. **Create VLANs**:
   - Click on **Create VLAN**.
   - Select **VLAN7**, move it to the right.
   - Select **VLAN8**, move it to the right.
   - Select **VLAN9**, move it to the right.
   - Click **Apply** to save the changes.

### Step 5: Configure VLANs

1. **VLAN007 Configuration**:
   - Click on **VLAN Configuration**.
   - Select **VLAN007** from the dropdown menu.
   - Set port **2** to **Tagged** and ports **7** and **14** to **Untagged**.
   - Click **Save** and **Apply**.

2. **VLAN008 Configuration**:
   - Select **VLAN008** from the dropdown menu.
   - Set port **2** to **Tagged** and ports **8**, **16**, and **24** to **Untagged**.
   - Click **Save** and **Apply**.

3. **VLAN009 Configuration**:
   - Select **VLAN009** from the dropdown menu.
   - Set port **2** to **Tagged** and port **9** to **Untagged**.
   - Click **Save** and **Apply**.

Your switch is now configured with the required VLANs.

## Step 6: Configure VirtualBox and pfSense VM Network Settings

1. Open **VirtualBox** and go to the **Settings** of the pfSense VM.
2. Navigate to the **Network** tab.
3. For **Adapter 3**, set the network mode to **Bridged**.

## Step 7: Connect the Second Ethernet Cable

1. Connect the second Ethernet cable from **switch port 7** to another lab machine's Ethernet port.
   - Remember to reconnect the existing Ethernet connection when you are done.

## Step 8: Configure the Second Lab Machine's Network Settings

1. On the second lab machine, open **Ubuntu Settings** > **Network**.
2. Click the gear icon next to the **Wired** connection and go to the **IPv4** tab.
3. Set the connection to **Automatic (DHCP)**.
4. Apply the changes.
5. Turn off the wired connection, wait a few seconds, and then turn it back on. Your IP address should now start with `192.168.7`.

## Step 9: Verify the Configuration

If everything has been configured correctly, you should now have network connectivity. Congratulations, your firewall configuration is now complete!



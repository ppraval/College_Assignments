---
title: Basic Concepts
layout: default
parent: pfSense
nav_order: 1
---
# Concepts to Understand

## Network Interface Cards (NICs)
For a pfSense firewall setup, you need at least three network interfaces:

- **WAN Interface:** The connection between the firewall and the internet or external network.
- **LAN Interface:** The internal network that pfSense will protect.
- **Management Interface:** A dedicated interface for configuring pfSense through its web GUI.

Each of these interfaces is connected to a physical or virtual network adapter, and the firewall uses these interfaces to manage traffic based on your defined rules.

## Virtualization with VirtualBox
In this setup, we will use **VirtualBox** to create virtual machines (VMs) for both pfSense and Ubuntu. VirtualBox is a free and open-source hypervisor that allows you to run multiple operating systems on a single host machine. We'll be configuring virtual NICs in VirtualBox to represent the various network interfaces needed by pfSense.

## VLANs (Virtual Local Area Networks)
A **VLAN** is a network segment that operates independently of physical hardware. It allows you to logically group devices, regardless of their physical location, for better traffic management and security. VLANs are identified by a VLAN ID (ranging from 1 to 4095). The key benefit of VLANs is the ability to separate traffic between different groups, ensuring better isolation and reducing broadcast traffic.

## Switches and Tagged VLANs
A **Layer 2 (L2) Switch** operates at the data link layer and is responsible for forwarding data between devices on the same local network. It uses MAC addresses to determine the correct output port for each incoming packet.

When dealing with VLANs, a switch needs to be configured to handle **tagged traffic**. Tagged traffic means that the VLAN ID is included in the Ethernet frame to tell the switch which VLAN the traffic belongs to. The switch then forwards the traffic to the appropriate port based on the VLAN ID.

## Input and Output Ports on Switches
Switches have multiple **input and output ports**. When a tagged VLAN packet is received on an input port, the switch uses the VLAN ID to decide which output ports should forward the packet. Each port can be assigned to a specific VLAN, effectively isolating traffic for different groups of users or devices.

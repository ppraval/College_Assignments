---
title: pfSense
layout: default
nav_order: 2
---
# pfSense Firewall Setup Documentation

This guide outlines the steps to set up a pfSense firewall in a virtualized environment using VirtualBox. The setup involves configuring both the pfSense and Ubuntu virtual machines (VMs), configuring the management network, and using a switch to pass the network to another system. Understanding these basic concepts will help you navigate the further configuration and fine-tuning of the firewall.

## Overview of pfSense Firewall

pfSense is a powerful, open-source firewall/router software based on FreeBSD. It is highly customizable and often used for network security in both home and enterprise environments. To function properly, pfSense typically requires at least **three network interfaces**:

1. **WAN (Wide Area Network)** - This is the interface connected to the internet or an external network. It handles inbound and outbound traffic between your internal network and the wider world.
2. **LAN (Local Area Network)** - The internal network that connects to the devices within your local area. This interface connects to your home network or the private network you wish to protect.
3. **Management Network** - A dedicated network for administering the pfSense firewall. It is used to configure the firewall via the pfSense GUI, which allows you to define rules, monitor traffic, and manage network settings.

Additionally, pfSense supports **VLANs (Virtual Local Area Networks)**, which are used to logically divide a physical network into smaller, isolated segments. Each VLAN has a unique identifier (VLAN ID), which allows you to control traffic between different network segments.


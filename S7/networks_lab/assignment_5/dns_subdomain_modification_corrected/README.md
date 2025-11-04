# DNS Subdomain Delegation Configuration - Corrected

## Overview
This configuration properly sets up DNS subdomain delegation where `sub.nw.nitc.ac.in` is delegated to a separate DNS server.

## Server Setup

### 1. Master DNS Server (192.168.1.10)
- **Role**: Authoritative for `nwlab.cse.nitc.ac.in` and `nw.nitc.ac.in`
- **Configuration Files**:
  - `named.conf.local`
  - `named.conf.options`
- **Zone Files**:
  - `db.nwlab.cse.nitc.ac.in` (unchanged)
  - `db.nw.nitc.ac.in` - **KEY FILE**: Contains delegation records
  - `db.192.168.1` (unchanged)
  - `db.192.168.2` (unchanged)

### 2. Subdomain Master DNS Server (192.168.3.10)
- **Role**: Authoritative for `sub.nw.nitc.ac.in` subdomain
- **Configuration Files**:
  - `named.conf.local`
  - `named.conf.options`
- **Zone Files**:
  - `db.sub.nw.nitc.ac.in`
  - `db.192.168.3`

### 3. Slave DNS Server (192.168.1.11)
- **Role**: Secondary server for all zones
- **Configuration Files**:
  - `named.conf.local`
  - `named.conf.options`
- **Behavior**: Automatically transfers zones from both masters

## How Subdomain Delegation Works

### In the Parent Zone (nw.nitc.ac.in)
The key is in `db.nw.nitc.ac.in`:

```bind
; Delegate sub.nw.nitc.ac.in to another DNS server
sub     IN      NS      ns3.sub.nw.nitc.ac.in.
sub     IN      NS      ns2.nw.nitc.ac.in.

; Glue record - REQUIRED because ns3 is within the subdomain
ns3.sub IN      A       192.168.3.10
```

**What this does:**
1. When someone queries for `*.sub.nw.nitc.ac.in`, the parent server says "ask ns3.sub.nw.nitc.ac.in"
2. The glue record provides the IP (192.168.3.10) so the client can contact that server
3. The subdomain server then provides the actual answer

### In the Subdomain Zone (sub.nw.nitc.ac.in)
The subdomain server (`db.sub.nw.nitc.ac.in`) is fully authoritative:

```bind
@       IN      SOA     ns3.sub.nw.nitc.ac.in. admin.sub.nw.nitc.ac.in. (...)
@       IN      NS      ns3.sub.nw.nitc.ac.in.
@       IN      NS      ns2.nw.nitc.ac.in.
ns3     IN      A       192.168.3.10
www     IN      A       192.168.3.100
```

## Key Differences from Your Original Setup

### ❌ What Was Wrong:
1. **No delegation in parent zone**: The parent zone (`nw.nitc.ac.in`) didn't have NS records pointing to the subdomain server
2. **Wrong approach**: You can't just create separate master zones; the parent must delegate
3. **Missing glue records**: Without glue records, clients can't find the subdomain server

### ✅ What's Fixed:
1. **Proper delegation**: Parent zone has NS and glue records for subdomain
2. **Clear authority**: Subdomain server is explicitly authoritative for `sub.nw.nitc.ac.in`
3. **Correct zone transfers**: Slave gets subdomain zone from subdomain master (192.168.3.10), not parent

## DNS Resolution Flow

When querying `www.sub.nw.nitc.ac.in`:

1. **Client queries any nameserver** for `www.sub.nw.nitc.ac.in`
2. **Parent server (192.168.1.10) responds**: "For `sub.nw.nitc.ac.in`, ask ns3.sub.nw.nitc.ac.in at 192.168.3.10"
3. **Client queries subdomain server (192.168.3.10)** for `www.sub.nw.nitc.ac.in`
4. **Subdomain server responds**: "www.sub.nw.nitc.ac.in is at 192.168.3.100"

## Deployment Instructions

### On Master Server (192.168.1.10):
```bash
# Copy corrected files
sudo cp named.conf.local /etc/bind/named.conf.local
sudo cp named.conf.options /etc/bind/named.conf.options
sudo cp zones/db.nw.nitc.ac.in /etc/bind/zones/db.nw.nitc.ac.in

# Check configuration
sudo named-checkconf
sudo named-checkzone nw.nitc.ac.in /etc/bind/zones/db.nw.nitc.ac.in

# Restart BIND
sudo systemctl restart bind9
```

### On Subdomain Master Server (192.168.3.10):
```bash
# Create zones directory
sudo mkdir -p /etc/bind/zones

# Copy corrected files
sudo cp named.conf.local /etc/bind/named.conf.local
sudo cp named.conf.options /etc/bind/named.conf.options
sudo cp zones/db.sub.nw.nitc.ac.in /etc/bind/zones/db.sub.nw.nitc.ac.in
sudo cp zones/db.192.168.3 /etc/bind/zones/db.192.168.3

# Check configuration
sudo named-checkconf
sudo named-checkzone sub.nw.nitc.ac.in /etc/bind/zones/db.sub.nw.nitc.ac.in
sudo named-checkzone 3.168.192.in-addr.arpa /etc/bind/zones/db.192.168.3

# Restart BIND
sudo systemctl restart bind9
```

### On Slave Server (192.168.1.11):
```bash
# Copy corrected files
sudo cp named.conf.local /etc/bind/named.conf.local
sudo cp named.conf.options /etc/bind/named.conf.options

# Check configuration
sudo named-checkconf

# Restart BIND
sudo systemctl restart bind9
```

## Testing

### Test delegation from parent:
```bash
dig @192.168.1.10 sub.nw.nitc.ac.in NS
# Should return: ns3.sub.nw.nitc.ac.in and ns2.nw.nitc.ac.in
```

### Test subdomain resolution:
```bash
dig @192.168.3.10 www.sub.nw.nitc.ac.in
# Should return: 192.168.3.100

dig @192.168.1.10 www.sub.nw.nitc.ac.in
# Should refer to subdomain server or return answer if recursive
```

### Test from slave:
```bash
dig @192.168.1.11 www.sub.nw.nitc.ac.in
# Should return: 192.168.3.100
```

### Test reverse lookup:
```bash
dig @192.168.3.10 -x 192.168.3.100
# Should return: www.sub.nw.nitc.ac.in
```

## Summary

The corrected configuration implements **proper DNS subdomain delegation**:
- Parent zone delegates authority via NS records
- Glue records provide necessary IP addresses
- Subdomain server is independently authoritative
- Slave server replicates from correct masters
- DNS resolution follows standard referral process

#!/bin/sh
sudo ip link set xxxx down
sudo ip link set xxxx arp off
sudo ip link set xxxx multicast off
sudo ip link set xxxx allmulticast off
sudo ip link set xxxx promisc on
echo "stm-eth well done"
sudo ip link set xxxx up

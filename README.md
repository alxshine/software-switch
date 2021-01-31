# software-switch

This is a spanning-tree protocol (STP) capable software-switch that I wrote because we didn't have enough test devices for my bachelor's thesis.
It takes multiple interface names as command line parameters, and bridges them using NPCAP.

STP information is kept locally, and ports are blocked according to that information.
Packets are also forwarded correctly.

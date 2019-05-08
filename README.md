# ACE-TAO-RTEMS-example

This is going to use the ACE-TAO example to run connect to a grid server. The grid 
server will run on the ZC706 board and will utilize RTEMS and WAF. You will need
to build RTEMS and WAF in order to build the following project.


https://github.com/matthew-renodin/ACE-TAO-RTEMS-installer


You need to clone this project, source the setenv in the ACE-TAO-RTEMS-installer 
project.

cd ACE-TAO-RTEMS-installer

source setenv


Run the following command from the root of the ACE-TAO-RTEMS-example project. This will create the Makefiles
and will utilize the GNU macros in the ACE_TAO project that gets created when you follow
the steps in the ACE-TAO-RTEMS-installer project.

cd ACE-TAO-RTEMS-example

$ACE_ROOT/bin/mwc.pl example.mwc -type gnuace

cd src

make

JTAG the server.exe to the ZC706.


The server code essentially needs configurations. For convience, this example will
simply take place some arguments in the server.cpp

This is what you get:

Notice that I am using a file "/etc/server.ior" - this is probably unnecessary. This 
example lacks a nameserver that could provide the server.ior, but if you are 
debugging the file can store the IOR. 
  
```cpp
includes...
int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  argc = 12;
  char* myargv[argc+1] = {
  "-ORBDottedDecimalAddresses", "1", 
  "-ORBEnforcePreferredInterfaces", "1", 
  "-ORBPreferredInterfaces", "*=192.168.1.10",
  "-ORBListenEndpoints","iiop://192.168.1.10:9999", 
  "-o", "/etc/service.ior", 
  "-ORBdebuglevel", "2", 
  0};
  ...
}
```

When I use the Grid example's server and bring this code into it, the exectuable will 
run on the ZC706. I JTAG this onto the board, and then run the corresponding client on
a Windows 10 machine that is connected to the board. The client I compiled under MINGW 
using the ACE-TAO-MINGW-installer project.

https://github.com/matthew-renodin/ACE-TAO-mingw-installer

When I started the server it output string that I saved to a file I called server.ior:

IOR:010000001500000049444c3a477269645f466163746f72793a312e3000000000010000000000000064000000010102ff0d0000003139322e3136382e312e313000e90f273100000014010f004e55500000001a0000000001000000526f6f74504f41006368696c645f706f6100000000000100000047726964777fff0100000000000000080000000115ec00004f4154


I ran the client with the following parameters:

cd ACE-TAO-mingw-installer

source setenv

cd ACE_TAO/TAO/examples/

$ACE_ROOT/bin/mwc.pl examples.mwc -type gnuace

cd Simple/grid

make

./client.exe -f server.ior -w 12 -h 12 -p 2 -q 2 -v 2345  -ORBEndpoint iiop://192.168.1.10:9999 -x


This is the output to the screen from the grid server that can be found in ACE-TAO/TAO/examples/Simple/grid.

```
*** RTEMS PROJECT TEST ***
nexus0: <RTEMS Nexus device>
zy7_slcr0: <Zynq-7000 slcr block> on nexus0
cgem0: <Cadence CGEM Gigabit Ethernet Interface> on nexus0
miibus0: <MII bus> on cgem0
e1000phy0: <Marvell 88E1116R Gigabit PHY> PHY 7 on miibus0
e1000phy0:  none, 10baseT, 10baseT-FDX, 100baseTX, 100baseTX-FDX, 1000baseT-FDX, 1000baseT-FDX-master, auto
cgem0: no mac address found, assigning random: 0e:b0:ba:5e:ba:11
info: cgem0: Ethernet address: 0e:b0:ba:5e:ba:11
info: lo0: link state changed to UP
info: cgem0: link state changed to DOWN
add host 192.1
This will print if there is a network
*****************************************************************

68.1.1: gateway cgem0
add net default: gateway 192.168.1.1

Grid server

TAO (1|167837697) - TAO_Time_Policy_Manager: loaded time policy strategy 'TAO_SYSTEM_TIME_POLICY'
TAO (1|167837697) - Default_Resource_Factory - unable to create codeset manager.
TAO (1|167837697) - ORB_Core: Codeset Manager not available
TAO (1|167837697) - Loaded default protocol <IIOP_Factory>
The IOR is: <IOR:010000001500000049444c3a477269645f466163746f72793a312e3000000000010000000000000064000000010102000d0000003139322e3136382e312e313000e90f273100000014010f004e55500000001a0000000001000000526f6f74504f41006368696c645f706f6100000000000100000047726964777fff0100000000000000080000000115ec00004f4154>
Starting Grid Server
info: cgem0: link state changed to UP
TAO (1|167837697) - IIOP_Connection_Handler::open, IIOP connection to peer <192.168.1.1:57255> on [17]
(1|167837697) Making a new Grid
(1|167837697) Grid has been destroyed
TAO (1|167837697) - Transport[17]::handle_input, error parsing incoming message
TAO (1|167837697) - Connection_Handler[17]::close_connection_eh, purging entry from cache
TAO (1|167837697) - Connection_Handler[17]::close_connection_eh, removing from the reactor
TAO (1|167837697) - Connection_Handler[17]::close_connection_eh, cancel all timers
TAO (1|167837697) - Connection_Handler[17]::close_connection_eh end
TAO (1|167837697) - IIOP_Connection_Handler::open, IIOP connection to peer <192.168.1.1:57258> on [18]

```

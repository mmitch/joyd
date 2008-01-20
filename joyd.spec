Summary: A utility to execute programs using a joystick
Name: joyd
Version: 0.2.2
Release: 1
Copyright: GPL
Group: System Environment/Daemons
Source: joyd-0.2.2.tar.gz
Patch: joyd-0.2.2-rpm-patch.gz

%description
joyd is a daemon that allows you to trigger commands on your computer                        
by using your joystick. It can execute any program.                                          
                                                                                             
You can bind commands to single buttons, movements of your stick or                          
combinations of both. There can be as much commands as you like, you                         
can even bind multiple commands to one combination.                                          
                                                                                             
joyd uses very few resources: when nothing happens at the joystick                           
port, it just sleeps, needing no CPU time.                                                   
                                                                                             
When run in daemon mode, joyd switches to the background. It can still                       
be controlled by joystick commands or signals (e.g.  SIGHUP). joyd is                        
also able to log its output to syslog.

%prep
%setup
%patch -p1

%build
./configure
make

%install
make install

%files
%doc doc/* examples/*
/usr/man/man1/joyd.1
/usr/man/man5/joydrc.5
/usr/bin/joyd
/usr/bin/joyreadbutton
/usr/bin/joyreadaxis

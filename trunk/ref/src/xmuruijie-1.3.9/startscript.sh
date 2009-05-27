#! /bin/sh
### BEGIN INIT INFO
# Provides:           xmuruijie
# Required-Start:     $network
# Default-Start:      S
# Default-Stop:       0 6
# Short-Description:  EAP-MD5 client for Red Gaient Algorithm 
# Description:        Debian init script for xmuruijie
### END INIT INFO
#

PATH=/sbin:/bin:/usr/sbin:/usr/bin
DAEMON=/usr/sbin/xmuruijie
NAME=xmuruijie
DESC="EAP-MD5 client"

set -e

test -x $DAEMON || exit 0

# check xmuruijie configuration

if [ ! -f "/etc/xmuruijie.conf" ] ; then
    echo "You must set configs in /etc/xmuruijie.conf first"
    exit 1
fi

DEV=$(sed -n '/interface/p' /etc/xmuruijie.conf | awk '{print $2}')

case "$1" in
 start) 
    echo "Starting xmuruijie"
    	/usr/sbin/xmuruijie -d -c /etc/xmuruijie.conf

    echo "Running dhcp client to get ip address..."
    if [ -f "/sbin/dhcpcd" ] ; then
        /sbin/dhcpcd ${DEV}
    elif [ -f "/sbin/dhcpcd-bin" ] ; then
        /sbin/dhcpcd-bin ${DEV}
    elif [ -f "/sbin/dhclient" ] ; then
    	/sbin/dhclient ${DEV}
    fi
       ;;
 stop) 
    echo "Stopping dhcp client..."
    if [ -f "/sbin/dhcpcd" ] ; then
        /sbin/dhcpcd -k ${DEV}
    elif [ -f "/sbin/dhcpcd-bin" ] ; then
        /sbin/dhcpcd-bin -k ${DEV}
    elif [ -f "/sbin/dhclient" ] ; then
        /sbin/dhclient -r ${DEV}
    fi
    echo "Stopping xmuruijie..."
    	/usr/sbin/xmuruijie -p -c /etc/xmuruijie.conf
       ;;
esac

exit 0

##EOF

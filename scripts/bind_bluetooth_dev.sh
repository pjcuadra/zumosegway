#!/usr/bin/env bash

bt_dev=`./get_bluetooth_dev.sh`
bt_dev_file='/dev/rfcommp'

rfcomm bind $bt_dev $1 $bt_dev_file


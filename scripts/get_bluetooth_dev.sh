#!/usr/bin/env bash

rfkill list | grep hci | sed  "s/.*hci\(.*\):.*/\1/"

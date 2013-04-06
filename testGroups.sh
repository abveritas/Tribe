#!/bin/bash

pkglist=data/netinstall.conf
pacman_args=( $(sed -e 's/[#[].*//' -e 's/[ ^I]*$$//' -e '/^$$/ d' ${pkglist}) )
pacman -Sp ${pacman_args[@]} > /dev/null && echo "Group list OK"

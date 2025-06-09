#!/usr/bin/env sh
# This is a personal script to update my system configuration to use the latest commit of this repo

git push

cd /nixdots || exit

nix flake update dwl
nh home switch

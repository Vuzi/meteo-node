#!/bin/bash
Host=192.168.0.41
User=pi
HostDir=/home/pi/MeteoNode

Rsync=/usr/local/Cellar/rsync/3.1.1/bin/rsync # or rsync

echo "Build client app"
echo "Build server app"
echo "Sync $LocalDir -> $User@$Host:$HostDir"
"$Rsync" -e ssh -vra --delete-after --exclude="build" --exclude="node_modules" --exclude=".git" . "$User@$Host:$HostDir"
echo "Done"

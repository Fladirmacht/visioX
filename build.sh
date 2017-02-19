#!/bin/bash

set -e

date
ps axjf

#################################################################
# Update Ubuntu and install prerequisites for running Visio     #
#################################################################
sudo apt-get update
#################################################################
# Build Visio from source                                       #
#################################################################
NPROC=$(nproc)
echo "nproc: $NPROC"
#################################################################
# Install all necessary packages for building Visio             #
#################################################################
sudo apt-get install -y qt4-qmake libqt4-dev libminiupnpc-dev libdb++-dev libdb-dev libcrypto++-dev libqrencode-dev libboost-all-dev build-essential libboost-system-dev libboost-filesystem-dev libboost-program-options-dev libboost-thread-dev libboost-filesystem-dev libboost-program-options-dev libboost-thread-dev libssl-dev libdb++-dev libssl-dev ufw git
sudo add-apt-repository -y ppa:bitcoin/bitcoin
sudo apt-get update
sudo apt-get install -y libdb4.8-dev libdb4.8++-dev

cd /usr/local
file=/usr/local/visioX
if [ ! -e "$file" ]
then
        sudo git clone https://github.com/Fladirmacht/visio.git
fi

cd /usr/local/visioX/src
file=/usr/local/visioX/src/visiod
if [ ! -e "$file" ]
then
        sudo make -j$NPROC -f makefile.unix
fi

sudo cp /usr/local/visioX/src/visiod /usr/bin/visiod

################################################################
# Configure to auto start at boot                                      #
################################################################
file=$HOME/.visio
if [ ! -e "$file" ]
then
        sudo mkdir $HOME/.visio
fi
printf '%s\n%s\n%s\n%s\n' 'daemon=1' 'server=1' 'rpcuser=u' 'rpcpassword=p' | sudo tee $HOME/.visio/visio.conf
file=/etc/init.d/visio
if [ ! -e "$file" ]
then
        printf '%s\n%s\n' '#!/bin/sh' 'sudo visiod' | sudo tee /etc/init.d/visio
        sudo chmod +x /etc/init.d/visio
        sudo update-rc.d visio defaults
fi

/usr/bin/visiod
echo "Visio has been setup successfully and is running..."
exit 0


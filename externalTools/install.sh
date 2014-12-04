#!/bin/bash

function checkInstalled
{
    if !(which $1 > /dev/null); then
    	echo -e "\e[1;31m$1 is not installed.\e[0m"
    	echo -e "\e[1;31mYou will need to install the dependencies manually.\e[0m"        
        exit 1
    fi
}

function install
{
	echo Installing $1
	tar -xvzf $1	
	cd $(basename $1 .tar.gz)	
	echo -e "\e[1;32mRunning ./configure --> $1\e[0m"
	./configure
	echo -e "\e[1;32mRunning make -j4 --> $1\e[0m"
	make -j4
	echo -e "\e[1;32mRunning sudo make install --> $1\e[0m"
	sudo make install
	echo -e "\e[1;32mRunning make clean --> $1-src\e[0m"
	make clean
	cd ..
	#echo -e "\e[1;32m$1 installed\e[0m"
}

#Check dependencies
checkInstalled make

#Install ViennaRNA 1.8
install ViennaRNA-1.8.5.tar.gz
rm -rf ViennaRNA-1.8.5

#Install UNAFold
install unafold-3.8.tar.gz
rm -rf unafold-3.8

#Install InfoRNA
tar -xvzf INFO-RNA-2.1.2.tar.gz 

#Install RNAhybrid
tar -xvzf RNAhybrid-2.1.1-src.tar.gz
cd RNAhybrid-2.1.1
echo -e "\e[1;32mRunning ./configure --> RNAhybrid-2.1.1\e[0m"
./configure
echo -e "\e[1;32mRunning make -j4 --> RNAhybrid-2.1.1\e[0m"
make -j4
echo -e "\e[1;32mRunning make install --> RNAhybrid-2.1.1\e[0m"
sudo make install
echo -e "\e[1;32mRunning make clean --> RNAhybrid-2.1.1\e[0m"
make clean
rm -rf RNAhybrid-2.1.1

#Install ViennaRNA 2.1
install ViennaRNA-2.0.7.tar.gz 
rm -rf ViennaRNA-2.0.7

#Install intaRNA
install intarna-1.2.5.tar.gz	

if which "RNAfold" >/dev/null; then
	echo -e "\e[1;32mVienna installed successfully.\e[0m"
else
	echo -e "\e[1;31mVienna is not installed.\e[0m" 
fi

if which "UNAFold.pl" >/dev/null; then
	echo -e "\e[1;32mUnafold installed successfully.\e[0m"
else
	echo -e "\e[1;31mUnafold is not installed.\e[0m" 
fi

cd INFO-RNA-2.1.2
if [ -f "INFO-RNA-2.1.2" ]; then
	echo -e "\e[1;32mINFO-RNA installed successfully.\e[0m"
else
	echo -e "\e[1;31mINFO-RNA is not installed.\e[0m" 
fi
cd ..

if which "RNAhybrid" >/dev/null; then
	echo -e "\e[1;32mRNAhybrid installed successfully.\e[0m"
else
	echo -e "\e[1;31mRNAhybrid is not installed.\e[0m" 
fi

cd intarna-1.2.5/src
if [ -f "IntaRNA" ]; then
	echo -e "\e[1;32mIntaRNA installed successfully.\e[0m"
else
	echo -e "\e[1;31mIntaRNA is not installed.\e[0m" 
fi
cd ../..
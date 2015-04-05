#########################################################################
# File Name: test.sh
# Author: Mengxing Liu
# Created Time: 2015-04-02
#########################################################################
#!/bin/bash

./waf configure
./waf

int=0
while(($int<$1))
do
./run.py -f config.teaker.xml -H config/hosts-teaker
let "int++"
done


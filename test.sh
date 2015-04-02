#########################################################################
# File Name: test.sh
# Author: Mengxing Liu
# Created Time: 2015-04-02
#########################################################################
#!/bin/bash

cd cmakebuild
make -j 8
cd ..

./run.py -f ../config.mcore.xml

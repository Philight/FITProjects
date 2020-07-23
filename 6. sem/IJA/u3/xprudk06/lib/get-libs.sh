#!/bin/sh
BASEDIR=$(dirname $BASH_SOURCE)

URL="http://search.maven.org/remotecontent?filepath=org/hamcrest/hamcrest-core/1.3/hamcrest-core-1.3.jar"
FILENAME="hamcrest-core-1.3.jar"
wget -O $BASEDIR/$FILENAME $URL

URL="http://search.maven.org/remotecontent?filepath=junit/junit/4.12/junit-4.12.jar"
FILENAME="junit-4.12.jar"
wget -O $BASEDIR/$FILENAME $URL



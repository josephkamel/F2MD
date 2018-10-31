#!/bin/bash

# create the output directory (if it does not exist)
mkdir -p output
# remove result from previous runs
rm output/*.json
# add first opening bracked
echo { >> output/tmp.json
echo 'Hello World 1'
# use all json files in current folder

for i in MDBsms_Sybil_V1/*.bsm
do 
	echo 'Hello World 2'
    # first create the key; it is the filename without the extension
    echo \"$i\": | sed 's/\.json//' >> output/tmp.json
    # dump the file's content
    cat "$i" >> output/tmp.json
    # add a comma afterwards
    echo , >>  output/tmp.json
done
# remove the last comma from the file; otherwise it's not valid json
cat output/tmp.json | sed '$ s/.$//' >> output/out.json
# remove tempfile
rm output/tmp.json
# add closing bracket
echo } >> output/out.json
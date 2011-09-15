# Script to export rbfsum data to libra database
# Ardhan Madras <ajhwb@knac.com>
#
#!/bin/sh

dir=/srv/www/htdocs/rbfsum

find $dir -type f -name '*.txt' -mtime -2 -exec /mindo/rbfsum2db {} \;

#!/usr/bin/awk -f
#
#
BEGIN {
	regexp = domain"\\.";
        target = 0;
}
#debug
#/^Device/ {
#	print $0;
#}
$1 == "deviceId" {
        if ($3 ~ domain) {
		devid = substr($3, index($3, ".") + 1);
                printf("%s,", devid);
		target = 1;
        }
        next;
}
$1 == "primaryKey" {
	if (target) {
		print $3;
	}
	target = 0;
        next;
}

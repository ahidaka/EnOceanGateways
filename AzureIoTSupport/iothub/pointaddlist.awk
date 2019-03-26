#!/usr/bin/awk -f
#
#
BEGIN {
        pDomain = domain;
	i = 0;
	while("PointList" | getline) {
		sub("\r", "");
		points[pDomain "." $0] = 0;
	}
	#for (p in points) {
	#        print(p "=" points[p]);
	#}
}

/deviceId/ {
	if ($3 in points) {
		#print($3 " is Hit!");
		points[$3]++;
	}
	else {
		#print($3 " is notfound");
	}
}

END {
	##print("**** summary ****");
	for (p in points) {
	        if (points[p] > 0) {
			##print("*" p "=" points[p]);
		}
		else {
			print(p);
		}
	}
}

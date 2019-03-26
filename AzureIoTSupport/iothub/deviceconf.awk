#!/usr/bin/awk -f
#
#
BEGIN {
	regexp = domain"\\.";
	constr = cs; 
	
        while ("/usr/local/bin/dppoint" | getline) {
                lists[$1] = "0";
                # printf("%s\n", $1); #debug
        }
}
#debug
#/^Device/ {
#	print $0;
#}
$1 == "deviceId" {
        if ($3 ~ regexp) {
		devid = substr($3, index($3, ".") + 1);
                printf("#%s,%s\n", $3, devid); #debug
		if (lists[devid] == "0") {
			printf("** Noop %s %s\n", $3, devid); #debug
			lists[devid] == "1";
		}
		else {
			printf("** Delete %s %s %s\n", $3, devid, lists[devid]);  #debug
			cmd = "sample delete -l " constr " " $3;
			print cmd;
		}
        }
        next;
}
END {
       for(name in lists) {
               # print name, lists[name]; #debug
	       if (lists[name] == "0") {
		       devid = domain "." name;
		       printf("** Create %s %s %s\n", name, devid, lists[name]);
			cmd = "sample create -l " constr " " devid;
			print cmd;
	       }
       }
}

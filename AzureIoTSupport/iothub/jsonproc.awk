#!/usr/bin/awk -f
#
#
BEGIN {
	pCs = cs;
	pDomain = domain;
	pDevList = devlist;
	
	addr = 1;
	idx = 1;
	status = 0;
	count = 0;

	start = index(pCs, "=") + 1;
	end = index(pCs, ".");
	leng = end - start;
	IoTHubName = substr(pCs, start, leng);
	#print "*", "IoTHubName=" IoTHubName; #debug

	start = end + 1;
	end = index(pCs, ";");
	leng = end - start;
	IoTHubSuffix = substr(pCs, start, leng);
	#print "*", "IoTHubSuffix=" IoTHubSuffix; #debug

	#while ("cat devlist" | getline) {
	while ("cat " pDevList | getline) {
		comma = index($0, ",");
		name = substr($0, 1, comma - 1)
                lists[name] = substr($0, comma + 1);
		idxes[name] = addr;
		macAddr = sprintf("%06d", addr++);
		#printf("macAddr<%s>\n", macAddr);
		realMac = "00:00:00:" substr(macAddr, 1, 2) ":"	\
			substr(macAddr, 3, 2) ":" \
			substr(macAddr, 5, 2);
		#printf("mac=%s\n", realMac);
		macaddrs[name] = realMac;
        }
	#for (name in lists) { #debug
	#	print name, macaddrs[name], lists[name];
	#}

	count = length(lists) ;
	
	##print "lists.size=" listlength;
	## Generate prime number
	while("PrimeNumber -f 1800 -c " count | getline) {
		sub("\r", "");
		primes[idx++] = $0;
	}
	
	#for (name in lists) {
	#	# debug
	#	print name, macaddrs[name], lists[name], idxes[name], primes[idxes[name]];
	#}
}

{
	if (status < 2) {
		if ($1 ~ /IoTHubName/) {
			print "        \"IoTHubName\"" ":" " \"" IoTHubName "\",";
			status++;
		}
		else if ($1 ~ /IoTHubSuffix/) {
			print "        \"IoTHubSuffix\"" ":" " \"" IoTHubSuffix "\",";
			status++;
		}
		else {
			print;
		}
		next;
	}
	else if (status < 3) {
		if ($1 ~ /args/) {
			print "      \"args\": [";

			##count = length(lists);
			i = 0;
			for (name in lists) {
				#print "*", i++,  name, lists[name]; #debug
				i++;
				macAddress = macaddrs[name];
				deviceId = pDomain "." name;
				deviceKey = lists[name];
				
				print "        {"
				print "          \"macAddress\": \"" macAddress "\","
				print "          \"deviceId\": \"" deviceId "\","
				print "          \"deviceKey\": \"" deviceKey "\""
				printf("        }%s\n", i == count ? "" : ",");
			}
			status++;
		}
		else {
			print;
		}
		next;
        }
	else if (status < 4) {
		if ($1 == "},") {
			print;

			for (name in lists) {
                                #print "#", name, lists[name], status; #debug
				macAddress = macaddrs[name];
				filename = "/var/tmp/dpride/" name;
			print "    {"
			print "      \"name\": \"" name "\","
			print "      \"loader\": {"
			print "        \"name\": \"native\","
			print "        \"entrypoint\": {"
			print "          \"module.path\": \"./modules/simulated_device/libsimulated_device.so\""
			print "        }"
			print "      },"
			print "      \"args\": {"
			print "        \"macAddress\": \"" macAddress "\","
			print "        \"filename\": \"" filename "\","
			print "        \"dataUnit\": \"u\","
			print "        \"messagePeriod\": " primes[idxes[name]]
			print  "      }"
			print "    },"
			}
			status++;
		}
		else {
			print;
		}
		next;
	}
	else if (status < 5) {
		if ($1 ~ /links/) {
			status++;
		}
		print;
                next;
	}
	else if (status < 6) {
		if ($1 == "},") {
			print;

			for (name in lists) {
                                #print "#", name, status; #debug
			print "    {"
			print "      \"source\": \"" name "\","
			print "      \"sink\": \"mapping\""
			print "    },"
			}
			status++;
		}
		else {
                        print;
                }
                next;
	}
	else if (status < 7) {
		if ($1 == "},") {
			print;

			for (name in lists) {
                                #print "#", name, status; #debug
			print "    {"
			print "      \"source\": \"mapping\","
			print "      \"sink\": \"" name "\""
			print "    },"
			}
			status++;
		}
		else {
                        print;
                }
                next;
	}
	else {
		print;
	}
}

#!/usr/bin/gawk -f

BEGIN {
    itemCount = 0;
    seqs = 0;
    eep = "";
    count = 0;
    lineCount = 0;
    title = "";
    eepRenamed = "";
}

/^\*\*/ {
    #
    # Finish last line
    #
    if (eep != "") {
    # output last line haeader
    print "{1,\"\",\"\",0,0,0,0,0,0,\"\",{{0, NULL}}} };";
    printf( \
   	  "if (SaveEep(&EepTable[count], %d, \"%s\", \"%s\", &%s[0]) > 0) count++;\n", \
        lineCount, eep, title, eepRenamed);
    }

    #
    # Start new line
    #
    itemCount++;
    seqs = $2;
    eep = $3;
    count = $4;
    title = gensub(/.*<(.*)>.*/, "\\1", 1, $0);
    lineCount = 0;

    ## New EEP
    eepRenamed = eep;
    gsub("-", "_", eepRenamed);

    #printf("%d-%d: %s %s [%s]\n",
    #    itemCount, seqs, eep, count, title)

	printf("static DATAFIELD %s[] = {\n", eepRenamed);  
}

/^ / {
    lineCount++;
    separater = index($1, ":");
    type = substr($1, 1, separater-1);

    typeNum = 0;
    if (type == "Data")
        typeNum = 1;
    else if (type == "Flag")
        typeNum = 2;
    else if (type == "Enum")
        typeNum = 3;

    dataName = gensub(/.*\:(.*)\{.*/, "\\1", 1, $0);
    shortCut = gensub(/.*\{(.*)\}.*/, "\\1", 1, $0);
    numberField = gensub(/.*\}(.*)\[.*/, "\\1", 1, $0);
    unit = gensub(/.*\[(.*)\]/, "\\1", 1, $0);

    if (unit == "(null)")
        unit = "NULL";
    else
        unit = "\"" unit "\"";
        
    n = split(numberField, numbers, " ");
    bitOffs = numbers[1];
    bitSize = numbers[2];
    rangeMin = numbers[3];
    rangeMax = numbers[4];
    scaleMin = numbers[5];
    scaleMax = numbers[6];

    #printf(" %s:%s(%d) [%s](%s) %d %d %d %d %.3f %.3f [%s]\n",
    #    lineCount, type, typeNum,
    #    dataName, shortCut,
    #    bitOffs, bitSize,
    #    rangeMin, rangeMax,
    #    scaleMin, scaleMax,
    #    unit); 

    printf(" {%d, \"%s\", \"%s\", %d, %d, %d, %d, %.3f, %.3f, %s, {{0, NULL}}},\n",
        typeNum,
        dataName, shortCut,
        bitOffs, bitSize,
        rangeMin, rangeMax,
        scaleMin, scaleMax,
        unit); 
}

END {
    #
    # Finish last line
    #
    if (eep != "") {
    # output last line haeader
    print "{1,\"\",\"\",0,0,0,0,0,0,\"\",{{0, NULL}}} };";
    printf( \
   	  "if (SaveEep(&EepTable[count], %d, \"%s\", \"%s\", &%s[0]) > 0) count++;\n", \
        lineCount, eep, title, eepRenamed);
    }
}

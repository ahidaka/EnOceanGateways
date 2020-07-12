static DATAFIELD F6_01_01[] = {
 {3, "Push button", "PB", 3, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 1, "F6-01-01", "Switch Buttons", &F6_01_01[0]) > 0) count++;
static DATAFIELD F6_02_01[] = {
 {3, "Rocker 1st action", "R1", 0, 3, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "Energy Bow", "EB", 3, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "Rocker 2nd action", "R2", 4, 3, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "2nd Action", "SA", 7, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "Number of buttons pressed simultaneously (other bit combinations are not valid)", "R1", 0, 3, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "Energy Bow", "EB", 3, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 6, "F6-02-01", "Rocker Switch- 2 Rocker", &F6_02_01[0]) > 0) count++;
static DATAFIELD F6_02_02[] = {
 {3, "Rocker 1st action", "R1", 0, 3, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "Energy Bow", "EB", 3, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "Rocker 2nd action", "R2", 4, 3, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "2nd Action", "SA", 7, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "Number of buttons pressed simultaneously (other bit combinations are not valid)", "R1", 0, 3, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "Energy Bow", "EB", 3, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 6, "F6-02-02", "Rocker Switch- 2 Rocker", &F6_02_02[0]) > 0) count++;
static DATAFIELD F6_02_03[] = {
 {3, "Rocker action", "RA", 0, 8, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 1, "F6-02-03", "Rocker Switch- 2 Rocker", &F6_02_03[0]) > 0) count++;
static DATAFIELD F6_02_04[] = {
 {3, "Energy Bow", "EBO", 0, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "Button coding", "BC", 1, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "BI", "RBI", 4, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "B0", "RB0", 5, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "AI", "RAI", 6, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "A0", "RA0", 7, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 6, "F6-02-04", "Rocker Switch- 2 Rocker", &F6_02_04[0]) > 0) count++;
static DATAFIELD F6_03_01[] = {
 {3, "Rocker 1st action", "R1", 0, 3, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "Energy Bow", "EB", 3, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "Rocker 2nd action", "R2", 4, 3, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "2nd Action", "SA", 7, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "Number of buttons pressed simultaneously", "R1", 0, 3, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "Energy Bow", "EB", 3, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 6, "F6-03-01", "Rocker Switch- 4 Rocker", &F6_03_01[0]) > 0) count++;
static DATAFIELD F6_03_02[] = {
 {3, "Rocker 1st action", "R1", 0, 3, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "Energy Bow", "EB", 3, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "Rocker 2nd action", "R2", 4, 3, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "2nd Action", "SA", 7, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "Number of buttons pressed simultaneously", "R1", 0, 3, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "Energy Bow", "EB", 3, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 6, "F6-03-02", "Rocker Switch- 4 Rocker", &F6_03_02[0]) > 0) count++;
static DATAFIELD F6_04_01[] = {
 {3, "Key Card", "KC", 0, 8, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "Key Card", "KC", 0, 8, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 2, "F6-04-01", "Position Switch- Home and Office Application", &F6_04_01[0]) > 0) count++;
static DATAFIELD F6_04_02[] = {
 {3, "Energy Bow", "EBO", 0, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "Button coding", "BC", 1, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "State of card", "SOC", 5, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 3, "F6-04-02", "Position Switch- Home and Office Application", &F6_04_02[0]) > 0) count++;
static DATAFIELD F6_05_00[] = {
 {3, "Status", "WND", 0, 8, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 1, "F6-05-00", "Detectors", &F6_05_00[0]) > 0) count++;
static DATAFIELD F6_05_01[] = {
 {3, "Water sensor", "WAS", 0, 8, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 1, "F6-05-01", "Detectors", &F6_05_01[0]) > 0) count++;
static DATAFIELD F6_05_02[] = {
 {3, "Status", "SMO", 0, 8, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 1, "F6-05-02", "Detectors", &F6_05_02[0]) > 0) count++;
static DATAFIELD F6_10_00[] = {
 {3, "Window handle", "WIN", 0, 8, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 1, "F6-10-00", "Mechanical Handle", &F6_10_00[0]) > 0) count++;
static DATAFIELD F6_10_01[] = {
 {3, "Handle coding", "HC", 1, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "Handle value", "HVL", 4, 4, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 2, "F6-10-01", "Mechanical Handle", &F6_10_01[0]) > 0) count++;
static DATAFIELD D5_10_01[] = {
 {3, "Handle coding", "HC", 1, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "Handle value", "HVL", 4, 4, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 2, "D5-10-01", "Mechanical Handle", &D5_10_01[0]) > 0) count++;
static DATAFIELD D5_00_01[] = {
 {3, "Contact", "CO", 7, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "Learn Button", "LRN", 4, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 2, "D5-00-01", "Contacts and Switches", &D5_00_01[0]) > 0) count++;
static DATAFIELD A5_00_01[] = {
 {3, "Contact", "CO", 7, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "Learn Button", "LRN", 4, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 2, "A5-00-01", "Contacts and Switches", &A5_00_01[0]) > 0) count++;
static DATAFIELD A5_02_01[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Temperature", "TMP", 16, 8, 255, 0, -40.000, 0.000, "°C", {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 2, "A5-02-01", "Temperature Sensors", &A5_02_01[0]) > 0) count++;
static DATAFIELD A5_02_02[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Temperature", "TMP", 16, 8, 255, 0, -30.000, 10.000, "°C", {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 2, "A5-02-02", "Temperature Sensors", &A5_02_02[0]) > 0) count++;
static DATAFIELD A5_02_03[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Temperature", "TMP", 16, 8, 255, 0, -20.000, 20.000, "°C", {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 2, "A5-02-03", "Temperature Sensors", &A5_02_03[0]) > 0) count++;
static DATAFIELD A5_02_04[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Temperature", "TMP", 16, 8, 255, 0, -10.000, 30.000, "°C", {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 2, "A5-02-04", "Temperature Sensors", &A5_02_04[0]) > 0) count++;
static DATAFIELD A5_02_05[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Temperature", "TMP", 16, 8, 255, 0, 0.000, 40.000, "°C", {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 2, "A5-02-05", "Temperature Sensors", &A5_02_05[0]) > 0) count++;
static DATAFIELD A5_02_06[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Temperature", "TMP", 16, 8, 255, 0, 10.000, 50.000, "°C", {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 2, "A5-02-06", "Temperature Sensors", &A5_02_06[0]) > 0) count++;
static DATAFIELD A5_02_07[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Temperature", "TMP", 16, 8, 255, 0, 20.000, 60.000, "°C", {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 2, "A5-02-07", "Temperature Sensors", &A5_02_07[0]) > 0) count++;
static DATAFIELD A5_02_08[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Temperature", "TMP", 16, 8, 255, 0, 30.000, 70.000, "°C", {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 2, "A5-02-08", "Temperature Sensors", &A5_02_08[0]) > 0) count++;
static DATAFIELD A5_02_09[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Temperature", "TMP", 16, 8, 255, 0, 40.000, 80.000, "°C", {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 2, "A5-02-09", "Temperature Sensors", &A5_02_09[0]) > 0) count++;
static DATAFIELD A5_02_0A[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Temperature", "TMP", 16, 8, 255, 0, 50.000, 90.000, "°C", {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 2, "A5-02-0A", "Temperature Sensors", &A5_02_0A[0]) > 0) count++;
static DATAFIELD A5_02_0B[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Temperature", "TMP", 16, 8, 255, 0, 60.000, 100.000, "°C", {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 2, "A5-02-0B", "Temperature Sensors", &A5_02_0B[0]) > 0) count++;
static DATAFIELD A5_02_10[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Temperature", "TMP", 16, 8, 255, 0, -60.000, 20.000, "°C", {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 2, "A5-02-10", "Temperature Sensors", &A5_02_10[0]) > 0) count++;
static DATAFIELD A5_02_11[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Temperature", "TMP", 16, 8, 255, 0, -50.000, 30.000, "°C", {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 2, "A5-02-11", "Temperature Sensors", &A5_02_11[0]) > 0) count++;
static DATAFIELD A5_02_12[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Temperature", "TMP", 16, 8, 255, 0, -40.000, 40.000, "°C", {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 2, "A5-02-12", "Temperature Sensors", &A5_02_12[0]) > 0) count++;
static DATAFIELD A5_02_13[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Temperature", "TMP", 16, 8, 255, 0, -30.000, 50.000, "°C", {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 2, "A5-02-13", "Temperature Sensors", &A5_02_13[0]) > 0) count++;
static DATAFIELD A5_02_14[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Temperature", "TMP", 16, 8, 255, 0, -20.000, 60.000, "°C", {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 2, "A5-02-14", "Temperature Sensors", &A5_02_14[0]) > 0) count++;
static DATAFIELD A5_02_15[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Temperature", "TMP", 16, 8, 255, 0, -10.000, 70.000, "°C", {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 2, "A5-02-15", "Temperature Sensors", &A5_02_15[0]) > 0) count++;
static DATAFIELD A5_02_16[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Temperature", "TMP", 16, 8, 255, 0, 0.000, 80.000, "°C", {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 2, "A5-02-16", "Temperature Sensors", &A5_02_16[0]) > 0) count++;
static DATAFIELD A5_02_17[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Temperature", "TMP", 16, 8, 255, 0, 10.000, 90.000, "°C", {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 2, "A5-02-17", "Temperature Sensors", &A5_02_17[0]) > 0) count++;
static DATAFIELD A5_02_18[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Temperature", "TMP", 16, 8, 255, 0, 20.000, 100.000, "°C", {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 2, "A5-02-18", "Temperature Sensors", &A5_02_18[0]) > 0) count++;
static DATAFIELD A5_02_19[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Temperature", "TMP", 16, 8, 255, 0, 30.000, 110.000, "°C", {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 2, "A5-02-19", "Temperature Sensors", &A5_02_19[0]) > 0) count++;
static DATAFIELD A5_02_1A[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Temperature", "TMP", 16, 8, 255, 0, 40.000, 120.000, "°C", {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 2, "A5-02-1A", "Temperature Sensors", &A5_02_1A[0]) > 0) count++;
static DATAFIELD A5_02_1B[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Temperature", "TMP", 16, 8, 255, 0, 50.000, 130.000, "°C", {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 2, "A5-02-1B", "Temperature Sensors", &A5_02_1B[0]) > 0) count++;
static DATAFIELD A5_02_20[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Temperature", "TMP", 14, 10, 1023, 0, -10.000, 41.200, "°C", {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 2, "A5-02-20", "Temperature Sensors", &A5_02_20[0]) > 0) count++;
static DATAFIELD A5_02_30[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Temperature", "TMP", 14, 10, 1023, 0, -40.000, 62.300, "°C", {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 2, "A5-02-30", "Temperature Sensors", &A5_02_30[0]) > 0) count++;
static DATAFIELD A5_04_01[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Humidity", "HUM", 8, 8, 0, 250, 0.000, 100.000, "%", {{0, NULL}}},
 {1, "Temperature", "TMP", 16, 8, 0, 250, 0.000, 40.000, "°C", {{0, NULL}}},
 {3, "T-Sensor", "TSN", 30, 1, 0, 250, 0.000, 40.000, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 4, "A5-04-01", "Temperature and Humidity Sensor", &A5_04_01[0]) > 0) count++;
static DATAFIELD A5_04_02[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Humidity", "HUM", 8, 8, 0, 250, 0.000, 100.000, "%", {{0, NULL}}},
 {1, "Temperature", "TMP", 16, 8, 0, 250, -20.000, 60.000, "°C", {{0, NULL}}},
 {3, "T-Sensor", "TSN", 30, 1, 0, 250, -20.000, 60.000, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 4, "A5-04-02", "Temperature and Humidity Sensor", &A5_04_02[0]) > 0) count++;
static DATAFIELD A5_04_03[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Humidity", "HUM", 0, 8, 0, 255, 0.000, 100.000, "%", {{0, NULL}}},
 {1, "Temperature", "TMP", 14, 10, 0, 1023, -20.000, 60.000, "°C", {{0, NULL}}},
 {3, "Telegram Type", "TTP", 31, 1, 0, 1023, -20.000, 60.000, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 4, "A5-04-03", "Temperature and Humidity Sensor", &A5_04_03[0]) > 0) count++;
static DATAFIELD A5_04_04[] = {
 {1, "humidity", "(null)", 0, 8, 0, 199, 0.000, 100.000, "%", {{0, NULL}}},
 {1, "temperature", "(null)", 12, 12, 0, 1599, -40.000, 120.000, "°C", {{0, NULL}}},
 {3, "LrnBit", "(null)", 28, 1, 0, 1599, -40.000, 120.000, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 3, "A5-04-04", "Temperature and Humidity Sensor", &A5_04_04[0]) > 0) count++;
static DATAFIELD A5_05_01[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Barometer", "BAR", 6, 10, 0, 1023, 500.000, 1150.000, "hPa", {{0, NULL}}},
 {3, "Telegram Type", "TTP", 31, 1, 0, 1023, 500.000, 1150.000, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 3, "A5-05-01", "Barometric Sensor", &A5_05_01[0]) > 0) count++;
static DATAFIELD A5_06_01[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Supply voltage", "SVC", 0, 8, 0, 255, 0.000, 5.100, "V", {{0, NULL}}},
 {1, "Illumination", "ILL2", 8, 8, 0, 255, 300.000, 30000.000, "lx", {{0, NULL}}},
 {1, "Illumination", "ILL1", 16, 8, 0, 255, 600.000, 60000.000, "lx", {{0, NULL}}},
 {3, "Range select", "RS", 31, 1, 0, 255, 600.000, 60000.000, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 5, "A5-06-01", "Light Sensor", &A5_06_01[0]) > 0) count++;
static DATAFIELD A5_06_02[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Supply voltage", "SVC", 0, 8, 0, 255, 0.000, 5.100, "V", {{0, NULL}}},
 {1, "Illumination", "ILL2", 8, 8, 0, 255, 0.000, 510.000, "lx", {{0, NULL}}},
 {1, "Illumination", "ILL1", 16, 8, 0, 255, 0.000, 1020.000, "lx", {{0, NULL}}},
 {3, "Range select", "RS", 31, 1, 0, 255, 0.000, 1020.000, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 5, "A5-06-02", "Light Sensor", &A5_06_02[0]) > 0) count++;
static DATAFIELD A5_06_03[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Supply voltage", "SVC", 0, 8, 0, 250, 0.000, 5.000, "V", {{0, NULL}}},
 {1, "Illumination", "ILL", 8, 10, 0, 1000, 0.000, 1000.000, "lx", {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 3, "A5-06-03", "Light Sensor", &A5_06_03[0]) > 0) count++;
static DATAFIELD A5_06_04[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Temperature", "TEMP", 0, 8, 0, 255, -20.000, 60.000, "°C", {{0, NULL}}},
 {1, "Illuminance", "ILL", 8, 16, 0, 65535, 0.000, 65535.000, "lx", {{0, NULL}}},
 {1, "Energy Storage", "SV", 24, 4, 0, 15, 0.000, 100.000, "%", {{0, NULL}}},
 {3, "Temperature Availability", "TMPAV", 30, 1, 0, 15, 0.000, 100.000, NULL, {{0, NULL}}},
 {3, "Energy Storage Availability", "ENAV", 31, 1, 0, 15, 0.000, 100.000, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 6, "A5-06-04", "Light Sensor", &A5_06_04[0]) > 0) count++;
static DATAFIELD A5_06_05[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Supply voltage", "SVC", 0, 8, 0, 255, 0.000, 5.100, "V", {{0, NULL}}},
 {1, "Illumination", "ILL2", 8, 8, 0, 255, 0.000, 5100.000, "lx", {{0, NULL}}},
 {1, "Illumination", "ILL1", 16, 8, 0, 255, 0.000, 10200.000, "lx", {{0, NULL}}},
 {3, "Range select", "RS", 31, 1, 0, 255, 0.000, 10200.000, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 5, "A5-06-05", "Light Sensor", &A5_06_05[0]) > 0) count++;
static DATAFIELD A5_07_01[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Supply voltage (OPTIONAL)", "SVC", 0, 8, 0, 250, 0.000, 5.000, "V", {{0, NULL}}},
 {3, "PIR Status", "PIRS", 16, 8, 0, 250, 0.000, 5.000, NULL, {{0, NULL}}},
 {3, "Supply voltage availability", "SVA", 31, 1, 0, 250, 0.000, 5.000, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 4, "A5-07-01", "Occupancy Sensor", &A5_07_01[0]) > 0) count++;
static DATAFIELD A5_07_02[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Supply voltage (REQUIRED)", "SVC", 0, 8, 0, 250, 0.000, 5.000, "V", {{0, NULL}}},
 {3, "PIR Status", "PIRS", 24, 1, 0, 250, 0.000, 5.000, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 3, "A5-07-02", "Occupancy Sensor", &A5_07_02[0]) > 0) count++;
static DATAFIELD A5_07_03[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Supply voltage (REQUIRED)", "SVC", 0, 8, 0, 250, 0.000, 5.000, "V", {{0, NULL}}},
 {1, "Illumination", "ILL", 8, 10, 0, 1000, 0.000, 1000.000, "lx", {{0, NULL}}},
 {3, "PIR Status", "PIRS", 24, 1, 0, 1000, 0.000, 1000.000, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 4, "A5-07-03", "Occupancy Sensor", &A5_07_03[0]) > 0) count++;
static DATAFIELD A5_08_01[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Supply voltage", "SVC", 0, 8, 0, 255, 0.000, 5.100, "V", {{0, NULL}}},
 {1, "Illumination", "ILL", 8, 8, 0, 255, 0.000, 510.000, "lx", {{0, NULL}}},
 {1, "Temperature", "TMP", 16, 8, 0, 255, 0.000, 51.000, "°C", {{0, NULL}}},
 {3, "PIR Status", "PIRS", 30, 1, 0, 255, 0.000, 51.000, NULL, {{0, NULL}}},
 {3, "Occupancy Button", "OCC", 31, 1, 0, 255, 0.000, 51.000, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 6, "A5-08-01", "Light- Temperature and Occupancy Sensor", &A5_08_01[0]) > 0) count++;
static DATAFIELD A5_08_02[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Supply voltage", "SVC", 0, 8, 0, 255, 0.000, 5.100, "V", {{0, NULL}}},
 {1, "Illumination", "ILL", 8, 8, 0, 255, 0.000, 1020.000, "lx", {{0, NULL}}},
 {1, "Temperature", "TMP", 16, 8, 0, 255, 0.000, 51.000, "°C", {{0, NULL}}},
 {3, "PIR Status", "PIRS", 30, 1, 0, 255, 0.000, 51.000, NULL, {{0, NULL}}},
 {3, "Occupancy Button", "OCC", 31, 1, 0, 255, 0.000, 51.000, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 6, "A5-08-02", "Light- Temperature and Occupancy Sensor", &A5_08_02[0]) > 0) count++;
static DATAFIELD A5_08_03[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Supply voltage", "SVC", 0, 8, 0, 255, 0.000, 5.100, "V", {{0, NULL}}},
 {1, "Illumination", "ILL", 8, 8, 0, 255, 0.000, 1530.000, "lx", {{0, NULL}}},
 {1, "Temperature", "TMP", 16, 8, 0, 255, -30.000, 50.000, "°C", {{0, NULL}}},
 {3, "PIR Status", "PIRS", 30, 1, 0, 255, -30.000, 50.000, NULL, {{0, NULL}}},
 {3, "Occupancy Button", "OCC", 31, 1, 0, 255, -30.000, 50.000, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 6, "A5-08-03", "Light- Temperature and Occupancy Sensor", &A5_08_03[0]) > 0) count++;
static DATAFIELD A5_09_02[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Supply voltage", "SVC", 0, 8, 0, 255, 0.000, 5.100, "V", {{0, NULL}}},
 {1, "Concentration", "Conc", 8, 8, 0, 255, 0.000, 1020.000, "ppm", {{0, NULL}}},
 {1, "Temperature", "TMP", 16, 8, 0, 255, 0.000, 51.000, "°C", {{0, NULL}}},
 {3, "T-Sensor", "TSN", 30, 1, 0, 255, 0.000, 51.000, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 5, "A5-09-02", "Gas Sensor", &A5_09_02[0]) > 0) count++;
static DATAFIELD A5_09_04[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Humidity", "HUM", 0, 8, 0, 200, 0.000, 100.000, "%", {{0, NULL}}},
 {1, "Concentration", "Conc", 8, 8, 0, 255, 0.000, 2550.000, "ppm", {{0, NULL}}},
 {1, "Temperature", "TMP", 16, 8, 0, 255, 0.000, 51.000, "°C", {{0, NULL}}},
 {3, "H-Sensor", "HSN", 29, 1, 0, 255, 0.000, 51.000, NULL, {{0, NULL}}},
 {3, "T-Sensor", "TSN", 30, 1, 0, 255, 0.000, 51.000, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 6, "A5-09-04", "Gas Sensor", &A5_09_04[0]) > 0) count++;
static DATAFIELD A5_09_05[] = {
 {1, "VOC", "Conc", 0, 16, 0, 65535, 0.000, 65535.000, "ppb", {{0, NULL}}},
 {3, "VOC ID", "VOC_ID", 16, 8, 0, 65535, 0.000, 65535.000, NULL, {{0, NULL}}},
 {1, "LRN Bit", "LRNB", 28, 1, 0, 65535, 0.000, 65535.000, NULL, {{0, NULL}}},
 {1, "Scale Multiplier", "SCM", 30, 2, 0, 65535, 0.000, 65535.000, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 4, "A5-09-05", "Gas Sensor", &A5_09_05[0]) > 0) count++;
static DATAFIELD A5_09_06[] = {
 {1, "Radon", "Act", 0, 10, 0, 1023, 0.000, 1023.000, "Bq/m3", {{0, NULL}}},
 {3, "LRN Bit", "LRNB", 28, 1, 0, 1023, 0.000, 1023.000, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 2, "A5-09-06", "Gas Sensor", &A5_09_06[0]) > 0) count++;
static DATAFIELD A5_09_07[] = {
 {1, "Particles_10", "PM10", 0, 9, 0, 511, 0.000, 511.000, "µg/m3", {{0, NULL}}},
 {1, "Particles_2.5", "PM2.5", 9, 9, 0, 511, 0.000, 511.000, "µg/m3", {{0, NULL}}},
 {1, "Particles_1", "PM1", 18, 9, 0, 511, 0.000, 511.000, "µg/m3", {{0, NULL}}},
 {3, "LRN Bit", "LRNB", 28, 1, 0, 511, 0.000, 511.000, NULL, {{0, NULL}}},
 {3, "PM10 active", "PM10a", 29, 1, 0, 511, 0.000, 511.000, NULL, {{0, NULL}}},
 {3, "PM2.5 active", "PM2.5a", 30, 1, 0, 511, 0.000, 511.000, NULL, {{0, NULL}}},
 {3, "PM1 active", "PM1a", 31, 1, 0, 511, 0.000, 511.000, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 7, "A5-09-07", "Gas Sensor", &A5_09_07[0]) > 0) count++;
static DATAFIELD A5_09_08[] = {
 {1, "CO2", "CO2", 16, 8, 0, 255, 0.000, 2000.000, "ppm", {{0, NULL}}},
 {3, "LRN Bit", "LRNB", 28, 1, 0, 255, 0.000, 2000.000, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 2, "A5-09-08", "Gas Sensor", &A5_09_08[0]) > 0) count++;
static DATAFIELD A5_09_09[] = {
 {1, "CO2", "CO2", 16, 8, 0, 255, 0.000, 2000.000, "ppm", {{0, NULL}}},
 {3, "LRN Bit", "LRNB", 28, 1, 0, 255, 0.000, 2000.000, NULL, {{0, NULL}}},
 {3, "Power Failure detection", "PFD", 29, 1, 0, 255, 0.000, 2000.000, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 3, "A5-09-09", "Gas Sensor", &A5_09_09[0]) > 0) count++;
static DATAFIELD A5_09_0A[] = {
 {1, "Concentration", "Conc", 0, 16, 0, 65535, 0.000, 65535.000, "ppm", {{0, NULL}}},
 {1, "Temperature", "TEMP", 16, 8, 0, 255, -20.000, 60.000, "°C", {{0, NULL}}},
 {1, "Supply voltage", "SV", 24, 4, 0, 15, 2.000, 5.000, "V", {{0, NULL}}},
 {3, "LRN Bit", "LRNB", 28, 1, 0, 15, 2.000, 5.000, NULL, {{0, NULL}}},
 {3, "Temp sensor availability", "TSA", 30, 1, 0, 15, 2.000, 5.000, NULL, {{0, NULL}}},
 {3, "Supply voltage availability", "SVA", 31, 1, 0, 15, 2.000, 5.000, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 6, "A5-09-0A", "Gas Sensor", &A5_09_0A[0]) > 0) count++;
static DATAFIELD A5_09_0B[] = {
 {1, "Supply voltage", "SV", 0, 4, 0, 15, 2.000, 5.000, "V", {{0, NULL}}},
 {1, "Radioactivity", "Ract", 8, 16, 0, 65535, 0.000, 6553.000, "According to", {{0, NULL}}},
 {3, "LRN Bit", "LRNB", 28, 1, 0, 65535, 0.000, 6553.000, NULL, {{0, NULL}}},
 {3, "Scale Multiplier", "SCM", 24, 4, 0, 65535, 0.000, 6553.000, NULL, {{0, NULL}}},
 {3, "Value unit", "VUNIT", 29, 2, 0, 65535, 0.000, 6553.000, NULL, {{0, NULL}}},
 {3, "Supply voltage availability", "SVA", 31, 1, 0, 65535, 0.000, 6553.000, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 6, "A5-09-0B", "Gas Sensor", &A5_09_0B[0]) > 0) count++;
static DATAFIELD A5_09_0C[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "VOC", "Conc", 0, 16, 0, 65535, 0.000, 65535.000, "Unit", {{0, NULL}}},
 {3, "VOC ID*", "VOC ID", 16, 8, 0, 65535, 0.000, 65535.000, NULL, {{0, NULL}}},
 {1, "Unit", "Unit", 29, 1, 0, 65535, 0.000, 65535.000, NULL, {{0, NULL}}},
 {1, "Scale Multiplier", "SCM", 30, 2, 0, 65535, 0.000, 65535.000, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 5, "A5-09-0C", "Gas Sensor", &A5_09_0C[0]) > 0) count++;
static DATAFIELD A5_10_01[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "Turn-switch for fan speed", "FAN", 0, 8, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Set point", "SP", 8, 8, 0, 255, 0.000, 255.000, "N/A", {{0, NULL}}},
 {1, "Temperature", "TMP", 16, 8, 255, 0, 0.000, 40.000, "°C", {{0, NULL}}},
 {3, "Occupancy", "OCC", 31, 1, 255, 0, 0.000, 40.000, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 5, "A5-10-01", "Room Operating Panel", &A5_10_01[0]) > 0) count++;
static DATAFIELD A5_10_02[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "Turn-switch for fan speed", "FAN", 0, 8, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Set point", "SP", 8, 8, 0, 255, 0.000, 255.000, "N/A", {{0, NULL}}},
 {1, "Temperature", "TMP", 16, 8, 255, 0, 0.000, 40.000, "°C", {{0, NULL}}},
 {3, "Slide switch 0/I", "SLSW", 31, 1, 255, 0, 0.000, 40.000, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 5, "A5-10-02", "Room Operating Panel", &A5_10_02[0]) > 0) count++;
static DATAFIELD A5_10_03[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Set point", "SP", 8, 8, 0, 255, 0.000, 255.000, "N/A", {{0, NULL}}},
 {1, "Temperature", "TMP", 16, 8, 255, 0, 0.000, 40.000, "°C", {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 3, "A5-10-03", "Room Operating Panel", &A5_10_03[0]) > 0) count++;
static DATAFIELD A5_10_04[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "Turn-switch for fan speed", "FAN", 0, 8, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Set point", "SP", 8, 8, 0, 255, 0.000, 255.000, "N/A", {{0, NULL}}},
 {1, "Temperature", "TMP", 16, 8, 255, 0, 0.000, 40.000, "°C", {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 4, "A5-10-04", "Room Operating Panel", &A5_10_04[0]) > 0) count++;
static DATAFIELD A5_10_05[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Set point", "SP", 8, 8, 0, 255, 0.000, 255.000, "N/A", {{0, NULL}}},
 {1, "Temperature", "TMP", 16, 8, 255, 0, 0.000, 40.000, "°C", {{0, NULL}}},
 {3, "Occupancy", "OCC", 31, 1, 255, 0, 0.000, 40.000, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 4, "A5-10-05", "Room Operating Panel", &A5_10_05[0]) > 0) count++;
static DATAFIELD A5_10_06[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Set point", "SP", 8, 8, 0, 255, 0.000, 255.000, "N/A", {{0, NULL}}},
 {1, "Temperature", "TMP", 16, 8, 255, 0, 0.000, 40.000, "°C", {{0, NULL}}},
 {3, "Slide switch 0/I", "SLSW", 31, 1, 255, 0, 0.000, 40.000, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 4, "A5-10-06", "Room Operating Panel", &A5_10_06[0]) > 0) count++;
static DATAFIELD A5_10_07[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "Turn-switch for fan speed", "FAN", 0, 8, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Temperature", "TMP", 16, 8, 255, 0, 0.000, 40.000, "°C", {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 3, "A5-10-07", "Room Operating Panel", &A5_10_07[0]) > 0) count++;
static DATAFIELD A5_10_08[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "Turn-switch for fan speed", "FAN", 0, 8, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Temperature", "TMP", 16, 8, 255, 0, 0.000, 40.000, "°C", {{0, NULL}}},
 {3, "Occupancy", "OCC", 31, 1, 255, 0, 0.000, 40.000, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 4, "A5-10-08", "Room Operating Panel", &A5_10_08[0]) > 0) count++;
static DATAFIELD A5_10_09[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "Turn-switch for fan speed", "FAN", 0, 8, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Temperature", "TMP", 16, 8, 255, 0, 0.000, 40.000, "°C", {{0, NULL}}},
 {3, "Slide switch 0/I", "SLSW", 31, 1, 255, 0, 0.000, 40.000, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 4, "A5-10-09", "Room Operating Panel", &A5_10_09[0]) > 0) count++;
static DATAFIELD A5_10_0A[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Set point", "SP", 8, 8, 0, 255, 0.000, 255.000, "N/A", {{0, NULL}}},
 {1, "Temperature", "TMP", 16, 8, 255, 0, 0.000, 40.000, "°C", {{0, NULL}}},
 {3, "Contact State", "CTST", 31, 1, 255, 0, 0.000, 40.000, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 4, "A5-10-0A", "Room Operating Panel", &A5_10_0A[0]) > 0) count++;
static DATAFIELD A5_10_0B[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Temperature", "TMP", 16, 8, 255, 0, 0.000, 40.000, "°C", {{0, NULL}}},
 {3, "Contact State", "CTST", 31, 1, 255, 0, 0.000, 40.000, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 3, "A5-10-0B", "Room Operating Panel", &A5_10_0B[0]) > 0) count++;
static DATAFIELD A5_10_0C[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Temperature", "TMP", 16, 8, 255, 0, 0.000, 40.000, "°C", {{0, NULL}}},
 {3, "Occupancy", "OCC", 31, 1, 255, 0, 0.000, 40.000, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 3, "A5-10-0C", "Room Operating Panel", &A5_10_0C[0]) > 0) count++;
static DATAFIELD A5_10_0D[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Temperature", "TMP", 16, 8, 255, 0, 0.000, 40.000, "°C", {{0, NULL}}},
 {3, "Slide switch", "SLSW", 31, 1, 255, 0, 0.000, 40.000, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 3, "A5-10-0D", "Room Operating Panel", &A5_10_0D[0]) > 0) count++;
static DATAFIELD A5_10_10[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Set point", "SP", 0, 8, 0, 255, 0.000, 255.000, "N/A", {{0, NULL}}},
 {1, "Humidity", "HUM", 8, 8, 0, 250, 0.000, 100.000, "%", {{0, NULL}}},
 {1, "Temperature", "TMP", 16, 8, 0, 250, 0.000, 40.000, "°C", {{0, NULL}}},
 {3, "Occupancy", "OCC", 31, 1, 0, 250, 0.000, 40.000, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 5, "A5-10-10", "Room Operating Panel", &A5_10_10[0]) > 0) count++;
static DATAFIELD A5_10_11[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Set point", "SP", 0, 8, 0, 255, 0.000, 255.000, "N/A", {{0, NULL}}},
 {1, "Humidity", "HUM", 8, 8, 0, 250, 0.000, 100.000, "%", {{0, NULL}}},
 {1, "Temperature", "TMP", 16, 8, 0, 250, 0.000, 40.000, "°C", {{0, NULL}}},
 {3, "Slide switch", "SLSW", 31, 1, 0, 250, 0.000, 40.000, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 5, "A5-10-11", "Room Operating Panel", &A5_10_11[0]) > 0) count++;
static DATAFIELD A5_10_12[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Set point", "SP", 0, 8, 0, 255, 0.000, 255.000, "N/A", {{0, NULL}}},
 {1, "Humidity", "HUM", 8, 8, 0, 250, 0.000, 100.000, "%", {{0, NULL}}},
 {1, "Temperature", "TMP", 16, 8, 0, 250, 0.000, 40.000, "°C", {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 4, "A5-10-12", "Room Operating Panel", &A5_10_12[0]) > 0) count++;
static DATAFIELD A5_10_13[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Humidity", "HUM", 8, 8, 0, 250, 0.000, 100.000, "%", {{0, NULL}}},
 {1, "Temperature", "TMP", 16, 8, 0, 250, 0.000, 40.000, "°C", {{0, NULL}}},
 {3, "Occupancy", "OCC", 31, 1, 0, 250, 0.000, 40.000, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 4, "A5-10-13", "Room Operating Panel", &A5_10_13[0]) > 0) count++;
static DATAFIELD A5_10_14[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Humidity", "HUM", 8, 8, 0, 250, 0.000, 100.000, "%", {{0, NULL}}},
 {1, "Temperature", "TMP", 16, 8, 0, 250, 0.000, 40.000, "°C", {{0, NULL}}},
 {3, "Slide switch ", "SLSW", 31, 1, 0, 250, 0.000, 40.000, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 4, "A5-10-14", "Room Operating Panel", &A5_10_14[0]) > 0) count++;
static DATAFIELD A5_10_15[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Temperature", "TMP", 14, 10, 1023, 0, -10.000, 41.200, "°C", {{0, NULL}}},
 {1, "Set point", "SP", 8, 6, 0, 63, 0.000, 63.000, "N/A", {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 3, "A5-10-15", "Room Operating Panel", &A5_10_15[0]) > 0) count++;
static DATAFIELD A5_10_16[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Temperature", "TMP", 14, 10, 1023, 0, -10.000, 41.200, "°C", {{0, NULL}}},
 {1, "Set point", "SP", 8, 6, 0, 63, 0.000, 63.000, "N/A", {{0, NULL}}},
 {3, "Occupancy", "OCC", 31, 1, 0, 63, 0.000, 63.000, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 4, "A5-10-16", "Room Operating Panel", &A5_10_16[0]) > 0) count++;
static DATAFIELD A5_10_17[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Temperature", "TMP", 14, 10, 1023, 0, -10.000, 41.200, "°C", {{0, NULL}}},
 {3, "Occupancy", "OCC", 31, 1, 1023, 0, -10.000, 41.200, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 3, "A5-10-17", "Room Operating Panel", &A5_10_17[0]) > 0) count++;
static DATAFIELD A5_10_18[] = {
 {3, "Fan Speed", "FAN", 25, 3, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Illumination", "ILL", 0, 8, 0, 250, 0.000, 1000.000, "lx", {{0, NULL}}},
 {1, "Temp Setpoint", "TMPSP", 8, 8, 250, 0, 0.000, 40.000, "°C", {{0, NULL}}},
 {1, "Temperature", "TMP", 16, 8, 250, 0, 0.000, 40.000, "°C", {{0, NULL}}},
 {3, "Occupancy enable/disable", "OED", 30, 1, 250, 0, 0.000, 40.000, NULL, {{0, NULL}}},
 {3, "Occupancy button", "OB", 31, 1, 250, 0, 0.000, 40.000, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 7, "A5-10-18", "Room Operating Panel", &A5_10_18[0]) > 0) count++;
static DATAFIELD A5_10_19[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Humidity", "HUM", 0, 8, 0, 250, 0.000, 100.000, "%", {{0, NULL}}},
 {1, "Temp Setpoint", "TMP Sp", 8, 8, 250, 0, 0.000, 40.000, "°C", {{0, NULL}}},
 {1, "Temperature", "TMP", 16, 8, 250, 0, 0.000, 40.000, "°C", {{0, NULL}}},
 {3, "Fan speed", "FAN", 25, 3, 250, 0, 0.000, 40.000, NULL, {{0, NULL}}},
 {3, "Occupancy enable/disable", "OED", 31, 1, 250, 0, 0.000, 40.000, NULL, {{0, NULL}}},
 {3, "Occupancy button", "OB", 30, 1, 250, 0, 0.000, 40.000, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 7, "A5-10-19", "Room Operating Panel", &A5_10_19[0]) > 0) count++;
static DATAFIELD A5_10_1A[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Supply Voltage", "SV", 0, 8, 0, 250, 0.000, 5.000, "V", {{0, NULL}}},
 {1, "Temp Setpoint", "TMP Sp", 8, 8, 250, 0, 0.000, 40.000, "°C", {{0, NULL}}},
 {1, "Temperature", "TMP", 16, 8, 250, 0, 0.000, 40.000, "°C", {{0, NULL}}},
 {3, "Fan speed", "FAN", 25, 3, 250, 0, 0.000, 40.000, NULL, {{0, NULL}}},
 {3, "Occupancy enable/disable", "OED", 30, 1, 250, 0, 0.000, 40.000, NULL, {{0, NULL}}},
 {3, "Occupancy button", "OB", 31, 1, 250, 0, 0.000, 40.000, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 7, "A5-10-1A", "Room Operating Panel", &A5_10_1A[0]) > 0) count++;
static DATAFIELD A5_10_1B[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Supply Voltage", "SV", 0, 8, 0, 250, 0.000, 5.000, "V", {{0, NULL}}},
 {1, "Illumination", "ILL", 8, 8, 0, 250, 0.000, 1000.000, "lx", {{0, NULL}}},
 {1, "Temperature", "TMP", 16, 8, 250, 0, 0.000, 40.000, "°C", {{0, NULL}}},
 {3, "Fan speed", "FAN", 25, 3, 250, 0, 0.000, 40.000, NULL, {{0, NULL}}},
 {3, "Occupancy enable/disable", "OED", 30, 1, 250, 0, 0.000, 40.000, NULL, {{0, NULL}}},
 {3, "Occupancy button", "OB", 31, 1, 250, 0, 0.000, 40.000, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 7, "A5-10-1B", "Room Operating Panel", &A5_10_1B[0]) > 0) count++;
static DATAFIELD A5_10_1C[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Illumination", "ILL", 0, 8, 0, 250, 0.000, 1000.000, "lx", {{0, NULL}}},
 {1, "Illumination Set Point", "ILLSP", 8, 8, 0, 250, 0.000, 1000.000, "lx", {{0, NULL}}},
 {1, "Temperature", "TMP", 16, 8, 250, 0, 0.000, 40.000, "°C", {{0, NULL}}},
 {3, "Fan speed", "FAN", 25, 3, 250, 0, 0.000, 40.000, NULL, {{0, NULL}}},
 {3, "Occupancy enable/disable", "OED", 30, 1, 250, 0, 0.000, 40.000, NULL, {{0, NULL}}},
 {3, "Occupancy button", "OB", 31, 1, 250, 0, 0.000, 40.000, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 7, "A5-10-1C", "Room Operating Panel", &A5_10_1C[0]) > 0) count++;
static DATAFIELD A5_10_1D[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Humidity", "HUM", 0, 8, 0, 250, 0.000, 100.000, "%", {{0, NULL}}},
 {1, "Humidity Set Point", "HUMSP", 8, 8, 0, 250, 0.000, 100.000, "%", {{0, NULL}}},
 {1, "Temperature", "TMP", 16, 8, 250, 0, 0.000, 40.000, "°C", {{0, NULL}}},
 {3, "Fan speed", "FAN", 25, 3, 250, 0, 0.000, 40.000, NULL, {{0, NULL}}},
 {3, "Occupancy enable/disable", "OED", 30, 1, 250, 0, 0.000, 40.000, NULL, {{0, NULL}}},
 {3, "Occupancy button", "OB", 31, 1, 250, 0, 0.000, 40.000, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 7, "A5-10-1D", "Room Operating Panel", &A5_10_1D[0]) > 0) count++;
static DATAFIELD A5_10_1F[] = {
 {3, "Turn-switch for fan speed", "FAN", 0, 8, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Set Point", "SP", 8, 8, 0, 255, 0.000, 255.000, "N/A", {{0, NULL}}},
 {1, "Temperature", "TMP", 16, 8, 255, 0, 0.000, 40.000, "°C", {{0, NULL}}},
 {3, "Temperature flag", "TMP_F", 25, 1, 255, 0, 0.000, 40.000, NULL, {{0, NULL}}},
 {3, "Set point flag", "SP_F", 26, 1, 255, 0, 0.000, 40.000, NULL, {{0, NULL}}},
 {3, "Fan speed flag", "FAN_F", 27, 1, 255, 0, 0.000, 40.000, NULL, {{0, NULL}}},
 {3, "LRN Bit", "LRNB", 28, 1, 255, 0, 0.000, 40.000, NULL, {{0, NULL}}},
 {3, "Unoccupancy", "UNOCC", 30, 1, 255, 0, 0.000, 40.000, NULL, {{0, NULL}}},
 {3, "Occupancy", "OCC", 31, 1, 255, 0, 0.000, 40.000, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 9, "A5-10-1F", "Room Operating Panel", &A5_10_1F[0]) > 0) count++;
static DATAFIELD A5_10_20[] = {
 {1, "Set Point", "SP", 0, 8, 0, 255, 0.000, 255.000, "N/A", {{0, NULL}}},
 {1, "Temperature", "TMP", 16, 8, 0, 250, 0.000, 40.000, "°C", {{0, NULL}}},
 {3, "Set point mode", "SPM", 25, 2, 0, 250, 0.000, 40.000, NULL, {{0, NULL}}},
 {3, "Battery state", "BATT", 27, 1, 0, 250, 0.000, 40.000, NULL, {{0, NULL}}},
 {3, "LRN Bit", "LRNB", 28, 1, 0, 250, 0.000, 40.000, NULL, {{0, NULL}}},
 {3, "User activity", "ACT", 31, 1, 0, 250, 0.000, 40.000, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 6, "A5-10-20", "Room Operating Panel", &A5_10_20[0]) > 0) count++;
static DATAFIELD A5_10_21[] = {
 {1, "Set Point", "SP", 0, 8, 0, 255, 0.000, 255.000, "N/A", {{0, NULL}}},
 {1, "Humidity", "HUM", 8, 8, 0, 250, 0.000, 100.000, "%", {{0, NULL}}},
 {1, "Temperature", "TMP", 16, 8, 0, 250, 0.000, 40.000, "°C", {{0, NULL}}},
 {3, "Set point mode", "SPM", 25, 2, 0, 250, 0.000, 40.000, NULL, {{0, NULL}}},
 {3, "Battery state", "BATT", 27, 1, 0, 250, 0.000, 40.000, NULL, {{0, NULL}}},
 {3, "LRN Bit", "LRNB", 28, 1, 0, 250, 0.000, 40.000, NULL, {{0, NULL}}},
 {3, "User activity", "ACT", 31, 1, 0, 250, 0.000, 40.000, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 7, "A5-10-21", "Room Operating Panel", &A5_10_21[0]) > 0) count++;
static DATAFIELD A5_10_22[] = {
 {1, "Relative Setpoint", "SP", 0, 8, 0, 255, 0.000, 255.000, "N/A", {{0, NULL}}},
 {1, "Humidity", "HUM", 8, 8, 0, 250, 0.000, 100.000, "%", {{0, NULL}}},
 {1, "Temperature", "TMP", 16, 8, 0, 250, 0.000, 40.000, "°C", {{0, NULL}}},
 {3, "Fanspeed", "FAN", 24, 3, 0, 250, 0.000, 40.000, NULL, {{0, NULL}}},
 {3, "LRN Bit", "LRNB", 28, 1, 0, 250, 0.000, 40.000, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 5, "A5-10-22", "Room Operating Panel", &A5_10_22[0]) > 0) count++;
static DATAFIELD A5_10_23[] = {
 {1, "Relative Setpoint", "SP", 0, 8, 0, 255, 0.000, 255.000, "N/A", {{0, NULL}}},
 {1, "Humidity", "HUM", 8, 8, 0, 250, 0.000, 100.000, "%", {{0, NULL}}},
 {1, "Temperature", "TMP", 16, 8, 0, 250, 0.000, 40.000, "°C", {{0, NULL}}},
 {3, "Fanspeed", "FAN", 24, 3, 0, 250, 0.000, 40.000, NULL, {{0, NULL}}},
 {3, "LRN Bit", "LRNB", 28, 1, 0, 250, 0.000, 40.000, NULL, {{0, NULL}}},
 {3, "Occupancy", "OCC", 31, 1, 0, 250, 0.000, 40.000, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 6, "A5-10-23", "Room Operating Panel", &A5_10_23[0]) > 0) count++;
static DATAFIELD A5_11_01[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Illumination", "ILL", 0, 8, 0, 255, 0.000, 510.000, "lx", {{0, NULL}}},
 {1, "Illumination Set Point ", "ISP", 8, 8, 0, 255, 0.000, 255.000, "N/A", {{0, NULL}}},
 {1, "Dimming Output Level", "DIM", 16, 8, 0, 255, 0.000, 255.000, "N/A", {{0, NULL}}},
 {3, "Repeater", "REP", 24, 1, 0, 255, 0.000, 255.000, NULL, {{0, NULL}}},
 {3, "Power Relay Timer", "PRT", 25, 1, 0, 255, 0.000, 255.000, NULL, {{0, NULL}}},
 {3, "Daylight Harvesting", "DHV", 26, 1, 0, 255, 0.000, 255.000, NULL, {{0, NULL}}},
 {3, "Dimming", "EDIM", 27, 1, 0, 255, 0.000, 255.000, NULL, {{0, NULL}}},
 {3, "Magnet Contact", "MGC", 29, 1, 0, 255, 0.000, 255.000, NULL, {{0, NULL}}},
 {3, "Occupancy", "OCC", 30, 1, 0, 255, 0.000, 255.000, NULL, {{0, NULL}}},
 {3, "Power Relay", "PWR", 31, 1, 0, 255, 0.000, 255.000, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 11, "A5-11-01", "Controller Status", &A5_11_01[0]) > 0) count++;
static DATAFIELD A5_11_02[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Control Variable", "CVAR", 0, 8, 0, 255, 0.000, 100.000, "%", {{0, NULL}}},
 {3, "FanStage", "FAN", 8, 8, 0, 255, 0.000, 100.000, NULL, {{0, NULL}}},
 {1, "Actual Setpoint ", "ASP", 16, 8, 0, 255, 0.000, 51.200, "°C", {{0, NULL}}},
 {3, "Alarm", "ALR", 24, 1, 0, 255, 0.000, 51.200, NULL, {{0, NULL}}},
 {3, "Controller mode", "CTM", 25, 2, 0, 255, 0.000, 51.200, NULL, {{0, NULL}}},
 {3, "Controller state", "CST", 27, 1, 0, 255, 0.000, 51.200, NULL, {{0, NULL}}},
 {3, "Energy hold-off", "ERH", 29, 1, 0, 255, 0.000, 51.200, NULL, {{0, NULL}}},
 {3, "Room occupancy", "RO", 30, 2, 0, 255, 0.000, 51.200, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 9, "A5-11-02", "Controller Status", &A5_11_02[0]) > 0) count++;
static DATAFIELD A5_11_03[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Blind/shutter pos.", "BSP", 0, 8, 0, 100, 0.000, 100.000, "%", {{0, NULL}}},
 {3, "Angle sign", "AS", 8, 1, 0, 100, 0.000, 100.000, NULL, {{0, NULL}}},
 {1, "Angle", "AN", 9, 7, 0, 90, 0.000, 180.000, "°", {{0, NULL}}},
 {3, "Position value flag", "PVF", 16, 1, 0, 90, 0.000, 180.000, NULL, {{0, NULL}}},
 {3, "Angle value flag", "AVF", 17, 1, 0, 90, 0.000, 180.000, NULL, {{0, NULL}}},
 {3, "Error state", "ES", 18, 2, 0, 90, 0.000, 180.000, NULL, {{0, NULL}}},
 {3, "End-position", "EP", 20, 2, 0, 90, 0.000, 180.000, NULL, {{0, NULL}}},
 {3, "Status", "ST", 22, 2, 0, 90, 0.000, 180.000, NULL, {{0, NULL}}},
 {3, "Service Mode", "SM", 24, 1, 0, 90, 0.000, 180.000, NULL, {{0, NULL}}},
 {3, "Mode of the position", "MOTP", 25, 1, 0, 90, 0.000, 180.000, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 11, "A5-11-03", "Controller Status", &A5_11_03[0]) > 0) count++;
static DATAFIELD A5_11_04[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "Parameter 1", "P1", 0, 8, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "Parameter 2", "P2", 8, 8, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "Parameter 3", "P3", 16, 8, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "Service Mode", "SM", 24, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "Operating hours flag", "OHF", 25, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "Error state", "ES", 26, 2, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "Parameter Mode", "PM", 29, 2, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "Status", "ST", 31, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 9, "A5-11-04", "Controller Status", &A5_11_04[0]) > 0) count++;
static DATAFIELD A5_11_05[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "Message Type", "MT", 31, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "Working Mode", "WM", 25, 3, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "Relay Status", "RS", 29, 2, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "Message Type", "MT", 31, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 6, "A5-11-05", "Controller Status", &A5_11_05[0]) > 0) count++;
static DATAFIELD A5_12_00[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Meter reading", "MR", 0, 24, 0, 16777215, 0.000, 16777215.000, "DT", {{0, NULL}}},
 {1, "Measurement channel", "CH", 24, 4, 0, 15, 0.000, 15.000, "1", {{0, NULL}}},
 {3, "Data type (unit)", "DT", 29, 1, 0, 15, 0.000, 0.000, "1/s", {{0, NULL}}},
 {3, "Divisor (scale)", "DIV", 30, 2, 0, 15, 0.000, 16777.215, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 5, "A5-12-00", "Automated Meter Reading (AMR)", &A5_12_00[0]) > 0) count++;
static DATAFIELD A5_12_01[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Meter reading", "MR", 0, 24, 0, 16777215, 0.000, 16777215.000, "DT", {{0, NULL}}},
 {1, "Tariff info", "TI", 24, 4, 0, 15, 0.000, 15.000, "1", {{0, NULL}}},
 {3, "Data type (unit)", "DT", 29, 1, 0, 15, 0.000, 0.000, "W", {{0, NULL}}},
 {3, "Divisor (scale)", "DIV", 30, 2, 0, 15, 0.000, 16777.215, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 5, "A5-12-01", "Automated Meter Reading (AMR)", &A5_12_01[0]) > 0) count++;
static DATAFIELD A5_12_02[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "meter reading", "MR", 0, 24, 0, 16777215, 0.000, 16777215.000, "DT", {{0, NULL}}},
 {1, "Tariff info", "TI", 24, 4, 0, 15, 0.000, 15.000, "1", {{0, NULL}}},
 {3, "data type (unit)", "DT", 29, 1, 0, 15, 0.000, 0.000, "liter/s", {{0, NULL}}},
 {3, "divisor (scale)", "DIV", 30, 2, 0, 15, 0.000, 16777.215, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 5, "A5-12-02", "Automated Meter Reading (AMR)", &A5_12_02[0]) > 0) count++;
static DATAFIELD A5_12_03[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Meter reading", "MR", 0, 24, 0, 16777215, 0.000, 16777215.000, "DT", {{0, NULL}}},
 {1, "Tariff info", "TI", 24, 4, 0, 15, 0.000, 15.000, "1", {{0, NULL}}},
 {3, "Data type (unit)", "DT", 29, 1, 0, 15, 0.000, 0.000, "Liter/s", {{0, NULL}}},
 {3, "Divisor (scale)", "DIV", 30, 2, 0, 15, 0.000, 16777.215, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 5, "A5-12-03", "Automated Meter Reading (AMR)", &A5_12_03[0]) > 0) count++;
static DATAFIELD A5_12_04[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Meter reading", "MR", 0, 14, 0, 16383, 0.000, 16383.000, "gram", {{0, NULL}}},
 {1, "Temperature", "TMP", 16, 8, 0, 255, -40.000, 40.000, "°C", {{0, NULL}}},
 {3, "Battery Level", "BL", 30, 2, 0, 255, -40.000, 40.000, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 4, "A5-12-04", "Automated Meter Reading (AMR)", &A5_12_04[0]) > 0) count++;
static DATAFIELD A5_12_05[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "Position Sensor 0", "PS0", 0, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "Position Sensor 1", "PS1", 1, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "Position Sensor 2", "PS2", 2, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "Position Sensor 3", "PS3", 3, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "Position Sensor 4", "PS4", 4, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "Position Sensor 5", "PS5", 5, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "Position Sensor 6", "PS6", 6, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "Position Sensor 7", "PS7", 7, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "Position Sensor 8", "PS8", 8, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "Position Sensor 9", "PS9", 9, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Temperature", "TMP", 16, 8, 0, 255, -40.000, 40.000, "°C", {{0, NULL}}},
 {3, "Battery Level", "BL", 30, 2, 0, 255, -40.000, 40.000, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 13, "A5-12-05", "Automated Meter Reading (AMR)", &A5_12_05[0]) > 0) count++;
static DATAFIELD A5_12_10[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Meter reading", "MR", 0, 24, 0, 16777215, 0.000, 16777215.000, "DT", {{0, NULL}}},
 {1, "Measurement channel", "CH", 24, 4, 0, 15, 0.000, 15.000, NULL, {{0, NULL}}},
 {3, "Data type (unit)", "DT", 29, 1, 0, 15, 0.000, 0.000, "mA", {{0, NULL}}},
 {3, "Divisor (scale)", "DIV", 30, 2, 0, 15, 0.000, 16777.215, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 5, "A5-12-10", "Automated Meter Reading (AMR)", &A5_12_10[0]) > 0) count++;
static DATAFIELD A5_13_01[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Dawn sensor", "DWS", 0, 8, 0, 255, 0.000, 999.000, "lx", {{0, NULL}}},
 {1, "Temperature", "TMP", 8, 8, 0, 255, -40.000, 80.000, "°C", {{0, NULL}}},
 {1, "Wind speed", "WND", 16, 8, 0, 255, 0.000, 70.000, "m/s", {{0, NULL}}},
 {3, "Identifier", "ID", 24, 4, 0, 255, 0.000, 70.000, NULL, {{0, NULL}}},
 {3, "Day / Night", "D/N", 29, 1, 0, 255, 0.000, 70.000, NULL, {{0, NULL}}},
 {3, "Rain Indication", "RAN", 30, 1, 0, 255, 0.000, 70.000, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 7, "A5-13-01", "Environmental Applications", &A5_13_01[0]) > 0) count++;
static DATAFIELD A5_13_02[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Sun – West", "SNW", 0, 8, 0, 255, 0.000, 150.000, "klx", {{0, NULL}}},
 {1, "Sun – South", "SNS", 8, 8, 0, 255, 0.000, 150.000, "klx", {{0, NULL}}},
 {1, "Sun – East", "SNE", 16, 8, 0, 255, 0.000, 150.000, "klx", {{0, NULL}}},
 {3, "Identifier", "ID", 24, 4, 0, 255, 0.000, 150.000, NULL, {{0, NULL}}},
 {3, "Hemisphere", "HEM", 29, 1, 0, 255, 0.000, 150.000, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 6, "A5-13-02", "Environmental Applications", &A5_13_02[0]) > 0) count++;
static DATAFIELD A5_13_03[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Day", "DY", 3, 5, 1, 31, 1.000, 31.000, "N/A", {{0, NULL}}},
 {1, "Month", "MTH", 12, 4, 1, 12, 1.000, 12.000, "N/A", {{0, NULL}}},
 {1, "Year", "YR", 17, 7, 0, 99, 2000.000, 2099.000, "N/A", {{0, NULL}}},
 {3, "Identifier", "ID", 24, 4, 0, 99, 2000.000, 2099.000, NULL, {{0, NULL}}},
 {3, "Source", "SRC", 31, 1, 0, 99, 2000.000, 2099.000, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 6, "A5-13-03", "Environmental Applications", &A5_13_03[0]) > 0) count++;
static DATAFIELD A5_13_04[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "Weekday", "WDY", 0, 3, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Hour", "HR", 3, 5, 0, 23, 0.000, 23.000, "N/A", {{0, NULL}}},
 {1, "Minute", "MIN", 10, 6, 0, 59, 0.000, 59.000, "N/A", {{0, NULL}}},
 {1, "Second", "SEC", 18, 6, 0, 59, 0.000, 59.000, "N/A", {{0, NULL}}},
 {3, "Identifier", "ID", 24, 4, 0, 59, 0.000, 59.000, NULL, {{0, NULL}}},
 {3, "Time Format", "TMF", 29, 1, 0, 59, 0.000, 59.000, NULL, {{0, NULL}}},
 {3, "AM/PM", "A/PM", 30, 1, 0, 59, 0.000, 59.000, NULL, {{0, NULL}}},
 {3, "Source", "SRC", 31, 1, 0, 59, 0.000, 59.000, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 9, "A5-13-04", "Environmental Applications", &A5_13_04[0]) > 0) count++;
static DATAFIELD A5_13_05[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Elevation", "ELV", 0, 8, 0, 180, -90.000, 90.000, "°", {{0, NULL}}},
 {1, "Azimut", "AZM", 15, 9, 0, 359, 0.000, 359.000, "°", {{0, NULL}}},
 {3, "Identifier", "ID", 24, 4, 0, 359, 0.000, 359.000, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 4, "A5-13-05", "Environmental Applications", &A5_13_05[0]) > 0) count++;
static DATAFIELD A5_13_06[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Latitude(MSB)", "LAT(MSB)", 0, 4, 0, 0, 0.000, 0.000, "LAT(LSB)", {{0, NULL}}},
 {1, "Longitude(MSB)", "LOT(MSB)", 4, 4, 0, 0, 0.000, 0.000, "LOT(LSB)", {{0, NULL}}},
 {1, "Latitude(LSB)", "LAT(LSB)", 8, 8, 0, 4095, -90.000, 90.000, "°", {{0, NULL}}},
 {1, "Longitude(LSB)", "LOT(LSB)", 16, 8, 0, 4095, -180.000, 180.000, "°", {{0, NULL}}},
 {3, "Identifier", "ID", 24, 4, 0, 4095, -180.000, 180.000, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 6, "A5-13-06", "Environmental Applications", &A5_13_06[0]) > 0) count++;
static DATAFIELD A5_13_07[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "Wind Direction", "WD", 4, 4, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Average Wind Speed", "AWS", 8, 8, 0, 255, 1.000, 199.900, "mph", {{0, NULL}}},
 {1, "Maximum Wind Speed", "MWS", 16, 8, 0, 255, 1.000, 199.900, "mph", {{0, NULL}}},
 {1, "Battery Status", "BS", 31, 1, 0, 255, 1.000, 199.900, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 5, "A5-13-07", "Environmental Applications", &A5_13_07[0]) > 0) count++;
static DATAFIELD A5_13_08[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "Rainfall Adjust Sign", "RAS", 1, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "Rainfall Adjust", "RFA", 2, 6, 0, 0, 0.000, 3.900, "%", {{0, NULL}}},
 {1, "Rainfall Count", "RFC", 8, 16, 0, 65535, 0.000, 65535.000, NULL, {{0, NULL}}},
 {3, "Battery Status", "BS", 31, 1, 0, 65535, 0.000, 65535.000, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 5, "A5-13-08", "Environmental Applications", &A5_13_08[0]) > 0) count++;
static DATAFIELD A5_13_10[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "Day / Night", "D/N", 7, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Sun Elevation", "SNE", 0, 7, 0, 90, 0.000, 90.000, "°", {{0, NULL}}},
 {1, "Sun Azimuth", "SNA", 8, 8, 0, 180, -90.000, 90.000, "°", {{0, NULL}}},
 {1, "Solar Radiation (MSB)", "SRA (MSB)", 16, 8, 0, 0, 0.000, 0.000, "SRA (LSB)", {{0, NULL}}},
 {1, "Solar Radiation (LSB)", "SRA (LSB)", 29, 3, 0, 2000, 0.000, 2000.000, "W/m2", {{0, NULL}}},
 {3, "Identifier", "ID", 24, 4, 0, 2000, 0.000, 2000.000, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 7, "A5-13-10", "Environmental Applications", &A5_13_10[0]) > 0) count++;
static DATAFIELD A5_14_01[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Supply voltage", "SVC", 0, 8, 0, 250, 0.000, 5.000, "V", {{0, NULL}}},
 {3, "Contact", "CT", 31, 1, 0, 250, 0.000, 5.000, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 3, "A5-14-01", "Multi-Func Sensor", &A5_14_01[0]) > 0) count++;
static DATAFIELD A5_14_02[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Supply voltage", "SVC", 0, 8, 0, 250, 0.000, 5.000, "V", {{0, NULL}}},
 {1, "Illumination", "ILL", 8, 8, 0, 250, 0.000, 1000.000, "lx", {{0, NULL}}},
 {3, "Contact", "CT", 31, 1, 0, 250, 0.000, 1000.000, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 4, "A5-14-02", "Multi-Func Sensor", &A5_14_02[0]) > 0) count++;
static DATAFIELD A5_14_03[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Supply voltage", "SVC", 0, 8, 0, 250, 0.000, 5.000, "V", {{0, NULL}}},
 {3, "Vibration", "VIB", 30, 1, 0, 250, 0.000, 5.000, NULL, {{0, NULL}}},
 {3, "Contact", "CT", 31, 1, 0, 250, 0.000, 5.000, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 4, "A5-14-03", "Multi-Func Sensor", &A5_14_03[0]) > 0) count++;
static DATAFIELD A5_14_04[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Supply voltage", "SVC", 0, 8, 0, 250, 0.000, 5.000, "V", {{0, NULL}}},
 {1, "Illumination", "ILL", 8, 8, 0, 250, 0.000, 1000.000, "lx", {{0, NULL}}},
 {3, "Vibration", "VIB", 30, 1, 0, 250, 0.000, 1000.000, NULL, {{0, NULL}}},
 {3, "Contact", "CT", 31, 1, 0, 250, 0.000, 1000.000, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 5, "A5-14-04", "Multi-Func Sensor", &A5_14_04[0]) > 0) count++;
static DATAFIELD A5_14_05[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Supply voltage", "SVC", 0, 8, 0, 250, 0.000, 5.000, "V", {{0, NULL}}},
 {3, "Vibration", "VIB", 30, 1, 0, 250, 0.000, 5.000, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 3, "A5-14-05", "Multi-Func Sensor", &A5_14_05[0]) > 0) count++;
static DATAFIELD A5_14_06[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Supply voltage", "SVC", 0, 8, 0, 250, 0.000, 5.000, "V", {{0, NULL}}},
 {1, "Illumination", "ILL", 8, 8, 0, 250, 0.000, 1000.000, "lx", {{0, NULL}}},
 {3, "Vibration", "VIB", 30, 1, 0, 250, 0.000, 1000.000, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 4, "A5-14-06", "Multi-Func Sensor", &A5_14_06[0]) > 0) count++;
static DATAFIELD A5_14_07[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Supply voltage", "SVC", 0, 8, 0, 250, 0.000, 5.000, "V", {{0, NULL}}},
 {3, "Door Contact", "DCT", 29, 1, 0, 250, 0.000, 5.000, NULL, {{0, NULL}}},
 {3, "Lock Contact", "LCT", 30, 1, 0, 250, 0.000, 5.000, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 4, "A5-14-07", "Multi-Func Sensor", &A5_14_07[0]) > 0) count++;
static DATAFIELD A5_14_08[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Supply voltage", "SVC", 0, 8, 0, 250, 0.000, 5.000, "V", {{0, NULL}}},
 {3, "Door Contact", "DCT", 29, 1, 0, 250, 0.000, 5.000, NULL, {{0, NULL}}},
 {3, "Lock Contact", "LCT", 30, 1, 0, 250, 0.000, 5.000, NULL, {{0, NULL}}},
 {3, "Vibration", "VIB", 31, 1, 0, 250, 0.000, 5.000, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 5, "A5-14-08", "Multi-Func Sensor", &A5_14_08[0]) > 0) count++;
static DATAFIELD A5_14_09[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Supply voltage", "SVC", 0, 8, 0, 250, 0.000, 5.000, "V", {{0, NULL}}},
 {3, "Contact", "CT", 29, 2, 0, 250, 0.000, 5.000, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 3, "A5-14-09", "Multi-Func Sensor", &A5_14_09[0]) > 0) count++;
static DATAFIELD A5_14_0A[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Supply voltage", "SVC", 0, 8, 0, 250, 0.000, 5.000, "V", {{0, NULL}}},
 {3, "Contact", "CT", 29, 2, 0, 250, 0.000, 5.000, NULL, {{0, NULL}}},
 {3, "Vibration", "VIB", 31, 1, 0, 250, 0.000, 5.000, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 4, "A5-14-0A", "Multi-Func Sensor", &A5_14_0A[0]) > 0) count++;
static DATAFIELD A5_20_01[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Current Value", "CV", 0, 8, 0, 100, 0.000, 100.000, "%", {{0, NULL}}},
 {3, "Service On", "SO", 8, 1, 0, 100, 0.000, 100.000, NULL, {{0, NULL}}},
 {3, "Energy input enabled", "ENIE", 9, 1, 0, 100, 0.000, 100.000, NULL, {{0, NULL}}},
 {3, "Energy Storage", "ES", 10, 1, 0, 100, 0.000, 100.000, NULL, {{0, NULL}}},
 {3, "Battery capacity ", "BCAP", 11, 1, 0, 100, 0.000, 100.000, NULL, {{0, NULL}}},
 {3, "Contact, cover open", "CCO", 12, 1, 0, 100, 0.000, 100.000, NULL, {{0, NULL}}},
 {3, "Failure temperature sensor, out off range", "FTS", 13, 1, 0, 100, 0.000, 100.000, NULL, {{0, NULL}}},
 {3, "Detection, window open", "DWO", 14, 1, 0, 100, 0.000, 100.000, NULL, {{0, NULL}}},
 {3, "Actuator obstructed", "ACO", 15, 1, 0, 100, 0.000, 100.000, NULL, {{0, NULL}}},
 {1, "Temperature", "TMP", 16, 8, 0, 255, 0.000, 40.000, "°C", {{0, NULL}}},
 {3, "LRN Bit", "LRNB", 28, 1, 0, 255, 0.000, 40.000, NULL, {{0, NULL}}},
 {1, "Valve position or Temperature Setpoint", "SP", 0, 8, 0, 100, 0.000, 100.000, "% or °C", {{0, NULL}}},
 {1, "Temperature  from RCU", "TMP", 8, 8, 255, 0, 0.000, 40.000, "°C", {{0, NULL}}},
 {3, "Run init sequence", "RIN", 16, 1, 255, 0, 0.000, 40.000, NULL, {{0, NULL}}},
 {3, "Lift set", "LFS", 17, 1, 255, 0, 0.000, 40.000, NULL, {{0, NULL}}},
 {3, "Valve open / maintenance", "VO", 18, 1, 255, 0, 0.000, 40.000, NULL, {{0, NULL}}},
 {3, "Valve closed", "VC", 19, 1, 255, 0, 0.000, 40.000, NULL, {{0, NULL}}},
 {3, "Summer bit, Reduction of energy consumption", "SB", 20, 1, 255, 0, 0.000, 40.000, NULL, {{0, NULL}}},
 {3, "Set Point Selection", "SPS", 21, 1, 255, 0, 0.000, 40.000, NULL, {{0, NULL}}},
 {3, "Set point inverse", "SPN", 22, 1, 255, 0, 0.000, 40.000, NULL, {{0, NULL}}},
 {3, "Select function", "RCU", 23, 1, 255, 0, 0.000, 40.000, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 22, "A5-20-01", "HVAC Components", &A5_20_01[0]) > 0) count++;
static DATAFIELD A5_20_02[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Actual Value", "AV", 0, 8, 0, 100, 0.000, 100.000, "%", {{0, NULL}}},
 {3, " Set point inverse", "SPI", 22, 1, 0, 100, 0.000, 100.000, NULL, {{0, NULL}}},
 {3, "LRN Bit", "LRNB", 28, 1, 0, 100, 0.000, 100.000, NULL, {{0, NULL}}},
 {1, "Valve Set point", "VSP", 0, 8, 0, 100, 0.000, 100.000, "%", {{0, NULL}}},
 {3, "Set point inverse", "VSP", 22, 1, 0, 100, 0.000, 100.000, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 6, "A5-20-02", "HVAC Components", &A5_20_02[0]) > 0) count++;
static DATAFIELD A5_20_03[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Actual valve", "AV", 0, 8, 0, 100, 0.000, 100.000, "%", {{0, NULL}}},
 {1, "Temperature", "TMP", 16, 8, 0, 255, 0.000, 40.000, "°C", {{0, NULL}}},
 {3, "Set Point Inverse", "(null)", 22, 1, 0, 255, 0.000, 40.000, NULL, {{0, NULL}}},
 {3, "Set Point Selection", "SPS", 21, 1, 0, 255, 0.000, 40.000, NULL, {{0, NULL}}},
 {3, "LRN Bit", "LRNB", 28, 1, 0, 255, 0.000, 40.000, NULL, {{0, NULL}}},
 {1, "Actuator or Temperature Setpoint", "ATS", 0, 8, 0, 100, 0.000, 100.000, "% or °C", {{0, NULL}}},
 {1, "Temperature  from RCU", "TMPRC", 8, 8, 255, 0, 0.000, 40.000, "°C", {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 8, "A5-20-03", "HVAC Components", &A5_20_03[0]) > 0) count++;
static DATAFIELD A5_20_04[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Current Position", "CP", 0, 8, 0, 100, 0.000, 100.000, "%", {{0, NULL}}},
 {1, "Feed Temperature OR Temperature Set Point", "FTS", 8, 8, 0, 255, 20.000, 10.000, "°C", {{0, NULL}}},
 {3, "Room Temperature OR Failure Code", "TMPFC", 16, 8, 0, 255, 10.000, 30.000, "°C", {{0, NULL}}},
 {1, "Measurement Status", "MST", 24, 1, 0, 255, 10.000, 30.000, NULL, {{0, NULL}}},
 {1, "Status Request", "STR", 25, 1, 0, 255, 10.000, 30.000, NULL, {{0, NULL}}},
 {1, "Button Lock Status", "BLS", 29, 1, 0, 255, 10.000, 30.000, NULL, {{0, NULL}}},
 {1, "Temperature Selection", "TS", 30, 1, 0, 255, 10.000, 30.000, NULL, {{0, NULL}}},
 {1, "Failure", "FL", 31, 1, 0, 255, 10.000, 30.000, NULL, {{0, NULL}}},
 {1, "Valve Position", "POS", 0, 8, 0, 100, 0.000, 100.000, "%", {{0, NULL}}},
 {1, "Temperature Set Point", "TSP", 8, 8, 0, 255, 10.000, 30.000, "°C", {{0, NULL}}},
 {1, "LRN Bit", "LRNB", 28, 1, 0, 255, 10.000, 30.000, NULL, {{0, NULL}}},
 {1, "Measurement Control", "MC", 17, 1, 0, 255, 10.000, 30.000, NULL, {{0, NULL}}},
 {1, "Wake-up Cycle", "WUC", 18, 6, 0, 255, 10.000, 30.000, NULL, {{0, NULL}}},
 {1, "Display Orientation", "DSO", 26, 2, 0, 255, 10.000, 30.000, NULL, {{0, NULL}}},
 {1, "Button Lock Control", "BLC", 29, 1, 0, 255, 10.000, 30.000, NULL, {{0, NULL}}},
 {1, "Service Command", "SER", 30, 2, 0, 255, 10.000, 30.000, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 17, "A5-20-04", "HVAC Components", &A5_20_04[0]) > 0) count++;
static DATAFIELD A5_20_05[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "Actual speed setting", "ASP", 0, 3, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "Actual speed timer setting", "ASPT", 3, 3, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "Node low battery", "E-NBAT", 11, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "Node comm. error", "E-NCOM", 12, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "Sensor error", "E-SENS", 13, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "Fan speed error", "E-FSPD", 14, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "Error", "ERR", 15, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "Filter condition", "FLTR", 16, 8, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "Auto speed supported", "AUTO", 24, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "By-pass active", "BPASS", 25, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "Frost protection", "FROST", 26, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "New speed setting", "NSP", 0, 3, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "New speed timer setting", "NSPT", 3, 5, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "Reset error", "RSTE", 24, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "Reset filter timer", "RSTF", 25, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 17, "A5-20-05", "HVAC Components", &A5_20_05[0]) > 0) count++;
static DATAFIELD A5_20_06[] = {
 {1, "Current Value", "CV", 0, 8, 0, 100, 0.000, 100.000, "%", {{0, NULL}}},
 {3, "Local Offset Mode", "LOM", 8, 1, 0, 100, 0.000, 100.000, NULL, {{0, NULL}}},
 {3, "Local Offset", "LO", 9, 7, 0, 100, 0.000, 40.000, "°C", {{0, NULL}}},
 {3, "Temperature", "TMP", 16, 8, 0, 100, 0.000, 80.000, "°C", {{0, NULL}}},
 {3, "Tempertature Selection", "TSL", 24, 1, 0, 100, 0.000, 80.000, NULL, {{0, NULL}}},
 {3, "Energy Input Enabled ", "ENIE", 25, 1, 0, 100, 0.000, 80.000, NULL, {{0, NULL}}},
 {3, "Energy Storage ", "ES", 26, 1, 0, 100, 0.000, 80.000, NULL, {{0, NULL}}},
 {3, "Window open detection", "DWO", 27, 1, 0, 100, 0.000, 80.000, NULL, {{0, NULL}}},
 {3, "LRN Bit", "LRNB", 28, 1, 0, 100, 0.000, 80.000, NULL, {{0, NULL}}},
 {3, "Radio Com Error", "RCE", 29, 1, 0, 100, 0.000, 80.000, NULL, {{0, NULL}}},
 {3, "Radio Signal strength", "RSS", 30, 1, 0, 100, 0.000, 80.000, NULL, {{0, NULL}}},
 {3, "Actuator obstructed", "ACO", 31, 1, 0, 100, 0.000, 80.000, NULL, {{0, NULL}}},
 {3, "Setpoint", "SP", 0, 8, 0, 100, 0.000, 40.000, "°C", {{0, NULL}}},
 {1, "Room temperature from contrul unit ", "TMP", 8, 8, 0, 160, 0.000, 40.000, "%", {{0, NULL}}},
 {3, "Reference run", "REF", 16, 1, 0, 160, 0.000, 40.000, NULL, {{0, NULL}}},
 {3, "RF Communication intervall", "RFC", 17, 4, 0, 160, 0.000, 40.000, NULL, {{0, NULL}}},
 {3, "Initiate summer mode Bit", "SB", 20, 1, 0, 160, 0.000, 40.000, NULL, {{0, NULL}}},
 {3, "Set point selection", "SPS", 21, 1, 0, 160, 0.000, 40.000, NULL, {{0, NULL}}},
 {3, "Temperature Selection", "TSL", 22, 1, 0, 160, 0.000, 40.000, NULL, {{0, NULL}}},
 {3, "Standbye", "SBY", 23, 1, 0, 160, 0.000, 40.000, NULL, {{0, NULL}}},
 {3, "LRN Bit", "LRNB", 28, 1, 0, 160, 0.000, 40.000, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 21, "A5-20-06", "HVAC Components", &A5_20_06[0]) > 0) count++;
static DATAFIELD A5_20_10[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "Mode", "MD", 0, 8, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Vane position", "VPS", 8, 4, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Fan Speed", "FANSP", 12, 4, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Control variable", "CVAR", 16, 8, 0, 100, 0.000, 100.000, "%", {{0, NULL}}},
 {1, "Room occupancy", "RO", 29, 2, 0, 100, 0.000, 100.000, NULL, {{0, NULL}}},
 {1, "On/Off", "O/I", 31, 1, 0, 100, 0.000, 100.000, NULL, {{0, NULL}}},
 {1, "LRN Bit", "LRNB", 28, 1, 0, 100, 0.000, 100.000, NULL, {{0, NULL}}},
 {1, "Mode", "MD", 0, 8, 0, 100, 0.000, 100.000, NULL, {{0, NULL}}},
 {1, "Vane position", "VPS", 8, 4, 0, 100, 0.000, 100.000, NULL, {{0, NULL}}},
 {1, "Fan Speed", "FANSP", 12, 4, 0, 100, 0.000, 100.000, NULL, {{0, NULL}}},
 {1, "Control variable", "CVAR", 16, 8, 0, 100, 0.000, 100.000, "%", {{0, NULL}}},
 {1, "Room occupancy", "RO", 29, 2, 0, 100, 0.000, 100.000, NULL, {{0, NULL}}},
 {1, "On/Off", "O/I", 31, 1, 0, 100, 0.000, 100.000, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 14, "A5-20-10", "HVAC Components", &A5_20_10[0]) > 0) count++;
static DATAFIELD A5_20_11[] = {
 {3, "Disable remote controller", "DRC", 29, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "External disablement", "EXDS", 23, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "Window contact", "WC", 30, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "Remote controller Disablement", "RCD", 29, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Error Code", "ERR", 0, 16, 0, 65535, 0.000, 65535.000, "N/A", {{0, NULL}}},
 {3, "Reserved", "RES", 16, 4, 0, 65535, 0.000, 65535.000, NULL, {{0, NULL}}},
 {3, "Other disablement", "OD", 20, 1, 0, 65535, 0.000, 65535.000, NULL, {{0, NULL}}},
 {3, "Window contact disablement", "WCD", 21, 1, 0, 65535, 0.000, 65535.000, NULL, {{0, NULL}}},
 {3, "Key card disablement", "KCD", 22, 1, 0, 65535, 0.000, 65535.000, NULL, {{0, NULL}}},
 {3, "External disablement", "ED", 23, 1, 0, 65535, 0.000, 65535.000, NULL, {{0, NULL}}},
 {3, "Window contact", "WC", 30, 1, 0, 65535, 0.000, 65535.000, NULL, {{0, NULL}}},
 {3, "Alarm State", "AS", 31, 1, 0, 65535, 0.000, 65535.000, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 14, "A5-20-11", "HVAC Components", &A5_20_11[0]) > 0) count++;
static DATAFIELD A5_20_12[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Control Variable override", "CV", 0, 8, 0, 255, 0.000, 100.000, "%", {{0, NULL}}},
 {3, "FanStage override", "FANOR", 8, 8, 0, 255, 0.000, 100.000, NULL, {{0, NULL}}},
 {1, "Setpoint shift", "SPS", 16, 8, 0, 255, -10.000, 10.000, "°K", {{0, NULL}}},
 {3, "Fan override", "FANOR", 24, 1, 0, 255, -10.000, 10.000, NULL, {{0, NULL}}},
 {3, "Controller mode", "CTM", 25, 2, 0, 255, -10.000, 10.000, NULL, {{0, NULL}}},
 {3, "Controller state", "CST", 27, 1, 0, 255, -10.000, 10.000, NULL, {{0, NULL}}},
 {3, "Energy hold-off / Dew point", "ERH", 29, 1, 0, 255, -10.000, 10.000, NULL, {{0, NULL}}},
 {3, "Room occupancy", "RO", 30, 2, 0, 255, -10.000, 10.000, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 9, "A5-20-12", "HVAC Components", &A5_20_12[0]) > 0) count++;
static DATAFIELD A5_30_01[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "Supply voltage", "SVC", 8, 8, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "Input State", "IPS", 16, 8, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 3, "A5-30-01", "Digital Input", &A5_30_01[0]) > 0) count++;
static DATAFIELD A5_30_02[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "Input State", "IPS", 31, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 2, "A5-30-02", "Digital Input", &A5_30_02[0]) > 0) count++;
static DATAFIELD A5_30_03[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Temperature", "TMP", 8, 8, 255, 0, 0.000, 40.000, "°C", {{0, NULL}}},
 {3, "Status of Wake", "WA0", 19, 1, 255, 0, 0.000, 40.000, NULL, {{0, NULL}}},
 {3, "Digital Input 3", "DI3", 20, 1, 255, 0, 0.000, 40.000, NULL, {{0, NULL}}},
 {3, "Digital Input 2", "DI2", 21, 1, 255, 0, 0.000, 40.000, NULL, {{0, NULL}}},
 {3, "Digital Input 1", "DI1", 22, 1, 255, 0, 0.000, 40.000, NULL, {{0, NULL}}},
 {3, "Digital Input 0", "DI0", 23, 1, 255, 0, 0.000, 40.000, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 7, "A5-30-03", "Digital Input", &A5_30_03[0]) > 0) count++;
static DATAFIELD A5_30_04[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Digital value-input", "DV0", 16, 8, 0, 255, 0.000, 255.000, "N/A", {{0, NULL}}},
 {3, "Digital Input 2", "DI2", 29, 1, 0, 255, 0.000, 255.000, NULL, {{0, NULL}}},
 {3, "Digital Input 1", "DI1", 30, 1, 0, 255, 0.000, 255.000, NULL, {{0, NULL}}},
 {3, "Digital Input 0", "DI0", 31, 1, 0, 255, 0.000, 255.000, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 5, "A5-30-04", "Digital Input", &A5_30_04[0]) > 0) count++;
static DATAFIELD A5_30_05[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Supply voltage", "VDD", 8, 8, 0, 255, 0.000, 3.300, "V", {{0, NULL}}},
 {3, "Signal type", "ST", 16, 1, 0, 255, 0.000, 3.300, NULL, {{0, NULL}}},
 {3, "Index of Signals", "IOS", 17, 7, 0, 255, 0.000, 3.300, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 4, "A5-30-05", "Digital Input", &A5_30_05[0]) > 0) count++;
static DATAFIELD A5_37_01[] = {
 {1, "DR Level", "DRL", 24, 4, 0, 15, 0.000, 15.000, "N/A", {{0, NULL}}},
 {3, "LRN Bit", "LRNB", 28, 1, 0, 15, 0.000, 15.000, NULL, {{0, NULL}}},
 {1, "Temporary default ", "TMPD", 0, 8, 0, 255, 0.000, 255.000, "N/A", {{0, NULL}}},
 {3, "Absolute/relative power usage", "SPWRU", 8, 1, 0, 255, 0.000, 255.000, NULL, {{0, NULL}}},
 {1, "Power Usage", "PWRU", 9, 7, 0, 100, 0.000, 100.000, "N/A", {{0, NULL}}},
 {1, "Timeout Setting", "TMOS", 16, 8, 1, 255, 15.000, 3825.000, "min", {{0, NULL}}},
 {3, "Random start delay", "RSD", 29, 1, 1, 255, 15.000, 3825.000, NULL, {{0, NULL}}},
 {3, "Randomized end delay", "RED", 30, 1, 1, 255, 15.000, 3825.000, NULL, {{0, NULL}}},
 {3, "Max/Min Power Usage for Default DR State", "MPWRU", 31, 1, 1, 255, 15.000, 3825.000, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 9, "A5-37-01", "Energy Management", &A5_37_01[0]) > 0) count++;
static DATAFIELD A5_38_08[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "Command", "COM", 0, 8, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Time", "TIM", 8, 16, 1, 65535, 0.100, 6553.500, "s", {{0, NULL}}},
 {3, "Lock/Unlock", "LCK", 29, 1, 1, 65535, 0.100, 6553.500, NULL, {{0, NULL}}},
 {3, "Delay or duration", "DEL", 30, 1, 1, 65535, 0.100, 6553.500, NULL, {{0, NULL}}},
 {3, "Switching Command", "SW", 31, 1, 1, 65535, 0.100, 6553.500, NULL, {{0, NULL}}},
 {3, "LRN Bit", "LRNB", 28, 1, 1, 65535, 0.100, 6553.500, NULL, {{0, NULL}}},
 {3, "Command", "COM", 0, 8, 1, 65535, 0.100, 6553.500, NULL, {{0, NULL}}},
 {1, "Dimming value", "EDIM", 8, 8, 0, 255, 0.000, 100.000, "%", {{0, NULL}}},
 {1, "Ramping time", "RMP", 16, 8, 0, 255, 0.000, 255.000, "s", {{0, NULL}}},
 {3, "Dimming Range", "EDIM R", 29, 1, 0, 255, 0.000, 255.000, NULL, {{0, NULL}}},
 {3, "Store final value", "STR", 30, 1, 0, 255, 0.000, 255.000, NULL, {{0, NULL}}},
 {3, "Switching Command", "SW", 31, 1, 0, 255, 0.000, 255.000, NULL, {{0, NULL}}},
 {3, "LRN Bit", "LRNB", 28, 1, 0, 255, 0.000, 255.000, NULL, {{0, NULL}}},
 {3, "Command", "COM", 0, 8, 0, 255, 0.000, 255.000, NULL, {{0, NULL}}},
 {1, "Setpoint", "SP", 16, 8, 0, 255, -12.700, 12.800, "K", {{0, NULL}}},
 {3, "LRN Bit", "LRNB", 28, 1, 0, 255, -12.700, 12.800, NULL, {{0, NULL}}},
 {3, "Command", "COM", 0, 8, 0, 255, -12.700, 12.800, NULL, {{0, NULL}}},
 {1, "Basic Setpoint", "BSP", 16, 8, 0, 255, 0.000, 51.200, "°C", {{0, NULL}}},
 {3, "LRN Bit", "LRNB", 28, 1, 0, 255, 0.000, 51.200, NULL, {{0, NULL}}},
 {3, "Command", "COM", 0, 8, 0, 255, 0.000, 51.200, NULL, {{0, NULL}}},
 {1, "Control variable override", "CVOV", 16, 8, 0, 255, 0.000, 100.000, "%", {{0, NULL}}},
 {3, "Controller mode", "CM", 25, 2, 0, 255, 0.000, 100.000, NULL, {{0, NULL}}},
 {3, "Controller state", "CS", 27, 1, 0, 255, 0.000, 100.000, NULL, {{0, NULL}}},
 {3, "Energy hold off", "ENHO", 29, 1, 0, 255, 0.000, 100.000, NULL, {{0, NULL}}},
 {3, "Room occupancy", "RMOCC", 30, 2, 0, 255, 0.000, 100.000, NULL, {{0, NULL}}},
 {3, "LRN Bit", "LRNB", 28, 1, 0, 255, 0.000, 100.000, NULL, {{0, NULL}}},
 {3, "Command", "COM", 0, 8, 0, 255, 0.000, 100.000, NULL, {{0, NULL}}},
 {3, "FanStage override", "FO", 16, 8, 0, 255, 0.000, 100.000, NULL, {{0, NULL}}},
 {3, "LRN Bit", "LRNB", 28, 1, 0, 255, 0.000, 100.000, NULL, {{0, NULL}}},
 {3, "Command", "COM", 0, 8, 0, 255, 0.000, 100.000, NULL, {{0, NULL}}},
 {3, "Parameter 1", "P1", 8, 8, 0, 255, 0.000, 100.000, NULL, {{0, NULL}}},
 {3, "Parameter 2", "P2", 16, 8, 0, 255, 0.000, 100.000, NULL, {{0, NULL}}},
 {3, "Function", "FUNC", 24, 4, 0, 255, 0.000, 100.000, NULL, {{0, NULL}}},
 {3, "Send status flag", "SSF", 29, 1, 0, 255, 0.000, 100.000, NULL, {{0, NULL}}},
 {3, "Pos. and Angle flag", "PAF", 30, 1, 0, 255, 0.000, 100.000, NULL, {{0, NULL}}},
 {3, "Service Mode Flag", "SMF", 31, 1, 0, 255, 0.000, 100.000, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 37, "A5-38-08", "Central Command", &A5_38_08[0]) > 0) count++;
static DATAFIELD A5_38_09[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "Parameter 1", "P1", 0, 8, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "Parameter 2", "P2", 8, 8, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "Parameter 3", "P3", 16, 8, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "Function", "FUNC", 24, 4, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "Send status flag", "SSF", 29, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "Store final value", "SFV", 30, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "Service Mode Flag", "SMF", 31, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 8, "A5-38-09", "Central Command", &A5_38_09[0]) > 0) count++;
static DATAFIELD A5_3F_00[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Not used", "(null)", 0, 28, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "MSG_ID", "MSGID", 29, 2, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "MSG-Source", "MSGS", 31, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Sub-Telegram Counter", "STCNT", 0, 2, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "RSSI Level in dBm", "RSLV", 2, 6, 0, 0, 0.000, 0.000, "dBm", {{0, NULL}}},
 {1, "Sub-Telegram Counter/RSSI Level in dBm", "RSLV", 8, 8, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Sub-Telegram Counter/RSSI Level in dBm", "RSLV", 16, 8, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "RSSI Level in dBm", "RSLV", 24, 4, 0, 0, 0.000, 0.000, "dBm", {{0, NULL}}},
 {1, "MSG_ID", "MSGID", 29, 2, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "MSG-Source", "MSGS", 31, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "LRN Bit", "LRNB", 4, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "RLT MSG-Counter MSB", "MC-MSB", 0, 4, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "RLT MSG-Counter LSB", "MC-LSB", 5, 2, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "MSG-Source", "MSGS", 7, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "LRN Bit", "LRNB", 4, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "RLT MSG-Counter MSB", "MC-MSB", 0, 4, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "RLT MSG-Counter LSB", "MC-LSB", 5, 2, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "MSG-Source", "MSGS", 7, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 20, "A5-3F-00", "Universal", &A5_3F_00[0]) > 0) count++;
static DATAFIELD A5_3F_7F[] = {
 {3, "LRN Bit", "LRNB", 28, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "undefined", "undef", 0, 28, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "undefined", "undef", 29, 3, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 3, "A5-3F-7F", "Universal", &A5_3F_7F[0]) > 0) count++;
static DATAFIELD D2_00_01[] = {
 {3, "MsgId", "MI", 5, 3, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "User Action", "KP", 11, 5, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "ConfigValid", "CV", 8, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "MsgId", "MI", 5, 3, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "MoreData", "MD", 4, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "Fan", "F", 1, 3, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "Fan manual", "M", 0, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "Figure A Type", "TA", 11, 5, 0, 0, 0.000, 0.000, "% rH", {{0, NULL}}},
 {1, "Presence", "PR", 8, 3, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Figure A Value", "ZA", 16, 16, 0, 0, 0.000, 0.000, "ppm", {{0, NULL}}},
 {1, "Heating", "Sa", 39, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Cooling", "Sb", 38, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Dew-Point", "Sc", 37, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Window", "Sd", 36, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, " Data:User Notification", " Data:User Notification", 0, 0, 0, 0, 0.000, 0.000, " Data:User Notification", {{0, NULL}}},
 {0, "                {Se} 35 1 0 0 0.000 0.000 [(null)]", "Se", 35, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "MsgId", "MI", 5, 3, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Fan", "F", 1, 3, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Set Point A Type", "TA", 11, 5, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Presence", "PR", 8, 3, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Set Point A Value", "ZA", 16, 16, -1270, 1270, -12.700, 12.700, "°", {{0, NULL}}},
 {1, "MsgId", "MI", 5, 3, -1270, 1270, -12.700, 12.700, NULL, {{0, NULL}}},
 {1, "Channel A Value", "VA (LSB)", 8, 8, 0, 4000, 0.000, 40.000, "°", {{0, NULL}}},
 {1, "Channel A Value", "VA (MSB)", 20, 4, 0, 4000, 0.000, 40.000, NULL, {{0, NULL}}},
 {1, "Channel A Type", "TA", 16, 4, 0, 4000, 0.000, 40.000, NULL, {{0, NULL}}},
 {1, "MsgId", "MI", 5, 3, 0, 4000, 0.000, 40.000, NULL, {{0, NULL}}},
 {1, "MoreData", "MD", 4, 1, 0, 4000, 0.000, 40.000, NULL, {{0, NULL}}},
 {1, " Data:", " Data:", 0, 0, 0, 0, 0.000, 0.000, " Data:", {{0, NULL}}},
 {0, "                  Set Point Range Limit", "                  Set Point Range Limit", 0, 0, 0, 0, 0.000, 0.000, "                  Set Point Range Limit", {{0, NULL}}},
 {0, "                {SPR} 9 7 0 4000 0.100 12.700 [°]", "SPR", 9, 7, 0, 4000, 0.100, 12.700, "°", {{0, NULL}}},
 {1, "Set PointSteps ", "SPS", 17, 7, 0, 4000, 1.000, 127.000, NULL, {{0, NULL}}},
 {1, " Data:", " Data:", 0, 0, 0, 0, 0.000, 0.000, " Data:", {{0, NULL}}},
 {0, "                  Temperature Measurement Timing", "                  Temperature Measurement Timing", 0, 0, 0, 0, 0.000, 0.000, "                  Temperature Measurement Timing", {{0, NULL}}},
 {0, "                {TT (LSB)} 24 4 0 4000 10.000 600.000 [s]", "TT (LSB)", 24, 4, 0, 4000, 10.000, 600.000, "s", {{0, NULL}}},
 {1, " Data:", " Data:", 0, 0, 0, 0, 0.000, 0.000, " Data:", {{0, NULL}}},
 {0, "                  Temperature Measurement Timing", "                  Temperature Measurement Timing", 0, 0, 0, 0, 0.000, 0.000, "                  Temperature Measurement Timing", {{0, NULL}}},
 {0, "                {TT (MSB)} 38 2 0 4000 10.000 600.000 [(null)]", "TT (MSB)", 38, 2, 0, 4000, 10.000, 600.000, NULL, {{0, NULL}}},
 {1, "Fan", "F", 35, 3, 0, 4000, 1.000, 7.000, NULL, {{0, NULL}}},
 {1, "Presence", "PR", 32, 3, 0, 4000, 1.000, 7.000, NULL, {{0, NULL}}},
 {1, "Keep Alive Timing", "KA", 45, 3, 0, 4000, 10.000, 70.000, NULL, {{0, NULL}}},
 {1, " Significant Temperature Difference ", "ST", 40, 4, 0, 0, 0.000, 3.000, "°", {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 41, "D2-00-01", "Room Control Panel (RCP)", &D2_00_01[0]) > 0) count++;
static DATAFIELD D2_01_00[] = {
 {3, "Command ID", "CMD", 4, 4, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "Dim value", "DV", 8, 3, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "I/O channel", "I/O", 11, 5, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "Output value", "OV", 17, 7, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "Taught-in devices", "d/e", 0, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "Command ID", "CMD", 4, 4, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "Over current shut down", "OC", 8, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "reset over current shut down", "RO", 9, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "Local control", "LC", 10, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "I/O channel", "I/O", 11, 5, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "Dim timer 3", "DT3", 20, 4, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "Dim timer 1", "DT1", 28, 4, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "Dim timer 2", "DT2", 16, 4, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "User interface indication", "d/n", 24, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "Power Failure", "PF", 25, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "Default state", "DS", 26, 2, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "Command ID", "CMD", 4, 4, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "I/O channel", "I/O", 11, 5, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "Power Failure", "PF", 0, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "Power Failure Detection", "PFD", 1, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "Command ID", "CMD", 4, 4, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "Over current switch off", "OC", 8, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "Error level", "EL", 9, 2, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "I/O channel", "I/O", 11, 5, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "Local control", "LC", 16, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "Output value", "OV", 17, 7, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "Command ID", "CMD", 4, 4, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "Report measurement", "RM", 8, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "Reset measurement", "RE", 9, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "Measurement mode", "e/p", 10, 1, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {3, "I/O channel", "I/O", 11, 5, 0, 0, 0.000, 0.000, NULL, {{0, NULL}}},
 {1, "Measurement delta to be reported (LSB)", "MD_LSB", 16, 4, 0, 4095, 0.000, 4095.000, "N/A", {{0, NULL}}},
 {1, "Measurement delta to be reported (MSB)", "MD_MSB", 24, 8, 0, 4095, 0.000, 4095.000, "N/A", {{0, NULL}}},
 {3, "Unit", "UN", 21, 3, 0, 4095, 0.000, 4095.000, NULL, {{0, NULL}}},
 {3, "Maximum time between two subsequent actuator messages", "MAT", 32, 8, 0, 4095, 10.000, 2550.000, "s", {{0, NULL}}},
 {3, "Minimum time between two subsequent actuator messages", "MIT", 40, 8, 0, 4095, 1.000, 255.000, "s", {{0, NULL}}},
 {3, "Command ID", "CMD", 4, 4, 0, 4095, 1.000, 255.000, NULL, {{0, NULL}}},
 {3, "Query", "qu", 10, 1, 0, 4095, 1.000, 255.000, NULL, {{0, NULL}}},
 {3, "I/O channel", "I/O", 11, 5, 0, 4095, 1.000, 255.000, NULL, {{0, NULL}}},
 {3, "Command ID", "CMD", 4, 4, 0, 4095, 1.000, 255.000, NULL, {{0, NULL}}},
 {3, "Unit", "UN", 8, 3, 0, 4095, 1.000, 255.000, NULL, {{0, NULL}}},
 {3, "I/O channel", "I/O", 11, 5, 0, 4095, 1.000, 255.000, NULL, {{0, NULL}}},
 {1, "Measurement value (4 bytes)", "MV", 16, 32, 0, 2147483647, 1.000, 255.000, "N/A", {{0, NULL}}},
 {3, "Command ID", "CMD", 4, 4, 0, 2147483647, 1.000, 255.000, NULL, {{0, NULL}}},
 {3, "Pilotwire mode", "PM", 13, 3, 0, 2147483647, 1.000, 255.000, NULL, {{0, NULL}}},
 {3, "Command ID", "CMD", 4, 4, 0, 2147483647, 1.000, 255.000, NULL, {{0, NULL}}},
 {3, "Command ID", "CMD", 4, 4, 0, 2147483647, 1.000, 255.000, NULL, {{0, NULL}}},
 {3, "Pilotwire mode", "PM", 13, 3, 0, 2147483647, 1.000, 255.000, NULL, {{0, NULL}}},
 {3, "Command ID", "CMD", 4, 4, 0, 2147483647, 1.000, 255.000, NULL, {{0, NULL}}},
 {3, "I/O channel", "I/O", 11, 5, 0, 2147483647, 1.000, 255.000, NULL, {{0, NULL}}},
 {3, "Auto OFF Timer", "AOT", 16, 16, 0, 2147483647, 0.100, 6553.400, "s", {{0, NULL}}},
 {3, "Delay OFF Timer", "DOT", 32, 16, 0, 2147483647, 0.100, 6553.400, "s", {{0, NULL}}},
 {3, "External Switch/Push Button", "EBM", 48, 2, 0, 2147483647, 0.100, 6553.400, NULL, {{0, NULL}}},
 {3, "2-state switch", "SWT", 50, 1, 0, 2147483647, 0.100, 6553.400, NULL, {{0, NULL}}},
 {3, "Command ID", "CMD", 4, 4, 0, 2147483647, 0.100, 6553.400, NULL, {{0, NULL}}},
 {3, "I/O channel", "I/O", 11, 5, 0, 2147483647, 0.100, 6553.400, NULL, {{0, NULL}}},
 {3, "Command ID", "CMD", 4, 4, 0, 2147483647, 0.100, 6553.400, NULL, {{0, NULL}}},
 {3, "I/O channel", "I/O", 11, 5, 0, 2147483647, 0.100, 6553.400, NULL, {{0, NULL}}},
 {3, "Auto OFF Timer", "AOT", 16, 16, 0, 2147483647, 0.100, 6553.400, "s", {{0, NULL}}},
 {3, "Delay OFF Timer", "DOT", 32, 16, 0, 2147483647, 0.100, 6553.400, "s", {{0, NULL}}},
 {3, "External Switch/Push Button", "EBM", 48, 2, 0, 2147483647, 0.100, 6553.400, NULL, {{0, NULL}}},
 {3, "2-state switch", "SWT", 50, 1, 0, 2147483647, 0.100, 6553.400, NULL, {{0, NULL}}},
 {3, "Command ID", "CMD", 4, 4, 0, 2147483647, 0.100, 6553.400, NULL, {{0, NULL}}},
 {3, "Extended Command ID", "ECID", 8, 8, 0, 2147483647, 0.100, 6553.400, NULL, {{0, NULL}}},
 {3, "I/O Channel", "I/O", 16, 5, 0, 2147483647, 0.100, 6553.400, NULL, {{0, NULL}}},
 {3, "Maximum Value", "MAXV", 25, 7, 0, 2147483647, 0.100, 6553.400, NULL, {{0, NULL}}},
 {3, "Minimum Value", "MINV", 33, 7, 0, 2147483647, 0.100, 6553.400, NULL, {{0, NULL}}},
 {3, "Command ID", "CMD", 4, 4, 0, 2147483647, 0.100, 6553.400, NULL, {{0, NULL}}},
 {3, "Extended Command ID", "ECID", 8, 8, 0, 2147483647, 0.100, 6553.400, NULL, {{0, NULL}}},
 {3, "I/O Channel", "I/O", 16, 5, 0, 2147483647, 0.100, 6553.400, NULL, {{0, NULL}}},
 {3, "Command ID", "CMD", 4, 4, 0, 2147483647, 0.100, 6553.400, NULL, {{0, NULL}}},
 {3, "Extended Command ID", "ECID", 8, 8, 0, 2147483647, 0.100, 6553.400, NULL, {{0, NULL}}},
 {3, "I/O Channel", "I/O", 16, 5, 0, 2147483647, 0.100, 6553.400, NULL, {{0, NULL}}},
 {3, "Maximum Value", "MAXV", 25, 7, 0, 2147483647, 0.100, 6553.400, NULL, {{0, NULL}}},
 {3, "Minimum Value", "MINV", 33, 7, 0, 2147483647, 0.100, 6553.400, NULL, {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 75, "D2-01-00", "Electronic switches and dimmers with Energy Measurement and Local Control", &D2_01_00[0]) > 0) count++;
static DATAFIELD D2_01_01[] = {
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 0, "D2-01-01", "Electronic switches and dimmers with Energy Measurement and Local Control", &D2_01_01[0]) > 0) count++;
static DATAFIELD D2_03_20[] = {
 {3, "Energy Supply", "ES", 0, 1, 0, 1, 0.000, 1.000, "", {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 1, "D2-03-20", "Beacon with Vibration Detection", &D2_03_20[0]) > 0) count++;
static DATAFIELD D2_14_40[] = {
 {1, "Temperature 10", "TP", 0, 10, 0, 1000, -40.000, 60.000, "℃", {{0, NULL}}},
 {1, "Humidity", "HU", 10, 8, 0, 200, 0.000, 100.000, "%", {{0, NULL}}},
 {1, "Illumination", "IL", 18, 17, 0, 100000, 0.000, 100000.000, "lx", {{0, NULL}}},
 {3, "Acceleration Status", "AS", 35, 2, 0, 3, 0.000, 3.000, "", {{0, NULL}}},
 {1, "Acceleration X", "AX", 37, 10, 0, 1000, -2.500, 2.500, "g", {{0, NULL}}},
 {1, "Acceleration Y", "AY", 47, 10, 0, 1000, -2.500, 2.500, "g", {{0, NULL}}},
 {1, "Acceleration Z", "AZ", 57, 10, 0, 1000, -2.500, 2.500, "g", {{0, NULL}}},
 {1, "", "", 0, 0, 0, 0, 0.000, 0.000, "", {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 8, "D2-14-40", "Multi Function Sensors", &D2_14_40[0]) > 0) count++;
static DATAFIELD D2_14_41[] = {
 {1, "Temperature 10", "TP", 0, 10, 0, 1000, -40.000, 60.000, "℃", {{0, NULL}}},
 {1, "Humidity", "HU", 10, 8, 0, 200, 0.000, 100.000, "%", {{0, NULL}}},
 {1, "Illumination", "IL", 18, 17, 0, 100000, 0.000, 100000.000, "lx", {{0, NULL}}},
 {3, "Acceleration Status", "AS", 35, 2, 0, 3, 0.000, 3.000, "", {{0, NULL}}},
 {1, "Acceleration X", "AX", 37, 10, 0, 1000, -2.500, 2.500, "g", {{0, NULL}}},
 {1, "Acceleration Y", "AY", 47, 10, 0, 1000, -2.500, 2.500, "g", {{0, NULL}}},
 {1, "Acceleration Z", "AZ", 57, 10, 0, 1000, -2.500, 2.500, "g", {{0, NULL}}},
 {3, "Contact", "CO", 67, 1, 0, 1, 0.000, 1.000, "", {{0, NULL}}},
 {1, "", "", 0, 0, 0, 0, 0.000, 0.000, "", {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 9, "D2-14-41", "Multi Function Sensors", &D2_14_41[0]) > 0) count++;
static DATAFIELD D2_32_00[] = {
 {3, "Power Fail", "PF", 0, 1, 0, 1, 0.000, 1.000, "", {{0, NULL}}},
 {3, "Divisor for all channels", "DIV", 1, 1, 0, 1, 0.000, 1.000, "", {{0, NULL}}},
 {1, "Current value", "CH", 8, 12, 0, 4095, 0.000, 4095.000, "A", {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 3, "D2-32-00", "A.C. Current Clamp", &D2_32_00[0]) > 0) count++;
static DATAFIELD D2_32_01[] = {
 {3, "Power Fail", "PF", 0, 1, 0, 1, 0.000, 1.000, "", {{0, NULL}}},
 {3, "Divisor for all channels", "DIV", 1, 1, 0, 1, 0.000, 1.000, "", {{0, NULL}}},
 {1, "Current value", "CH", 8, 12, 0, 4095, 0.000, 4095.000, "A", {{0, NULL}}},
 {1, "Current value", "CH", 20, 12, 0, 4095, 0.000, 4095.000, "A", {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 4, "D2-32-01", "A.C. Current Clamp", &D2_32_01[0]) > 0) count++;
static DATAFIELD D2_32_02[] = {
 {3, "Power Fail", "PF", 0, 1, 0, 1, 0.000, 1.000, "", {{0, NULL}}},
 {3, "Divisor for all channels", "DIV", 1, 1, 0, 1, 0.000, 1.000, "", {{0, NULL}}},
 {1, "Current value", "CH", 8, 12, 0, 4095, 0.000, 4095.000, "A", {{0, NULL}}},
 {1, "Current value", "CH", 20, 12, 0, 4095, 0.000, 4095.000, "A", {{0, NULL}}},
 {1, "Current value", "CH", 32, 12, 0, 4095, 0.000, 4095.000, "A", {{0, NULL}}},
{1,"","",0,0,0,0,0,0,"",{{0, NULL}}} };
if (SaveEep(&EepTable[count], 5, "D2-32-02", "A.C. Current Clamp", &D2_32_02[0]) > 0) count++;

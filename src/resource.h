#define STR_(x) #x
#define STR(x) STR_(x)

#define IDB_ALPHA      101
#define IDB_BACK       102
#define IDB_CLOSE      103
#define IDB_FIT        104
#define IDB_FOREGROUND 105
#define IDB_HALFTONE   106
#define IDB_HELP       107
#define IDB_HOLE       108
#define IDB_LOCK       109
#define IDB_MAXIMIZE   110
#define IDB_MINIMIZE   111
#define IDB_RESET      112
#define IDB_SCALE      113

#define IDI_NAGARAMI 201

#define VER_FILEDESCRIPTION "Nagarami utility"
#define VER_LEGALCOPYRIGHT  "Copyright (c) 2017 Gonbee"
#define VER_PRODUCTNAME     "Nagarami"
#define VER_VERSION\
    VER_MAJOR,\
    VER_MINOR,\
    VER_BUILD,\
    VER_REVISION
#define VER_VERSION_STR\
    STR(VER_MAJOR) "."\
    STR(VER_MINOR) "."\
    STR(VER_BUILD) "."\
    STR(VER_REVISION)
#define VER_MAJOR    0
#define VER_MINOR    1
#define VER_BUILD    4
#define VER_REVISION 0

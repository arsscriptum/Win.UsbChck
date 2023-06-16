
//==============================================================================
//
//     main.cpp
//
//============================================================================
//  Copyright (C) Guilaume Plante 2020 <guillaumeplante.qc@gmail.com>
//==============================================================================


#include <stdio.h>
#include <time.h>
#include <windows.h>
#include <string>
#include<iostream>

using namespace std;

#define MAX_LETTER 26
wchar_t PREV_DRIVE_LIST[MAX_LETTER];
wchar_t NEW_DRIVE_LIST[MAX_LETTER];

/* To GET DRIVE LIST in char ARRAY */
void getUSBStorageDeviceList(wchar_t drive[]) {

    int count = 0;

    char szLogicalDrives[MAX_PATH];
    size_t size = strlen(szLogicalDrives) + 1;
    wchar_t* text = new wchar_t[size];

    size_t outSize;
    mbstowcs_s(&outSize, text, size, szLogicalDrives, size - 1);

    DWORD dwResult = GetLogicalDriveStrings(MAX_PATH, text); // text = szLogicalDrives
    WCHAR* szSingleDrive = text;

    while (*szSingleDrive)
    {
        UINT nDriveType = GetDriveType(szSingleDrive);

        switch (nDriveType) {
            case DRIVE_REMOVABLE: {
                //  cout << "\nDrive type :  Removable Media:" << endl;
                wchar_t letter = szSingleDrive[0];
                drive[letter - 65] = letter;
            }
            case DRIVE_UNKNOWN:
            case DRIVE_NO_ROOT_DIR:
            case DRIVE_FIXED:
            case DRIVE_REMOTE:
            case DRIVE_CDROM:
            case DRIVE_RAMDISK: {
            }
        }

        szSingleDrive += wcslen(szSingleDrive) + 1; // next drive 
    }
}

void usage() {
    printf("usbchck v1.0\n");
    printf("usbchck: monitor usb devices, run a script on change\n");
    printf("example:\n");
    printf("usbchck.exe c:\\scripts\\scan.bat\n");
}


void title(char *scriptPath) {
    printf("usbchck v1.0\n");
    printf("usbchck: will run script \"%s\" when new usb drive are connected...\n\n", scriptPath);
}


int main(int argc, char *argv[]) {

    if (argc != 2) {
        usage();
        return -1;
    }
    char scriptPath[256];
    strncpy(scriptPath, argv[1],255);
    title(scriptPath);
    int count = 0;
    for (int i = 0; i < MAX_LETTER; i++) {
        PREV_DRIVE_LIST[i] = '0';
        NEW_DRIVE_LIST[i] = '0';
    }
    // initial drive list which is already attached 
    getUSBStorageDeviceList(PREV_DRIVE_LIST);

    while (1) {

        getUSBStorageDeviceList(NEW_DRIVE_LIST);
        count = 1;

        /* Check for insertion and removabal*/

        for (int i = 0; i < MAX_LETTER; i++) {
            // check for new drive
            if ((NEW_DRIVE_LIST[i] >= 65 && NEW_DRIVE_LIST[i] <= 89) && (PREV_DRIVE_LIST[i] == '0')) {

                printf("\n[+] New Device Inserted[%d] : %c\n\n", count++, NEW_DRIVE_LIST[i]);
                char newCmd[256];
                snprintf(newCmd, 255, "%s %c", scriptPath,NEW_DRIVE_LIST[i]);
                printf("[+] Running \"%s\"\n", newCmd);
                system(newCmd);
                PREV_DRIVE_LIST[i] = NEW_DRIVE_LIST[i];
            }
        }
        // fill ALl zero 
        for (int i = 0; i < MAX_LETTER; i++) {
            NEW_DRIVE_LIST[i] = '0';
        }
        // update NEW drive list
        getUSBStorageDeviceList(NEW_DRIVE_LIST);

        for (int i = 0; i < MAX_LETTER; i++) {
            // check for removed drive
            if ((PREV_DRIVE_LIST[i] >= 65 && PREV_DRIVE_LIST[i] <= 89) && (NEW_DRIVE_LIST[i] == '0')) {
                printf("\n[-] Device Removed[%d] : %c\n\n", count++, PREV_DRIVE_LIST[i]);
                PREV_DRIVE_LIST[i] = NEW_DRIVE_LIST[i];
            }
        }
        Sleep(500);
    }

    return 0;
}
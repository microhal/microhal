/*
 * fatFsStatusToString.cpp
 *
 *  Created on: May 8, 2019
 *      Author: pokas
 */

#include "fatFsStatusToString.h"

const char *printFatFSResult(FRESULT result) {
    switch (result) {
        case FR_OK:
            return "OK";
        case FR_DISK_ERR:
            return "Disk Error";
        case FR_INT_ERR:
            return "internal error";
        case FR_NOT_READY:
            return "Not ready";
        case FR_NO_FILE:
            return "Could not find the file.";
        case FR_NO_PATH:
            return "Could not find the path.";
        case FR_INVALID_NAME:
            return "The given string is invalid as the path name.";
        case FR_DENIED:
            return "The required access was denied";
        case FR_EXIST:
            return "Name collision. An object with the same name is already existing.";
        case FR_INVALID_OBJECT:
            return "The file/directory object is invalid or a null pointer is given.";
        case FR_WRITE_PROTECTED:
            return "A write mode operation against the write-protected media.";
        case FR_INVALID_DRIVE:
            return "Invalid drive number is specified in the path name.";
        case FR_NOT_ENABLED:
            return "Work area for the logical drive has not been registered by f_mount function.";
        case FR_NO_FILESYSTEM:
            return "There is no valid FAT volume on the drive.";
        case FR_MKFS_ABORTED:
            return "The f_mkfs function aborted.";
        case FR_TIMEOUT:
            return "The function was canceled due to a timeout of thread-safe control.";
        case FR_LOCKED:
            return "The operation to the object was rejected by file sharing control.";
        case FR_NOT_ENOUGH_CORE:
            return "Not enough memory for the operation.";
        case FR_TOO_MANY_OPEN_FILES:
            return "Number of open objects has been reached maximum value and no more object can be opened.";
        case FR_INVALID_PARAMETER:
            return "The given parameter is invalid or there is an inconsistent for the volume.";
        default:
            return "Undefined result code";
    }
}

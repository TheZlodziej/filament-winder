#include "files_scanner.hpp"

#include <algorithm>

/* FATFS definitions (can be found in fatfs.h)*/
extern char USBHPath[4];   /* USBH logical drive path */
extern FATFS USBHFatFS;    /* File system object for USBH logical drive */
extern FIL USBHFile;       /* File object for USBH */

/* defines of above so only two lines need to be changed in order for the code to work */
#define usbh_path USBHPath
#define usbh_fatfs USBHFatFS
#define usbh_file USBHFile

std::string FilesScanner::join_paths(const std::string& path1, const std::string& path2) {
    bool path1_has_sep = path1.ends_with(FilesScanner::_directory_separator);
    bool path2_has_sep = path2.ends_with(FilesScanner::_directory_separator);

    bool sep_in_either = (path1_has_sep && !path2_has_sep) || (!path1_has_sep && path2_has_sep);
    if(sep_in_either) {
        return path1 + path2;
    }

    bool sep_in_both = path1_has_sep && path2_has_sep;
    if(sep_in_both) {
        return path1 + path2.substr(1);
    }

    /* sep in none */
    return path1 + FilesScanner::_directory_separator + path2;
}

FRESULT FilesScanner::read_directory(const std::string& path, std::list<std::string>& save_to) {
    static FILINFO file_info;
    DIR directory;
    FRESULT fresult = f_opendir(&directory, path.c_str());

    if(fresult == FR_OK) {
    	while(true) {
            fresult = f_readdir(&directory, &file_info);
            if(fresult != FR_OK || file_info.fname[0] == '\0') {
            	break;
            }

            std::string full_path = FilesScanner::join_paths(path, std::string(file_info.fname));
            if(file_info.fattrib & AM_DIR) {
                /* a directory */
                bool is_system_info = !(strcmp("SYSTEM~1", file_info.fname) && strcmp("System Volume Information", file_info.fname));
                if(is_system_info) {
                    continue;
                }

                fresult = FilesScanner::read_directory(full_path, save_to);
            }
            else {
                /* a file */
                save_to.emplace_back(full_path);
            }

        }
    }
    
    f_closedir(&directory);
    return fresult;
}

std::list<std::string> FilesScanner::scan_files_in_directory(const std::string& path) {
    std::list<std::string> ret;
    FilesScanner::read_directory(path, ret);
    return ret;
}

std::list<std::string> FilesScanner::filter_files_based_on_extension(const std::list<std::string>& files,  const std::string& extension) {
    std::list<std::string> ret;
    std::copy_if(files.begin(), files.end(), std::back_inserter(ret),
        [&extension](const std::string& filename) {
            return filename.ends_with(extension);
        }
    );
    return ret;
}

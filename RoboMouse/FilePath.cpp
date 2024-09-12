#include "FilePath.h"
#include <ShlObj_core.h>

std::wstring FilePath::getPath(std::wstring fileName)
{
    const KNOWNFOLDERID folderId = FOLDERID_Profile;
    PWSTR path = NULL;
    SHGetKnownFolderPath(folderId, 0, NULL, &path);
    WCHAR filePath[202];
    wsprintf(filePath, L"%ls\\RoboMouseEvents.txt", path);
    return filePath;
}


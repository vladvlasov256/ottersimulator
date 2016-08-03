//
//  FileIOHelper.h
//  CoolOtter
//
//  Created by Vladimir Vlasov on 03.04.16.
//
//

#include <stdio.h>

namespace ottersimulator
{
    void GetFiles(const std::string& dir, std::vector<std::string>& filePathes);
    void GetFiles(const std::string& dir, const std::string& extension, std::vector<std::string>& filePathes);
    void GetUserDirectoryPath(const std::string& path, std::string& fullPath);
    std::string GetPlatformPath(const std::string& path);
}


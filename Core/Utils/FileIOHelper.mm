//
//  FileIOHelper.m
//  CoolOtter
//
//  Created by Vladimir Vlasov on 03.04.16.
//
//

#include <string>
#include <vector>
#include "global.h"
#import "FileIOHelper.h"

using namespace ottersimulator;

//!!!iOS/OS X specific

static NSString *filePrefix = @"file://";

std::string ottersimulator::GetPlatformPath(const std::string& path)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    NSString* platformPathStr = [NSString stringWithFormat:@"%@/%@", [[NSBundle mainBundle]resourcePath], [NSString stringWithUTF8String:path.c_str()]];
    return [platformPathStr UTF8String];
#else
    return std::string(path);
#endif
}

void ottersimulator::GetFiles(const std::string& dir, const std::string& extension, std::vector<std::string>& filePathes)
{
    std::string dirPlatformPath = GetPlatformPath(dir);
    NSURL *dirPathURL = [NSURL URLWithString:[NSString stringWithUTF8String:dirPlatformPath.c_str()]];
    
    NSArray *contents = [[NSFileManager defaultManager] contentsOfDirectoryAtURL:dirPathURL includingPropertiesForKeys:@[] options:NSDirectoryEnumerationSkipsHiddenFiles error:nil];
    NSString* extensionStr = [NSString stringWithUTF8String:extension.c_str()];
    
    for(NSURL *path in contents)
    {
        NSString *strFilePath = [path absoluteString];
        
        NSRange filePrefixRange = [strFilePath rangeOfString:filePrefix];
        if(filePrefixRange.length > 0)
        {
            strFilePath = [strFilePath substringFromIndex:filePrefix.length];
        }
        
        if(extensionStr.length > 0)
        {
            if(strFilePath.length <= extensionStr.length || ![extensionStr isEqualToString:[strFilePath substringFromIndex:(strFilePath.length - extensionStr.length)]])
            {
                continue;
            }
        }
        
        filePathes.push_back([strFilePath UTF8String]);
    }
}

void ottersimulator::GetFiles(const std::string& dir, std::vector<std::string>& filePathes)
{
    GetFiles(dir, "", filePathes);
}

void ottersimulator::GetUserDirectoryPath(const std::string& path, std::string& fullPath)
{
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *documentsDirectory = [paths objectAtIndex:0];
    fullPath = std::string([documentsDirectory UTF8String]) + std::string("/") + path;
}
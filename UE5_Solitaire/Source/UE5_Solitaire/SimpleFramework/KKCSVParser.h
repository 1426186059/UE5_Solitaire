#pragma once

#include "CoreMinimal.h"

class KKCSVParser
{
public:
    static void ParseRow(FString row)
    {
        while (true)
        {

        }
    }
    
private:
    // 禁止拷贝和移动（单例不应被复制）
    KKCSVParser() = delete;
    ~KKCSVParser() = delete;
    KKCSVParser(const KKCSVParser&) = delete;
    KKCSVParser& operator=(const KKCSVParser&) = delete;
    KKCSVParser(KKCSVParser&&) = delete;
    KKCSVParser& operator=(KKCSVParser&&) = delete;
};

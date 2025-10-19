// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

template<typename T>
class TSingleton
{
public:
    static T* GetInstance()
    {
        static T Instance;
        return &Instance;
    }

    // 禁止拷贝和移动（单例不应被复制）
    TSingleton(const TSingleton&) = delete;
    TSingleton& operator=(const TSingleton&) = delete;
    TSingleton(TSingleton&&) = delete;
    TSingleton& operator=(TSingleton&&) = delete;

protected:
    // 允许派生类构造/析构
    TSingleton() = default;
    virtual ~TSingleton() = default;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

template<typename T>
class KKSingleton
{
public:
    static T* GetSingleton()
    {
        static T Instance;
        return &Instance;
    }

protected:
    // 允许派生类构造/析构
    KKSingleton() = default;
    virtual ~KKSingleton() = default;

    // 禁止拷贝和移动（单例不应被复制）
    KKSingleton(const KKSingleton&) = delete;
    KKSingleton& operator=(const KKSingleton&) = delete;
    KKSingleton(KKSingleton&&) = delete;
    KKSingleton& operator=(KKSingleton&&) = delete;
};

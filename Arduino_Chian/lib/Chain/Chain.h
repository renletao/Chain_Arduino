/*
 *SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 *SPDX-License-Identifier: MIT
 */

#ifndef CHAIN_H
#define CHAIN_H

#include "ChainAngle.h"
#include "ChainEncoder.h"
#include "ChainJoystick.h"
#include "ChainKey.h"
#include "ChainToF.h"
#include "ChainUart.h"
class Chain : public ChainEncoder,
              public ChainAngle,
              public ChainJoystick,
              public ChainKey,
              public ChainUart,
              public ChainToF {
public:
    // 定义一个联合体来存储不同类型的设备指针
    void *_dev[256];  // 通用设备指针数组

    // 模板函数：根据索引获取设备指针
    template <typename T>
    T *get_dev_from_index(int index)
    {
        return reinterpret_cast<T *>(_dev[index]);  // 通过 reinterpret_cast 进行类型转换
    }

    // 模板函数：根据类型创建设备实例
    template <typename T>
    void create_DEV(int x)
    {
        T *dev  = new T();                        // 动态创建 T 类型的对象
        _dev[x] = reinterpret_cast<void *>(dev);  // 将创建的设备对象存储到 _dev 数组
    }

private:
};

#endif  // CHAIN_H
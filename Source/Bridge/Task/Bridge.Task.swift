//
// Created by Mengyu Li on 2019/10/18.
// Copyright (c) 2019 limengyu.top. All rights reserved.
//

@_silgen_name("gtr_get")
func c_gtr_get(
        _ task_id: UnsafeMutablePointer<UInt32>, _ url: UnsafePointer<Int8>?,
        _ headers: UnsafePointer<Int8>?, _ time_out: UInt32, _ speed_limit: Int
)

@_silgen_name("gtr_post")
func c_gtr_post(
        _ task_id: UnsafeMutablePointer<UInt32>, _ url: UnsafePointer<Int8>?,
        _ headers: UnsafePointer<Int8>?, _ time_out: UInt32, _ speed_limit: Int,
        _ param_data: UnsafeRawPointer?, _ param_size: UInt
)

@_silgen_name("gtr_custom")
func gtr_custom(
        _ task_id: UnsafeMutablePointer<UInt32>, _ url: UnsafePointer<Int8>?,
        _ headers: UnsafePointer<Int8>?, _ method: UnsafePointer<Int8>?, _ time_out: UInt32,
        _ speed_limit: Int, _ param_data: UnsafeRawPointer?, _ param_size: UInt
)

@_silgen_name("gtr_download")
func c_gtr_download(
        _ task_id: UnsafeMutablePointer<UInt32>?, _ url: UnsafePointer<Int8>?,
        _ filePath: UnsafePointer<Int8>?, _ headers: UnsafePointer<Int8>?,
        _ time_out: UInt32, _ speed_limit: Int
)
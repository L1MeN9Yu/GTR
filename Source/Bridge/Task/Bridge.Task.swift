//
// Created by Mengyu Li on 2019/10/18.
// Copyright (c) 2019 limengyu.top. All rights reserved.
//

@_silgen_name("gtr_download")
func c_gtr_download(
        _ task_id: UnsafeMutablePointer<UInt32>?, _ url: UnsafePointer<Int8>?,
        _ filePath: UnsafePointer<Int8>?, _ headers: UnsafePointer<Int8>?,
        _ time_out: UInt32, _ speed_limit: Int
)

@_silgen_name("gtr_data_task_create")
func gtr_data_task_create(_ task_id: UnsafeMutablePointer<UInt32>, _ url: UnsafePointer<Int8>?) -> OpaquePointer

@_silgen_name("gtr_data_task_add_header")
func gtr_data_task_add_header(_ data_task: OpaquePointer, _ value: UnsafePointer<Int8>!)

@_silgen_name("gtr_data_task_config_parameters")
func gtr_data_task_config_parameters(_ core_race: OpaquePointer, _ method: UnsafePointer<Int8>?, _ param_data: UnsafeRawPointer?, _ param_size: UInt)

@_silgen_name("gtr_data_task_add_form_data")
func gtr_data_task_add_form_data(_ data_task: OpaquePointer, _ type: Int32, _ name: UnsafePointer<Int8>?, _ value: UnsafePointer<Int8>?)

@_silgen_name("gtr_data_task_config_options")
func gtr_data_task_config_options(_ core_race: OpaquePointer, _ is_debug: Bool, _ time_out: UInt32, _ max_redirects: Int)

@_silgen_name("gtr_data_task_config_response_info_options")
func gtr_data_task_config_response_info_options(_ data_task: OpaquePointer, _ base: Bool, _ time: Bool, _ size: Bool, _ speed: Bool, _ ssl: Bool, _ socket: Bool, _ cookie: Bool)

@_silgen_name("gtr_data_task_config_speed")
func gtr_data_task_config_speed(_ core_race: OpaquePointer, _ max_receive_speed: Int, _ max_send_speed: Int, _ low_speed_limit: Int, _ low_speed_time: Int)

@_silgen_name("gtr_data_task_config_proxy")
func gtr_data_task_config_proxy(_ core_race: OpaquePointer, _ url: UnsafePointer<Int8>?, _ port: Int)

@_silgen_name("gtr_data_task_start")
func gtr_data_task_start(_ core_race: OpaquePointer)
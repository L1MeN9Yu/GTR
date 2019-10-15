//
// Created by Mengyu Li on 2019/10/18.
// Copyright (c) 2019 limengyu.top. All rights reserved.
//

@_silgen_name("swift_data_task_succeed")
func swift_data_task_succeed(_ task_id: UInt32, _ c_header_data: UnsafeRawPointer, _ c_header_data_size: UInt, _ c_body_data: UnsafeRawPointer, _ c_body_data_size: UInt) {
    __engine.dataTaskSucceed(task_id: task_id, c_header_data: c_header_data, c_header_data_size: c_header_data_size, c_body_data: c_body_data, c_body_data_size: c_body_data_size)
}

@_silgen_name("swift_data_task_failed")
func swift_data_task_failed(_ task_id: UInt32, _ http_response_code: Int, _ error_code: Int32, _ error_message: UnsafePointer<Int8>) {
    __engine.dataTaskFailed(task_id: task_id, http_response_code: http_response_code, error_code: error_code, error_message: error_message)
}
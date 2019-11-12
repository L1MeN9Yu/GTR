//
// Created by Mengyu Li on 2019/10/18.
// Copyright (c) 2019 limengyu.top. All rights reserved.
//

@_silgen_name("gtr_init")
func c_gtr_init(_ user_agent: UnsafePointer<Int8>?, _ cylinder_count: UInt32)

// MARK: - C Bridge
@_silgen_name("swift_log_callback")
func c_log_callback(c_flag: CUnsignedInt, c_message: UnsafePointer<CChar>?) {
    if let c_message = c_message {
        if let message = String(cString: c_message, encoding: .utf8) {
            if let type = HornType(flag: c_flag) {
                return whistle(type: type, message: message.trimmingCharacters(in: .whitespacesAndNewlines))
            }

            return whistle(type: .critical, message: "flag不存在!!!" + "\n" + "message : \(message)")
        }
    }
}
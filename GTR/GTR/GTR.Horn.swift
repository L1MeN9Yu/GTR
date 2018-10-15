//
// Created L1MeN9Yu on 2018/8/7.
// Copyright (c) 2018 limengyu.top. All rights reserved.
//

public protocol Horn {
    static func whistle(message: String, filename: String, function: String, line: Int)
}

extension Horn {
    public static func whistle(message: String, filename: String, function: String, line: Int) {
        let allMessage = "[\(URL(fileURLWithPath: filename).lastPathComponent):\(line)] \(function) - \(message)"
        print(allMessage)
    }
}

// MARK: - C Bridge
@_silgen_name("swift_log_callback")
func c_log_callback(c_message: UnsafePointer<CChar>?) {
    if let c_message = c_message {
        if let message = String(cString: c_message, encoding: .utf8) {
            GTR.whistle(message: message)
        }
    }
}
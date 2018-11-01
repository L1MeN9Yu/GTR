//
// Created L1MeN9Yu on 2018/8/7.
// Copyright (c) 2018 limengyu.top. All rights reserved.
//

public protocol Horn {
    static func whistle(type: HornType, message: String, filename: String, function: String, line: Int)
}

extension Horn {
    public static func whistle(type: HornType, message: String, filename: String, function: String, line: Int) {
        let allMessage = "[\(type.name)] ======>>>[\(URL(fileURLWithPath: filename).lastPathComponent):\(line)] \(function) - \(message)"
        print(allMessage)
    }
}

// MARK: - C Bridge
@_silgen_name("swift_log_callback")
func c_log_callback(c_flag: CUnsignedInt, c_message: UnsafePointer<CChar>?) {
    if let c_message = c_message {
        if let message = String(cString: c_message, encoding: .utf8) {
            if let type = HornType(flag: c_flag) {
                return GTR.whistle(type: type, message: message)
            }

            return GTR.whistle(type: .criticalError, message: "flag不存在!!!" + "\n" + "message : \(message)")
        }
    }
}

public enum HornType {
    case trace
    case debug
    case info
    case warning
    case error
    case criticalError

    init?(flag: CUnsignedInt) {
        switch flag {
        case 0:
            self = .trace
        case 1:
            self = .debug
        case 2:
            self = .info
        case 3:
            self = .warning
        case 4:
            self = .error
        case 5:
            self = .criticalError
        default:
            return nil
        }
    }

    public var name: String {
        switch self {
        case .trace:
            return "trace:"
        case .debug:
            return "debug:"
        case .info:
            return "info:"
        case .warning:
            return "warning:"
        case .error:
            return "error:"
        case .criticalError:
            return "criticalError:"
        }
    }
}
//
// Created by Mengyu Li on 2019/11/12.
// Copyright (c) 2019 limengyu.top. All rights reserved.
//

import Foundation

public struct ResponseInfo: Codable {
    public let base: Base?
    public let time: Time?
    public let size: Size?
    public let speed: Speed?
    public let ssl: SSL?
    public let socket: Socket?
    public let cookie: [String]?

    enum CodingKeys: String, CodingKey {
        case base = "BASE_INFO"
        case time = "TIME_INFO"
        case size = "SIZE_INFO"
        case speed = "SPEED_INFO"
        case ssl = "SSL_INFO"
        case socket = "SOCKET_INFO"
        case cookie = "COOKIE_LIST"
    }
}

extension ResponseInfo: CustomStringConvertible {
    public var description: String {
        var ret: String = ""
        if let base = base?.description {
            ret.append(contentsOf: """

                                   base => :
                                   \(base)
                                   """)
        }
        if let time = time?.description {
            ret.append(contentsOf: """

                                   time => :
                                   \(time)
                                   """)
        }
        if let size = size?.description {
            ret.append(contentsOf: """

                                   size => :
                                   \(size)
                                   """)
        }
        if let speed = speed?.description {
            ret.append(contentsOf: """

                                   speed => :
                                   \(speed)
                                   """)
        }
        if let ssl = ssl?.description {
            ret.append(contentsOf: """

                                   ssl => :
                                   \(ssl)
                                   """)
        }
        if let socket = socket?.description {
            ret.append(contentsOf: """

                                   socket => :
                                   \(socket)
                                   """)
        }
        if let cookie = cookie?.description {
            ret.append(contentsOf: """

                                   cookie => :
                                   \(cookie)
                                   """)
        }

        return ret
    }
}